#include "pch.h"
#include "Minty/Scripting/M_ScriptEngine.h"

#include "Minty/Libraries/M_Mono.h"

#include "Minty/Scenes/M_Scene.h"
#include "Minty/Scenes/M_SceneManager.h"
#include "Minty/Entities/M_EntityRegistry.h"
#include "Minty/Systems/M_SystemRegistry.h"

#include "Minty/Files/M_File.h"
#include "Minty/Tools/M_Console.h"
#include "Minty/Scripting/M_ScriptObject.h"
#include "Minty/Scripting/M_ScriptArguments.h"

#include "Minty/Types/M_Vector.h"
#include "Minty/Types/M_Quaternion.h"
#include "Minty/Types/M_Matrix.h"

#include "Minty/Components/M_TransformComponent.h"
#include "Minty/Components/M_EnabledComponent.h"
#include "Minty/Components/M_RelationshipComponent.h"
#include "Minty/Rendering/M_CameraComponent.h"

#include "Minty/Rendering/M_RenderSystem.h"

#include "Minty/Scripting/M_ScriptLink.h"
#include "Minty/Scripting/M_ScriptObject.h"

using namespace Minty;
using namespace Minty::Scripting;

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

ScriptEngine* Minty::ScriptEngine::_instance = nullptr;

Minty::ScriptEngine::ScriptEngine()
	: Engine()
	, _rootDomain()
	, _appDomain()
	, _assemblies()
{
	MINTY_ASSERT_MESSAGE(!_instance, "There should only be one ScriptEngine.");

	_instance = this;

	Path currentPath = std::filesystem::current_path();
	String currentPathString = currentPath.generic_string();

	//mono_set_dirs("C:/Program Files/Mono/lib", "C:/Program Files/Mono/etc");
	mono_set_dirs(currentPathString.c_str(), nullptr);

	_rootDomain = mono_jit_init("MintyRuntime");
	MINTY_ASSERT(_rootDomain != nullptr);

	//mono_set_assemblies_path("C:/Libraries/Mono/lib");
	mono_set_assemblies_path(currentPathString.c_str());

	String appDomainName = "MintyDomain";
	_appDomain = mono_domain_create_appdomain(appDomainName.data(), nullptr);
	mono_domain_set(_appDomain, true);
}

Minty::ScriptEngine::~ScriptEngine()
{
	// mono_gc_collect(mono_gc_max_generation());

	// unload all assemblies
	for (auto& pair : _assemblies)
	{
		delete pair.second;
	}
	_assemblies.clear();

	// dispose of Mono
	mono_domain_unload(_appDomain);
	// TODO: mono_jit_cleanup
	//mono_jit_cleanup(_rootDomain);
	_rootDomain = nullptr;
	_appDomain = nullptr;

	_instance = nullptr;
}

bool Minty::ScriptEngine::load_assembly(Path const& path, bool const referenceOnly)
{
	// if not found at path, do nothing
	if (!std::filesystem::exists(path))
	{
		MINTY_ERROR_FORMAT("Assembly not found at \"{}\".", path.string());
		return false;
	}

	String name = path.stem().string();

	// delete old assembly if there is one
	unload_assembly(name);

	// load the new assembly
	ScriptAssembly* scriptAssembly = _assemblies.emplace(name, new ScriptAssembly(path, referenceOnly)).first->second;

	// if the Script script has been loaded, then load all scripts derive from it
	ScriptClass const* scriptScriptClass = find_class(ASSEMBLY_ENGINE_NAME, "Script");

	if (scriptScriptClass)
	{
		// register all Scripts with the EntityRegistry
		for (auto const script : scriptAssembly->get_classes(scriptScriptClass))
		{
			if (script->is_derived_from(*scriptScriptClass) && script != scriptScriptClass)
			{
				ScriptLink::register_script(script->get_name());
			}
		}
	}

	return true;
}

ScriptAssembly const* Minty::ScriptEngine::find_assembly(String const& namespaceName, String const& className) const
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

void Minty::ScriptEngine::reload_assembly(String const& name)
{
	MINTY_TODO("reload assembly");
}

void Minty::ScriptEngine::unload_assembly(String const& name)
{
	// delete assembly if there is one
	auto found = _assemblies.find(name);
	if (found != _assemblies.end())
	{
		delete found->second;
		_assemblies.erase(name);
	}
}

ScriptAssembly const* Minty::ScriptEngine::get_assembly(String const& assemblyName) const
{
	auto found = _assemblies.find(assemblyName);

	if (found != _assemblies.end())
	{
		return found->second;
	}

	return nullptr;
}

MonoString* Minty::ScriptEngine::to_mono_string(String const& string) const
{
	return mono_string_new(_appDomain, string.c_str());
}

String Minty::ScriptEngine::from_mono_string(MonoString* const string)
{
	char* str = mono_string_to_utf8(string);
	String result(str);
	mono_free(str);
	return result;
}

String Minty::ScriptEngine::get_class_name(MonoClass* const klass) const
{
	return mono_class_get_name(klass);
}

String Minty::ScriptEngine::get_class_namespace(MonoClass* const klass) const
{
	return mono_class_get_namespace(klass);
}

bool Minty::ScriptEngine::check_class_inheritance(MonoClass* const klass, MonoClass* const baseKlass) const
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

MonoMethod* Minty::ScriptEngine::get_method(MonoClass* const klass, String const& methodName, int const paramCount) const
{
	return mono_class_get_method_from_name(klass, methodName.c_str(), paramCount);
}

std::vector<MonoMethod*> Minty::ScriptEngine::get_methods(MonoClass* const klass) const
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

Accessibility Minty::ScriptEngine::get_method_accessibility(MonoMethod* const method) const
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

std::vector<MonoMethod*> Minty::ScriptEngine::get_methods(MonoClass* const klass, Accessibility const accessibility) const
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

String Minty::ScriptEngine::get_method_name(MonoMethod* const method) const
{
	return mono_method_get_name(method);
}

MonoClassField* Minty::ScriptEngine::get_field(MonoClass* const klass, String const& fieldName) const
{
	MonoClassField* field = mono_class_get_field_from_name(klass, fieldName.c_str());

	if (!field)
	{
		MINTY_ERROR(std::format("Assembly::get_field(): Field \"{}\" does not exist.", fieldName));
		return nullptr;
	}

	return field;
}

Accessibility Minty::ScriptEngine::get_field_accessibility(MonoClassField* const field) const
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

std::vector<MonoClassField*> Minty::ScriptEngine::get_fields(MonoClass* const klass) const
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

std::vector<MonoClassField*> Minty::ScriptEngine::get_fields(MonoClass* const klass, Accessibility const accessibility) const
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

String Minty::ScriptEngine::get_field_name(MonoClassField* const field) const
{
	return mono_field_get_name(field);
}

MonoType* Minty::ScriptEngine::get_field_type(MonoClassField* const field) const
{
	return mono_field_get_type(field);
}

void Minty::ScriptEngine::get_field_value(MonoObject* const object, MonoClassField* const field, void* out) const
{
	mono_field_get_value(object, field, out);
}

void Minty::ScriptEngine::set_field_value(MonoObject* const object, MonoClassField* const field, void* in) const
{
	mono_field_set_value(object, field, in);
}

MonoProperty* Minty::ScriptEngine::get_property(MonoClass* const klass, String const& propertyName) const
{
	MonoProperty* prop = mono_class_get_property_from_name(klass, propertyName.c_str());

	if (!prop)
	{
		MINTY_ERROR(std::format("Assembly::get_field(): Field \"{}\" does not exist.", propertyName));
		return nullptr;
	}

	return prop;
}

void Minty::ScriptEngine::get_property_value(MonoObject* const object, MonoProperty* const prop, void* out) const
{
	MonoObject* exception = nullptr;

	mono_property_get_value(prop, object, &out, &exception);

	if (exception)
	{
		MINTY_ABORT(get_exception_message(exception));
	}
}

void Minty::ScriptEngine::set_property_value(MonoObject* const object, MonoProperty* const prop, void* in) const
{
	MonoObject* exception = nullptr;

	mono_property_set_value(prop, object, &in, &exception);

	if (exception)
	{
		MINTY_ABORT(get_exception_message(exception));
	}
}

void Minty::ScriptEngine::invoke_method(MonoObject* const object, MonoMethod* const method) const
{
	MonoObject* exception = nullptr;
	mono_runtime_invoke(method, object, nullptr, &exception);

	if (exception)
	{
		//MINTY_ABORT(get_exception_message(exception));
		MINTY_ERROR(get_exception_message(exception));
	}
}

void Minty::ScriptEngine::invoke_method(MonoObject* const object, MonoMethod* const method, std::vector<void*>& arguments) const
{
	MonoObject* exception = nullptr;
	mono_runtime_invoke(method, object, arguments.data(), &exception);

	if (exception)
	{
		MINTY_ABORT(get_exception_message(exception));
	}
}

String Minty::ScriptEngine::get_exception_message(MonoObject* const exception) const
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

	String out = "";

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

MonoObject* Minty::ScriptEngine::create_instance(MonoClass* const klass) const
{
	// create the instance
	MonoObject* instance = mono_object_new(_appDomain, klass);

	MINTY_ASSERT(instance != nullptr);

	// call the constructor
	mono_runtime_object_init(instance);

	return instance;
}

MonoObject* Minty::ScriptEngine::create_instance(MonoClass* const klass, std::vector<void*>& values) const
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

void* Minty::ScriptEngine::unbox(MonoObject* const object) const
{
	return mono_object_unbox(object);
}

String Minty::ScriptEngine::get_full_name(String const& namespaceName, String const& className)
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

ScriptClass const* Minty::ScriptEngine::find_class(String const& namespaceName, String const& className) const
{
	return find_class(get_full_name(namespaceName, className));
}

ScriptClass const* Minty::ScriptEngine::find_class(String const& fullName) const
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

ScriptClass const* Minty::ScriptEngine::search_for_class(String const& name) const
{
	for (auto const& pair : _assemblies)
	{
		if (ScriptClass const* scriptClass = pair.second->search_for_class(name))
		{
			return scriptClass;
		}
	}

	return nullptr;
}

ScriptObject& Minty::ScriptEngine::create_object(ScriptClass const& script, UUID id)
{
	return _idToObject.emplace(id, ScriptObject(id, script)).first->second;
}

ScriptObject& Minty::ScriptEngine::create_object(ScriptClass const& script, UUID id, ScriptArguments& scriptArguments)
{
	return _idToObject.emplace(id, ScriptObject(id, script, scriptArguments)).first->second;
}

ScriptObject* Minty::ScriptEngine::get_object(UUID id)
{
	auto found = _idToObject.find(id);

	if (found != _idToObject.end())
	{
		return &found->second;
	}

	return nullptr;
}

ScriptObject& Minty::ScriptEngine::get_or_create_object(UUID id, ScriptClass const& script)
{
	ScriptObject* object = get_object(id);

	if (object) return *object;

	return create_object(script, id);
}

ScriptObject& Minty::ScriptEngine::get_or_create_object(UUID id, ScriptClass const& script, ScriptArguments& scriptArguments)
{
	ScriptObject* object = get_object(id);

	if (object) return *object;

	return create_object(script, id, scriptArguments);
}

ScriptObject& Minty::ScriptEngine::get_or_create_entity(UUID id)
{
	// try get
	ScriptObject* entityObject = get_object(id);

	// if found, return this
	if (entityObject) return *entityObject;

	// create new
	return create_object_entity(id);
}

ScriptObject& Minty::ScriptEngine::get_or_create_component(UUID id, UUID const entityId, ScriptClass const& script)
{
	// try get
	ScriptObject* compObject = get_object(id);

	if (compObject) return *compObject;

	// new
	return create_object_component(id, entityId, script);
}

ScriptObject& Minty::ScriptEngine::create_object_entity(UUID id)
{
	// get the entity class
	ScriptAssembly const* engineAssembly = get_assembly(ASSEMBLY_ENGINE_NAME);
	MINTY_ASSERT(engineAssembly != nullptr);

	ScriptClass const* entityClass = engineAssembly->get_class(ASSEMBLY_ENGINE_NAME, "Entity");
	MINTY_ASSERT(entityClass != nullptr);

	// create an instance of it
	uint64_t rawId = static_cast<uint64_t>(id);
	ScriptArguments args({ &rawId });
	ScriptObject& object = create_object(*entityClass, id, args);

	return object;
}

ScriptObject& Minty::ScriptEngine::create_object_component(UUID id, UUID const entityId, ScriptClass const& script)
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
	MonoObject* entityObject = entity->data();
	MINTY_ASSERT(entityObject != nullptr);

	// spawn the object and save the entity to it
	ScriptObject& object = create_object(script, id);
	object.set_property("Entity", entityObject);

	return object;
}

void Minty::ScriptEngine::destroy_object(UUID id)
{
	// find the object and destroy if found
	auto found = _idToObject.find(id);

	if (found != _idToObject.end())
	{
		found->second.destroy();
		_idToObject.erase(id);
	}
}

void Minty::ScriptEngine::register_script_id(UUID const id, String const& name)
{
	_idToName[id] = name;
	_nameToId[name] = id;
}

String Minty::ScriptEngine::get_name_from_script_id(UUID const id) const
{
	auto found = _idToName.find(id);

	if (found == _idToName.end())
	{
		return "";
	}

	return found->second;
}

UUID Minty::ScriptEngine::get_id_from_script_name(String const& name) const
{
	auto found = _nameToId.find(name);

	if (found == _nameToId.end())
	{
		return INVALID_UUID;
	}

	return found->second;
}