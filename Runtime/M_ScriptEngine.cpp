#include "pch.h"
#include "M_ScriptEngine.h"

#include "M_Mono.h"

#include "M_Runtime.h"
#include "M_Scene.h"
#include "M_SceneManager.h"
#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"

#include "M_File.h"
#include "M_Console.h"
#include "M_ScriptObject.h"
#include "M_ScriptArguments.h"

using namespace minty;
using namespace minty::Scripting;

//
//		SCRIPT ENGINE
//

struct ScriptEngineData
{
	ScriptEngine* engine = nullptr;

	// uuid, script object
	std::unordered_map<UUID, ScriptObject> objects;

	Scene* get_scene()
	{
		return engine->get_runtime().get_scene_manager().get_working_scene();
	}

	void reset()
	{
		// destroy all objects
		for (auto& pair : objects)
		{
			pair.second.destroy();
		}
		objects.clear();
	}
};

static ScriptEngineData _data;

// SCRIPT FUNCTIONS C++ -> C#
/*
* OnCreate
* OnDestroy
* OnLoad
* OnUnload
* OnUpdate
* OnFixedUpdate / physics update
* OnFinalize / late update
* OnEnable
* OnDisable
*/

// SCRIPT FUNCTIONS C# -> C++
// a lot, check ScriptLinkage.cpp

minty::ScriptEngine::ScriptEngine()
	: _rootDomain()
	, _appDomain()
	, _assemblies()
{
	mono_set_dirs("C:/Program Files/Mono/lib", "C:/Program Files/Mono/etc");

	_rootDomain = mono_jit_init("MintyRuntime");
	MINTY_ASSERT(_rootDomain != nullptr);

	mono_set_assemblies_path("C:/Libraries/Mono/lib");

	String appDomainName = "MintyDomain";
	_appDomain = mono_domain_create_appdomain(appDomainName.data(), nullptr);
	mono_domain_set(_appDomain, true);

	_data.engine = this;
	_data.reset();
}

minty::ScriptEngine::~ScriptEngine()
{
	_data.reset();

	for (auto& pair : _assemblies)
	{
		delete pair.second;
	}
	_assemblies.clear();

	mono_jit_cleanup(_rootDomain);
	_rootDomain = nullptr;
	_appDomain = nullptr;
}

void minty::ScriptEngine::set_runtime(Runtime& runtime)
{
	if (&get_runtime() != &runtime)
	{
		Engine::set_runtime(runtime);

		_data.reset();
	}
}

bool minty::ScriptEngine::load_assembly(Path const& path, bool const referenceOnly)
{
	String name = path.stem().string();

	// delete old assembly if there is one
	unload_assembly(name);

	// load the new assembly
	ScriptAssembly* scriptAssembly = _assemblies.emplace(name, new ScriptAssembly(path, *this, referenceOnly)).first->second;

	// if the Script script has been loaded, then load all scripts derive from it
	ScriptClass const* scriptScriptClass = find_class(ASSEMBLY_ENGINE_NAME, "Script");

	if (scriptScriptClass)
	{
		// register all Scripts with the EntityRegistry
		for (auto const& script : scriptAssembly->get_classes(scriptScriptClass))
		{
			if (script->is_derived_from(*scriptScriptClass))
			{
				EntityRegistry::register_script(script->get_full_name());
			}
		}
	}

	return true;
}

ScriptAssembly const* minty::ScriptEngine::find_assembly(String const& namespaceName, String const& className) const
{
	for (auto const& pair : _assemblies)
	{
		if (pair.second->get_class(namespaceName, className))
		{
			return pair.second;
		}
	}

	return nullptr;
}

void minty::ScriptEngine::reload_assembly(String const& name)
{
	Console::todo("reload assembly");
}

void minty::ScriptEngine::unload_assembly(String const& name)
{
	// delete assembly if there is one
	auto found = _assemblies.find(name);
	if (found != _assemblies.end())
	{
		delete found->second;
		_assemblies.erase(name);
	}
}

ScriptAssembly const* minty::ScriptEngine::get_assembly(String const& assemblyName) const
{
	auto found = _assemblies.find(assemblyName);

	if (found != _assemblies.end())
	{
		return found->second;
	}

	return nullptr;
}

MonoString* minty::ScriptEngine::to_mono_string(String const& string) const
{
	return mono_string_new(_appDomain, string.c_str());
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

void minty::ScriptEngine::get_property_value(MonoObject* const object, MonoProperty* const prop, void* out) const
{
	MonoObject* exception = nullptr;

	mono_property_get_value(prop, object, &out, &exception);

	if (exception)
	{
		MINTY_ABORT(get_exception_message(exception));
	}
}

void minty::ScriptEngine::set_property_value(MonoObject* const object, MonoProperty* const prop, void* in) const
{
	MonoObject* exception = nullptr;

	mono_property_set_value(prop, object, &in, &exception);

	if (exception)
	{
		MINTY_ABORT(get_exception_message(exception));
	}
}

void minty::ScriptEngine::invoke_method(MonoObject* const object, MonoMethod* const method) const
{
	MonoObject* exception = nullptr;
	mono_runtime_invoke(method, object, nullptr, &exception);

	if (exception)
	{
		//MINTY_ABORT(get_exception_message(exception));
		MINTY_ERROR(get_exception_message(exception));
	}
}

void minty::ScriptEngine::invoke_method(MonoObject* const object, MonoMethod* const method, std::vector<void*>& arguments) const
{
	MonoObject* exception = nullptr;
	mono_runtime_invoke(method, object, arguments.data(), &exception);

	if (exception)
	{
		MINTY_ABORT(get_exception_message(exception));
	}
}

String minty::ScriptEngine::get_exception_message(MonoObject* const exception) const
{
	if (!exception) {
		return "";
	}

	MonoMethod* get_message_method = mono_class_get_method_from_name(mono_get_exception_class(), "get_Message", 0);
	if (!get_message_method) {
		//std::cerr << "Could not find the get_Message method." << std::endl;
		return "";
	}

	MonoObject* exc = nullptr;
	MonoObject* result = mono_runtime_invoke(get_message_method, exception, nullptr, &exc);
	if (exc) {
		// Handle any exception that might occur during the get_Message invocation
		//std::cerr << "Exception occurred while getting message." << std::endl;
		return "";
	}

	String out = Text::EMPTY;

	if (result) {
		char* str = mono_string_to_utf8((MonoString*)result);
		//std::cout << "Exception message: " << str << std::endl;
		out = str;
		mono_free(str);
	}
	else {
		//std::cout << "No exception message." << std::endl;
	}

	return out;
}

MonoObject* minty::ScriptEngine::create_instance(MonoClass* const klass) const
{
	// create the instance
	MonoObject* instance = mono_object_new(_appDomain, klass);

	MINTY_ASSERT(instance != nullptr);

	// call the constructor
	mono_runtime_object_init(instance);

	return instance;
}

MonoObject* minty::ScriptEngine::create_instance(MonoClass* const klass, std::vector<void*>& values) const
{
	if (values.size() == 0)
	{
		// no arguments, use less complicated create_instance
		return create_instance(klass);
	}

	// create the instance
	MonoObject* instance = mono_object_new(_appDomain, klass);

	MINTY_ASSERT(instance != nullptr);

	// get the name and namespace
	char const* name = mono_class_get_name(klass);
	char const* nameSpace = mono_class_get_namespace(klass);

	// get the constructor with the arguments
	MonoMethod* ctor = mono_class_get_method_from_name(klass, ".ctor", static_cast<int>(values.size()));
	MINTY_ASSERT(ctor != nullptr);

	// call the constructor
	invoke_method(instance, ctor, values);

	// done
	return instance;
}

void* minty::ScriptEngine::unbox(MonoObject* const object) const
{
	return mono_object_unbox(object);
}

String minty::ScriptEngine::get_full_name(String const& namespaceName, String const& className)
{
	if (namespaceName.empty())
	{
		return className;
	}
	else
	{
		return std::format("{}.{}", namespaceName, className);
	}
}

ScriptClass const* minty::ScriptEngine::find_class(String const& namespaceName, String const& className) const
{
	return find_class(get_full_name(namespaceName, className));
}

ScriptClass const* minty::ScriptEngine::find_class(String const& fullName) const
{
	for (auto const& pair : _assemblies)
	{
		if (ScriptClass const* scriptClass = pair.second->get_class(fullName))
		{
			return scriptClass;
		}
	}

	return nullptr;
}

ScriptObject const& minty::ScriptEngine::create_object(ScriptClass const& script, UUID id) const
{
	return _data.objects.emplace(id, ScriptObject(script)).first->second;
}

ScriptObject const& minty::ScriptEngine::create_object(ScriptClass const& script, UUID id, ScriptArguments& scriptArguments) const
{
	return _data.objects.emplace(id, ScriptObject(script, scriptArguments)).first->second;
}

ScriptObject const* minty::ScriptEngine::get_object(UUID id) const
{
	auto found = _data.objects.find(id);

	if (found != _data.objects.end())
	{
		return &found->second;
	}

	return nullptr;
}

ScriptObject const& minty::ScriptEngine::get_or_create_entity(UUID id)
{
	// try get
	ScriptObject const* entityObject = get_object(id);

	// if found, return this
	if (entityObject) return *entityObject;

	// create new
	return create_object_entity(id);
}

ScriptObject const& minty::ScriptEngine::create_object_entity(UUID id)
{
	// get the entity class
	ScriptAssembly const* engineAssembly = get_assembly(ASSEMBLY_ENGINE_NAME);
	MINTY_ASSERT(engineAssembly != nullptr);

	ScriptClass const* entityClass = engineAssembly->get_class(ASSEMBLY_ENGINE_NAME, "Entity");
	MINTY_ASSERT(entityClass != nullptr);

	// create an instance of it
	uint64_t rawId = static_cast<uint64_t>(id);
	ScriptArguments args({ &rawId });
	ScriptObject const& object = create_object(*entityClass, id, args);

	return object;
}

ScriptObject const& minty::ScriptEngine::create_object_component(UUID id, UUID const entityId, ScriptClass const& script)
{
	// get the component class
	ScriptAssembly const* engineAssembly = get_assembly(ASSEMBLY_ENGINE_NAME);
	MINTY_ASSERT(engineAssembly != nullptr);

	ScriptClass const* componentClass = engineAssembly->get_class("MintyEngine", "Component");
	MINTY_ASSERT(componentClass != nullptr);

	// check the new instance class
	MINTY_ASSERT(script.is_derived_from(*componentClass));

	// get entity object
	ScriptObject const* entity = get_object(entityId);
	MINTY_ASSERT(entity != nullptr);
	MonoObject* entityObject = entity->get_object();
	MINTY_ASSERT(entityObject != nullptr);

	// spawn the object and save the entity to it
	ScriptObject const& object = create_object(script, id);
	object.set_property("Entity", entityObject);

	return object;
}

void minty::ScriptEngine::destroy_object(UUID id)
{
	// find the object and destroy if found
	auto found = _data.objects.find(id);

	if (found != _data.objects.end())
	{
		found->second.destroy();
		_data.objects.erase(id);
	}
}

//
//		LINKING
//

constexpr static char const* INTERNAL_CLASS_NAME = "Runtime";

#define ADD_INTERNAL_CALL(csharpName, cppName) mono_add_internal_call(std::format("{}.{}::{}", ASSEMBLY_ENGINE_NAME, INTERNAL_CLASS_NAME, csharpName).c_str(), cppName)

static String mono_string_to_string(MonoString* const string)
{
	char* str = mono_string_to_utf8(string);
	std::string result(str);
	mono_free(str);
	return result;
}

#pragma region Console

static void console_log(MonoString* string)
{
	Console::log(mono_string_to_string(string));
}

static void console_log_color(MonoString* string, int color)
{
	Console::log_color(mono_string_to_string(string), static_cast<Console::Color>(color));
}

static void console_warn(MonoString* string)
{
	Console::warn(mono_string_to_string(string));
}

static void console_error(MonoString* string)
{
	Console::error(mono_string_to_string(string));
}

static void console_ass(bool condition, MonoString* string)
{
	Console::ass(condition, mono_string_to_string(string));
}

#pragma endregion

#pragma region Entity

static MonoString* entity_get_name(uint64_t id)
{
	if (!id)
	{
		return _data.engine->to_mono_string("");
	}

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	// get the entity
	Entity entity = registry.find(id);

	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the name
	String name = scene->get_entity_registry().get_name(entity);

	// convert to mono string and return
	return _data.engine->to_mono_string(name);
}

#pragma endregion

void minty::ScriptEngine::link()
{
	ADD_INTERNAL_CALL("Console_Log", console_log);
	ADD_INTERNAL_CALL("Console_LogColor", console_log_color);
	ADD_INTERNAL_CALL("Console_Warn", console_warn);
	ADD_INTERNAL_CALL("Console_Error", console_error);
	ADD_INTERNAL_CALL("Console_Assert", console_ass);

	ADD_INTERNAL_CALL("Entity_GetName", entity_get_name);
}

#undef ADD_INTERNAL_CALL