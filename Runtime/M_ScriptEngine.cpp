#include "pch.h"
#include "M_ScriptEngine.h"

#include "M_Mono.h"
#include "M_EntityRegistry.h"

#include "M_File.h"
#include "M_Console.h"

using namespace minty;
using namespace minty::Scripting;

struct ScriptEngineData
{
	Scene* scene;
};

static ScriptEngineData _data;

minty::ScriptEngine::ScriptEngine()
	: _rootDomain()
	, _appDomain()
	, _assemblies()
{
	mono_set_assemblies_path("C:/Libraries/Mono/lib");

	_rootDomain = mono_jit_init("MintyRuntime");
	if (!_rootDomain)
	{
		Console::error("Failed to init Mono root domain.");
		return;
	}

	String appDomainName = "MintyDomain";
	_appDomain = mono_domain_create_appdomain(appDomainName.data(), nullptr);
	mono_domain_set(_appDomain, true);
}

minty::ScriptEngine::~ScriptEngine()
{
	for (auto const& pair : _assemblies)
	{
		mono_assembly_close(pair.second.assembly);
	}
	_assemblies.clear();
}

void minty::ScriptEngine::set_scene(Scene* scene)
{
	Engine::set_scene(scene);

	_data.scene = scene;
}

bool minty::ScriptEngine::load_assembly(AssemblyType const type, Path const& path)
{
	MonoAssembly* assembly = load_mono_assembly(path);

	if (!assembly)
	{
		// did not load successfully
		Console::error(std::format("Could not load assembly of type {} at path \"{}\".", static_cast<int>(type), path.string()));

		return false;
	}

	// loaded successfully

	// delete old assembly if there is one
	auto found = _assemblies.find(type);
	if (found != _assemblies.end())
	{
		mono_assembly_close(found->second.assembly);
	}

	// assign it for reference
	_assemblies[type] = AssemblyInfo
	{
		.assembly = assembly,
		.scripts = std::unordered_map<String, Script>()
	};
	AssemblyInfo& info = _assemblies[type];

	// load all of the scripts
	MonoClass* scriptBaseClass = get_class(AssemblyType::Engine, "MintyEngine", "Script");
	std::vector<MonoClass*> scriptClasses = get_classes(type, scriptBaseClass);
	for (MonoClass* const klass : scriptClasses)
	{
		String name = get_class_name(klass);

		// add script by name to the assembly
		info.scripts.emplace(name, Script(*this, type, *klass));

		// also register with the entity registry so it can be added/removed as a "component"
		EntityRegistry::register_script(name);
	}

	Console::log(std::format("Assembly at \"{}\" loaded {} scripts.", path.string(), scriptClasses.size()));

	return true;
}

void minty::ScriptEngine::unload_assembly(AssemblyType const type)
{
	// delete assembly if there is one
	auto found = _assemblies.find(type);
	if (found != _assemblies.end())
	{
		mono_assembly_close(found->second.assembly);
		_assemblies.erase(type);
	}
}

void print_assembly(MonoAssembly* const assembly)
{
	MonoImage* assemblyImage = mono_assembly_get_image(assembly);
	MonoTableInfo const* typeDefinitionsTable = mono_image_get_table_info(assemblyImage, MONO_TABLE_TYPEDEF);
	int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

	for (int32_t i = 0; i < numTypes; i++)
	{
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

		char const* nameSpace = mono_metadata_string_heap(assemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
		char const* name = mono_metadata_string_heap(assemblyImage, cols[MONO_TYPEDEF_NAME]);

		Console::log(std::format("{}.{}", nameSpace, name));
	}
}

MonoAssembly* minty::ScriptEngine::load_mono_assembly(Path const& path) const
{
	if (!std::filesystem::exists(path))
	{
		return nullptr;
	}

	Console::log(std::format("Loading Assembly at path: \"{}\"", path.string()));
	std::vector<char> fileData = File::read_all_chars(path);

	bool const refOnly = false; // set refonly to true to do reflection stuffs

	// load the assembly image
	MonoImageOpenStatus status;
	MonoImage* image = mono_image_open_from_data_full(fileData.data(), static_cast<uint32_t>(fileData.size()), 1, &status, refOnly);

	// check for error
	if (status != MONO_IMAGE_OK)
	{
		// get the error message
		char const* errorMessage = mono_image_strerror(status);

		Console::error(std::format("ScriptEngine::load_mono_assembly(): Mono error: \"{}\"", errorMessage));

		return nullptr;
	}

	// load the assembly itself
	MonoAssembly* assembly = mono_assembly_load_from_full(image, path.string().c_str(), &status, refOnly);
	mono_image_close(image);

	print_assembly(assembly);

	return assembly;
}


MonoAssembly* minty::ScriptEngine::get_assembly(AssemblyType const assm) const
{
	return _assemblies.at(assm).assembly;
}

MonoClass* minty::ScriptEngine::get_class(AssemblyType const assm, String const& namespaceName, String const& className) const
{
	MonoImage* image = mono_assembly_get_image(get_assembly(assm));
	MonoClass* klass = mono_class_from_name(image, namespaceName.c_str(), className.c_str());

	return klass;
}

std::vector<MonoClass*> minty::ScriptEngine::get_classes(AssemblyType const assm, MonoClass* const baseKlass) const
{
	std::vector<MonoClass*> classes;

	MonoAssembly* assembly = get_assembly(assm);
	MonoImage* image = mono_assembly_get_image(assembly);
	const MonoTableInfo* tableInfo = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
	int rows = mono_table_info_get_rows(tableInfo);

	for (int i = 1; i < rows; i++)
	{
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(tableInfo, i, cols, MONO_TYPEDEF_SIZE);
		char const* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
		char const* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
		MonoClass* klass = mono_class_from_name(image, nameSpace, name);

		if (baseKlass && (klass == baseKlass || !check_class_inheritance(klass, baseKlass)))
		{
			// if there is a base class, and it does not inherit from it, ignore
			continue;
		}

		// no base class or it does inherit from the base class, so add
		classes.push_back(klass);
	}

	return classes;
}

String minty::ScriptEngine::get_class_name(MonoClass* const klass) const
{
	return mono_class_get_name(klass);
}

String minty::ScriptEngine::get_class_namespace(MonoClass* const klass) const
{
	return mono_class_get_namespace(klass);
}

bool minty::ScriptEngine::check_class_inheritance(MonoClass* const klass, MonoClass* const baseKlass) const
{
	// check each parent of each parent to see if klass derives from baseKlass at any point
	MonoClass* currentKlass = klass;
	while (currentKlass != nullptr && currentKlass != baseKlass)
	{
		currentKlass = mono_class_get_parent(currentKlass);
	}

	// if not null, it is derived from the base class
	return currentKlass != nullptr;
}

MonoMethod* minty::ScriptEngine::get_method(MonoClass* const klass, String const& methodName, int const paramCount) const
{
	return mono_class_get_method_from_name(klass, methodName.c_str(), paramCount);
}

std::vector<MonoMethod*> minty::ScriptEngine::get_methods(MonoClass* const klass) const
{
	void* iter = nullptr;
	MonoMethod* method;
	std::vector<MonoMethod*> methods;

	while (method = mono_class_get_methods(klass, &iter))
	{
		methods.push_back(method);
	}

	return methods;
}

Accessibility minty::ScriptEngine::get_method_accessibility(MonoMethod* const method) const
{
	Accessibility access = Accessibility::None;
	uint32_t accessFlags = mono_method_get_flags(method, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;

	switch (accessFlags)
	{
	case MONO_METHOD_ATTR_PRIVATE:
		access = Accessibility::Private;
		break;
	case MONO_METHOD_ATTR_FAM_AND_ASSEM:
		access = static_cast<Accessibility>(static_cast<uint8_t>(Accessibility::Protected) | static_cast<uint8_t>(Accessibility::Internal));
		break;
	case MONO_METHOD_ATTR_ASSEM:
		access = Accessibility::Internal;
		break;
	case MONO_METHOD_ATTR_FAMILY:
		access = Accessibility::Protected;
		break;
	case MONO_METHOD_ATTR_FAM_OR_ASSEM:
		access = static_cast<Accessibility>(static_cast<uint8_t>(Accessibility::Private) | static_cast<uint8_t>(Accessibility::Protected));
		break;
	case MONO_METHOD_ATTR_PUBLIC:
		access = Accessibility::Public;
		break;
	}

	return access;
}

std::vector<MonoMethod*> minty::ScriptEngine::get_methods(MonoClass* const klass, Accessibility const accessibility) const
{
	void* iter = nullptr;
	MonoMethod* method;
	std::vector<MonoMethod*> methods;

	while (method = mono_class_get_methods(klass, &iter))
	{
		if ((get_method_accessibility(method) & accessibility) == accessibility)
		{
			methods.push_back(method);
		}
	}

	return methods;
}

String minty::ScriptEngine::get_method_name(MonoMethod* const method) const
{
	return mono_method_get_name(method);
}

MonoClassField* minty::ScriptEngine::get_field(MonoClass* const klass, String const& fieldName) const
{
	MonoClassField* field = mono_class_get_field_from_name(klass, fieldName.c_str());

	if (!field)
	{
		Console::error(std::format("Assembly::get_field(): Field \"{}\" does not exist.", fieldName));
		return nullptr;
	}

	return field;
}

Accessibility minty::ScriptEngine::get_field_accessibility(MonoClassField* const field) const
{
	Accessibility access = Accessibility::None;
	uint32_t accessFlags = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

	switch (accessFlags)
	{
	case MONO_FIELD_ATTR_PRIVATE:
		access = Accessibility::Private;
		break;
	case MONO_FIELD_ATTR_FAM_AND_ASSEM:
		access = static_cast<Accessibility>(static_cast<uint8_t>(Accessibility::Protected) | static_cast<uint8_t>(Accessibility::Internal));
		break;
	case MONO_FIELD_ATTR_ASSEMBLY:
		access = Accessibility::Internal;
		break;
	case MONO_FIELD_ATTR_FAMILY:
		access = Accessibility::Protected;
		break;
	case MONO_FIELD_ATTR_FAM_OR_ASSEM:
		access = static_cast<Accessibility>(static_cast<uint8_t>(Accessibility::Private) | static_cast<uint8_t>(Accessibility::Protected));
		break;
	case MONO_FIELD_ATTR_PUBLIC:
		access = Accessibility::Public;
		break;
	}

	return access;
}

std::vector<MonoClassField*> minty::ScriptEngine::get_fields(MonoClass* const klass) const
{
	// iterate through each field, put them in a list
	void* iter = nullptr;
	MonoClassField* field;
	std::vector<MonoClassField*> fields;

	while (field = mono_class_get_fields(klass, &iter))
	{
		fields.push_back(field);
	}

	return fields;
}

std::vector<MonoClassField*> minty::ScriptEngine::get_fields(MonoClass* const klass, Accessibility const accessibility) const
{
	// iterate through each field, put them in a list if they match the accessibility
	void* iter = nullptr;
	MonoClassField* field;
	std::vector<MonoClassField*> fields;

	while (field = mono_class_get_fields(klass, &iter))
	{
		if ((get_field_accessibility(field) & accessibility) == accessibility)
		{
			fields.push_back(field);
		}
	}

	return fields;
}

String minty::ScriptEngine::get_field_name(MonoClassField* const field) const
{
	return mono_field_get_name(field);
}

MonoType* minty::ScriptEngine::get_field_type(MonoClassField* const field) const
{
	return mono_field_get_type(field);
}

void minty::ScriptEngine::get_field_value(MonoObject* const object, MonoClassField* const field, void* out) const
{
	mono_field_get_value(object, field, out);
}

void minty::ScriptEngine::set_field_value(MonoObject* const object, MonoClassField* const field, void* in) const
{
	mono_field_set_value(object, field, in);
}

MonoProperty* minty::ScriptEngine::get_property(MonoClass* const klass, String const& propertyName) const
{
	MonoProperty* prop = mono_class_get_property_from_name(klass, propertyName.c_str());

	if (!prop)
	{
		Console::error(std::format("Assembly::get_field(): Field \"{}\" does not exist.", propertyName));
		return nullptr;
	}

	return prop;
}

void minty::ScriptEngine::invoke_method(MonoObject* const object, MonoMethod* const method) const
{
	std::vector<void*> empty;
	invoke_method(object, method, empty);
}

void minty::ScriptEngine::invoke_method(MonoObject* const object, MonoMethod* const method, std::vector<void*>& arguments) const
{
	MonoObject* exception = nullptr;
	mono_runtime_invoke(method, object, arguments.data(), &exception);

	if (exception)
	{
		Console::error("Assembly::invoke_method(): An exception was thrown.");
		return;
	}
}

MonoObject* minty::ScriptEngine::create_instance(MonoClass* const klass) const
{
	// create the instance
	MonoObject* instance = mono_object_new(_appDomain, klass);

	if (!instance)
	{
		Console::error("Assembly::create_instance(): Failed.");
		return nullptr;
	}

	// call the constructor
	mono_runtime_object_init(instance);

	return instance;
}

Script* minty::ScriptEngine::get_script(AssemblyType const assm, String const& name)
{
	AssemblyInfo& info = _assemblies.at(assm);

	// if name found, return that script
	auto found = info.scripts.find(name);

	if (found != info.scripts.end())
	{
		return &found->second;
	}

	return nullptr;
}

Script const* minty::ScriptEngine::get_script(AssemblyType const assm, String const& name) const
{
	AssemblyInfo const& info = _assemblies.at(assm);

	// if name found, return that script
	auto const found = info.scripts.find(name);

	if (found != info.scripts.end())
	{
		return &found->second;
	}

	return nullptr;
}

Script* minty::ScriptEngine::get_script(String const& name)
{
	// go through each assembly type
	for (auto& pair : _assemblies)
	{
		Script* script = get_script(pair.first, name);

		if (script) return script;
	}

	return nullptr;
}

Script const* minty::ScriptEngine::get_script(String const& name) const
{
	// go through each assembly type
	for (auto const& pair : _assemblies)
	{
		Script const* script = get_script(pair.first, name);

		if (script) return script;
	}

	return nullptr;
}
