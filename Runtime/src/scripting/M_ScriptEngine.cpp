#include "pch.h"
#include "scripting/M_ScriptEngine.h"

#include "libraries/M_Mono.h"

#include "rendering/M_Window.h"
#include "runtime/M_Runtime.h"
#include "scenes/M_Scene.h"
#include "scenes/M_SceneManager.h"
#include "entities/M_EntityRegistry.h"
#include "systems/M_SystemRegistry.h"

#include "files/M_File.h"
#include "tools/M_Console.h"
#include "scripting/M_ScriptObject.h"
#include "scripting/M_ScriptArguments.h"

#include "types/M_Vector.h"
#include "types/M_Quaternion.h"
#include "types/M_Matrix.h"

#include "components/M_TransformComponent.h"
#include "components/M_EnabledComponent.h"
#include "components/M_RelationshipComponent.h"
#include "rendering/M_CameraComponent.h"

#include "rendering/M_RenderSystem.h"

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
	std::unordered_map<MonoType*, ScriptClass const*> types;

	Scene* get_scene()
	{
		return engine->get_working_scene();
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

minty::ScriptEngine::ScriptEngine(Runtime& runtime)
	: Engine(runtime)
	, _rootDomain()
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
	// dispose of all C# objects
	_data.reset();
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
}

bool minty::ScriptEngine::load_assembly(Path const& path, bool const referenceOnly)
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
	ScriptAssembly* scriptAssembly = _assemblies.emplace(name, new ScriptAssembly(path, *this, referenceOnly)).first->second;

	// if the Script script has been loaded, then load all scripts derive from it
	ScriptClass const* scriptScriptClass = find_class(ASSEMBLY_ENGINE_NAME, "Script");

	if (scriptScriptClass)
	{
		// register all Scripts with the EntityRegistry
		for (auto const script : scriptAssembly->get_classes(scriptScriptClass))
		{
			if (script->is_derived_from(*scriptScriptClass) && script != scriptScriptClass)
			{
				Runtime::register_script(script->get_name());
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
	MINTY_TODO("reload assembly");
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

String minty::ScriptEngine::from_mono_string(MonoString* const string)
{
	char* str = mono_string_to_utf8(string);
	String result(str);
	mono_free(str);
	return result;
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
		MINTY_ERROR(std::format("Assembly::get_field(): Field \"{}\" does not exist.", fieldName));
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
		MINTY_ERROR(std::format("Assembly::get_field(): Field \"{}\" does not exist.", propertyName));
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

ScriptClass const* minty::ScriptEngine::search_for_class(String const& name) const
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

ScriptObject& minty::ScriptEngine::create_object(ScriptClass const& script, UUID id)
{
	return _data.objects.emplace(id, ScriptObject(id, script)).first->second;
}

ScriptObject& minty::ScriptEngine::create_object(ScriptClass const& script, UUID id, ScriptArguments& scriptArguments)
{
	return _data.objects.emplace(id, ScriptObject(id, script, scriptArguments)).first->second;
}

ScriptObject* minty::ScriptEngine::get_object(UUID id) const
{
	auto found = _data.objects.find(id);

	if (found != _data.objects.end())
	{
		return &found->second;
	}

	return nullptr;
}

ScriptObject& minty::ScriptEngine::get_or_create_object(UUID id, ScriptClass const& script)
{
	ScriptObject* object = get_object(id);

	if (object) return *object;

	return create_object(script, id);
}

ScriptObject& minty::ScriptEngine::get_or_create_object(UUID id, ScriptClass const& script, ScriptArguments& scriptArguments)
{
	ScriptObject* object = get_object(id);

	if (object) return *object;

	return create_object(script, id, scriptArguments);
}

ScriptObject& minty::ScriptEngine::get_or_create_entity(UUID id)
{
	// try get
	ScriptObject* entityObject = get_object(id);

	// if found, return this
	if (entityObject) return *entityObject;

	// create new
	return create_object_entity(id);
}

ScriptObject& minty::ScriptEngine::get_or_create_component(UUID id, UUID const entityId, ScriptClass const& script)
{
	// try get
	ScriptObject* compObject = get_object(id);

	if (compObject) return *compObject;

	// new
	return create_object_component(id, entityId, script);
}

ScriptObject& minty::ScriptEngine::create_object_entity(UUID id)
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

ScriptObject& minty::ScriptEngine::create_object_component(UUID id, UUID const entityId, ScriptClass const& script)
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

void minty::ScriptEngine::link_script(String const& fullName)
{
	ScriptEngine* engine = _data.engine;

	MINTY_ASSERT(engine != nullptr);

	// find ScriptClass with the given name
	ScriptClass const* script = engine->find_class(fullName);

	// if found, register it
	if (!script) return;

	link_script(*script);
}

void minty::ScriptEngine::link_script(ScriptClass const& script)
{
	MonoType* type = script.get_type();

	MINTY_ASSERT(type != nullptr);

	_data.types[type] = &script;

	MINTY_INFO_FORMAT("Linked {}.", script.get_full_name());
}

void minty::ScriptEngine::register_script_id(UUID const id, String const& name)
{
	_idToName[id] = name;
	_nameToId[name] = id;
}

String minty::ScriptEngine::get_name_from_script_id(UUID const id) const
{
	auto found = _idToName.find(id);

	if (found == _idToName.end())
	{
		return "";
	}

	return found->second;
}

UUID minty::ScriptEngine::get_id_from_script_name(String const& name) const
{
	auto found = _nameToId.find(name);

	if (found == _nameToId.end())
	{
		return INVALID_UUID;
	}

	return found->second;
}

//
//		LINKING
//

constexpr static char const* INTERNAL_CLASS_NAME = "Runtime";

#define ADD_INTERNAL_CALL(csharpName, cppName) mono_add_internal_call(std::format("{}.{}::{}", ASSEMBLY_ENGINE_NAME, INTERNAL_CLASS_NAME, csharpName).c_str(), cppName)

static EntityRegistry& util_get_entity_registry()
{
	MINTY_ASSERT(_data.engine);

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	return scene->get_entity_registry();
}

static Window& util_get_window()
{
	MINTY_ASSERT(_data.engine != nullptr);
	return _data.engine->get_runtime().get_window();
}

#pragma region Time

static float time_get_total()
{
	MINTY_ASSERT(_data.engine);

	Runtime& runtime = _data.engine->get_runtime();

	return runtime.get_time().total;
}

static float time_get_elapsed()
{
	MINTY_ASSERT(_data.engine);

	Runtime& runtime = _data.engine->get_runtime();

	return runtime.get_time().elapsed;
}

#pragma endregion

#pragma region Console

static void console_log(MonoString* string)
{
	Console::log(ScriptEngine::from_mono_string(string));
}

static void console_log_color(MonoString* string, int color)
{
	Console::log_color(ScriptEngine::from_mono_string(string), static_cast<Console::Color>(color));
}

static void console_warn(MonoString* string)
{
	Console::warn(ScriptEngine::from_mono_string(string));
}

static void console_error(MonoString* string)
{
	Console::error(ScriptEngine::from_mono_string(string));
}

static void console_ass(bool condition, MonoString* string)
{	
	Console::ass(condition, ScriptEngine::from_mono_string(string));
}

#pragma endregion

#pragma region Cursor

static CursorMode cursor_get_mode()
{
	return util_get_window().get_cursor_mode();
}

static void cursor_set_mode(CursorMode mode)
{
	util_get_window().set_cursor_mode(mode);
}

#pragma endregion

#pragma region Object

static void object_destroy_entity(UUID id)
{
	MINTY_ASSERT(_data.engine);

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene);

	EntityRegistry& registry = scene->get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	registry.destroy(entity);
}

static void object_destroy_immediate_entity(UUID id)
{
	MINTY_ASSERT(_data.engine);

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene);

	EntityRegistry& registry = scene->get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	registry.destroy_immediate(entity);
}

#pragma endregion

#pragma region Entity

static Entity util_get_entity(UUID id)
{
	MINTY_ASSERT(_data.engine);
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	return entity;
}

static MonoString* entity_get_name(UUID id)
{
	if (!id) return _data.engine->to_mono_string("");

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the name
	String name = registry.get_name(entity);

	// convert to mono string and return
	return _data.engine->to_mono_string(name);
}

static void entity_set_name(UUID id, MonoString* string)
{
	if (!id) return;

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// set the name
	String name = ScriptEngine::from_mono_string(string);
	registry.set_name(entity, name);
}

static MonoString* entity_get_tag(UUID id)
{
	if (!id) return _data.engine->to_mono_string("");

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the name
	String name = registry.get_tag(entity);

	// convert to mono string and return
	return _data.engine->to_mono_string(name);
}

static void entity_set_tag(UUID id, MonoString* string)
{
	if (!id) return;

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// set the name
	String name = ScriptEngine::from_mono_string(string);
	registry.set_tag(entity, name);
}

static void entity_set_enabled(UUID id, bool enabled)
{
	MINTY_ASSERT(_data.engine);
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	if (enabled)
	{
		registry.enable(entity);
	}
	else
	{
		registry.disable(entity);
	}
}

static bool entity_get_enabled(UUID id)
{
	MINTY_ASSERT(_data.engine);
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	return registry.all_of<EnabledComponent>(entity);
}

static MonoObject* entity_add_component(UUID id, MonoReflectionType* reflectionType)
{
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the component name
	MonoType* type = mono_reflection_type_get_type(reflectionType);
	auto found = _data.types.find(type);
	if (found == _data.types.end())
	{
		MINTY_ABORT(std::format("{} cannot be added, it has not been registered.", mono_type_get_name(type)));
		return nullptr;
	}
	ScriptClass const* scriptClass = found->second;

	// get the component by name
	Component* component = registry.get_by_name(scriptClass->get_name(), entity);

	// if null, add the component
	if (!component)
	{
		component = registry.emplace_by_name(scriptClass->get_name(), entity);
	}

	// create a new object for this component, if needed
	ScriptObject const& componentObject = _data.engine->get_or_create_component(component->id, id, *scriptClass);
	return componentObject.data();
}

static MonoObject* entity_get_component(UUID id, MonoReflectionType* reflectionType)
{
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the component name
	MonoType* type = mono_reflection_type_get_type(reflectionType);
	auto found = _data.types.find(type);
	if (found == _data.types.end())
	{
		MINTY_ABORT(std::format("{} cannot be gotten, it has not been registered.", mono_type_get_name(type)));
		// type not found
		return nullptr;
	}
	ScriptClass const* scriptClass = found->second;

	// get the component by name
	Component* component = registry.get_by_name(scriptClass->get_name(), entity);

	// if null, return null
	if (!component)
	{
		return nullptr;
	}
	
	// create a new object for this component
	ScriptObject const& componentObject = _data.engine->get_or_create_component(component->id, id, *scriptClass);
	return componentObject.data();
}

static void entity_remove_component(UUID id, MonoReflectionType* reflectionType)
{
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the component name
	MonoType* type = mono_reflection_type_get_type(reflectionType);
	auto found = _data.types.find(type);
	if (found == _data.types.end())
	{
		MINTY_ABORT(std::format("{} cannot be removed, it has not been registered.", mono_type_get_name(type)));
		// type not found
		return;
	}
	ScriptClass const* scriptClass = found->second;

	// destroy the component by name
	registry.destroy(entity, scriptClass->get_name());
}

static MonoObject* entity_get_parent(UUID id)
{
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the parent
	Entity parent = registry.get_parent(entity);

	// get the ID
	UUID parentId = registry.get_id(parent);

	if (!parentId.valid()) return nullptr;

	MINTY_ASSERT(_data.engine != nullptr);
	return _data.engine->get_or_create_entity(parentId).data();
}

static void entity_set_parent(UUID id, UUID parentId)
{
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the parent
	Entity parentEntity = registry.find_by_id(parentId);

	// set the parent
	registry.set_parent(entity, parentEntity);
}

static int entity_get_child_count(UUID id)
{
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	size_t childCount = registry.get_child_count(entity);

	MINTY_ASSERT_MESSAGE(childCount <= INT_MAX, "Entity child count exceeded INT_MAX.");

	return static_cast<int>(childCount);
}

static MonoObject* entity_get_child(UUID id, int index)
{
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the child
	Entity child = registry.get_child(entity, index);

	UUID childId = registry.get_id(child);

	if (!childId.valid()) return nullptr;

	MINTY_ASSERT(_data.engine != nullptr);
	return _data.engine->get_or_create_entity(childId).data();
}

static MonoObject* entity_clone(UUID id)
{
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// clone it
	Entity clone = registry.clone(entity);

	// get the ID
	UUID cloneId = registry.get_id(clone);

	if (!cloneId.valid()) return nullptr;

	MINTY_ASSERT(_data.engine != nullptr);
	return _data.engine->get_or_create_entity(cloneId).data();
}

#pragma endregion

#pragma region Window

static MonoString* window_get_title()
{
	MINTY_ASSERT(_data.engine);

	return _data.engine->to_mono_string(util_get_window().get_title());
}

static void window_set_title(MonoString* string)
{
	MINTY_ASSERT(_data.engine);

	util_get_window().set_title(_data.engine->from_mono_string(string));
}

static void window_set_icon(MonoString* string)
{
	MINTY_ASSERT(_data.engine);

	util_get_window().set_icon(_data.engine->from_mono_string(string));
}

static bool window_is_open()
{
	return util_get_window().is_open();
}

static void window_close()
{
	util_get_window().close();
}

static void window_maximize()
{
	util_get_window().maximize();
}

static void window_minimize()
{
	util_get_window().minimize();
}

static void window_restore()
{
	util_get_window().restore();
}

#pragma endregion

#pragma region Components

#pragma region Camera

static CameraComponent& util_get_camera_component(UUID id)
{
	MINTY_ASSERT(_data.engine);
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	CameraComponent* component = registry.try_get<CameraComponent>(entity);
	MINTY_ASSERT(component != nullptr);

	return *component;
}

static int camera_get_perspective(UUID id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return static_cast<int>(camera.camera.get_perspective());
}

static void camera_set_perspective(UUID id, int value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_perspective(static_cast<Perspective>(value));
}

static float camera_get_fov(UUID id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return camera.camera.get_fov();
}

static void camera_set_fov(UUID id, float value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_fov(value);
}

static float camera_get_near(UUID id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return camera.camera.get_near();
}

static void camera_set_near(UUID id, float value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_near(value);
}

static float camera_get_far(UUID id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return camera.camera.get_far();
}

static void camera_set_far(UUID id, float value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_far(value);
}

static void camera_set_as_main(UUID id)
{
	MINTY_ASSERT(_data.engine);
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);
	MINTY_ASSERT(registry.all_of<CameraComponent>(entity));

	// set main in render system
	RenderSystem* renderSystem = scene->get_system_registry().find<RenderSystem>();
	MINTY_ASSERT(renderSystem != nullptr);

	renderSystem->set_main_camera(entity);
}

static Color::color_t camera_get_color(UUID id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return static_cast<Color::color_t>(camera.camera.get_color());
}

static void camera_set_color(UUID id, Color::color_t value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_color(Color(value));
}

#pragma endregion

#pragma region Transform

static TransformComponent& util_get_transform_component(UUID id, bool setDirty)
{
	MINTY_ASSERT(_data.engine);
	MINTY_ASSERT(id.valid());

	Scene* scene = _data.get_scene();
	MINTY_ASSERT(scene != nullptr);

	EntityRegistry& registry = scene->get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// if set dirty, do that
	if (setDirty)
	{
		registry.dirty(entity);
	}

	TransformComponent* component = registry.try_get<TransformComponent>(entity);
	MINTY_ASSERT(component != nullptr);

	return *component;
}

static void transform_get_local_position(UUID id, Vector3* position)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*position = component.localPosition;
}

static void transform_set_local_position(UUID id, Vector3* position)
{
	TransformComponent& component = util_get_transform_component(id, true);

	component.localPosition = *position;
}

static void transform_get_local_rotation(UUID id, Vector4* rotation)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*rotation = Vector4(component.localRotation.y, component.localRotation.z, component.localRotation.x, component.localRotation.w);
}

static void transform_set_local_rotation(UUID id, Vector4* rotation)
{
	TransformComponent& component = util_get_transform_component(id, true);

	component.localRotation = Quaternion(rotation->w, rotation->y, rotation->z, rotation->x);
}

static void transform_get_local_scale(UUID id, Vector3* scale)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*scale = component.localScale;
}

static void transform_set_local_scale(UUID id, Vector3* scale)
{
	TransformComponent& component = util_get_transform_component(id, true);

	component.localScale = *scale;
}

static void transform_get_right(UUID id, Vector3* direction)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*direction = component.get_right();
}

static void transform_get_up(UUID id, Vector3* direction)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*direction = component.get_up();
}

static void transform_get_forward(UUID id, Vector3* direction)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*direction = component.get_forward();
}

#pragma endregion

#pragma endregion

void minty::ScriptEngine::link()
{
	// link all the functions
#pragma region Time
	ADD_INTERNAL_CALL("Time_GetTotalTime", time_get_total);
	ADD_INTERNAL_CALL("Time_GetElapsedTime", time_get_elapsed);
#pragma endregion

#pragma region Console
	ADD_INTERNAL_CALL("Console_Log", console_log);
	ADD_INTERNAL_CALL("Console_LogColor", console_log_color);
	ADD_INTERNAL_CALL("Console_Warn", console_warn);
	ADD_INTERNAL_CALL("Console_Error", console_error);
	ADD_INTERNAL_CALL("Console_Assert", console_ass);
#pragma endregion

#pragma region Cursor
	ADD_INTERNAL_CALL("Cursor_GetMode", cursor_get_mode);
	ADD_INTERNAL_CALL("Cursor_SetMode", cursor_set_mode);
#pragma endregion

#pragma region Object
	ADD_INTERNAL_CALL("Object_DestroyEntity", object_destroy_entity);
	ADD_INTERNAL_CALL("Object_DestroyImmediateEntity", object_destroy_immediate_entity);
#pragma endregion

#pragma region Entity
	ADD_INTERNAL_CALL("Entity_GetName", entity_get_name);
	ADD_INTERNAL_CALL("Entity_SetName", entity_set_name);
	ADD_INTERNAL_CALL("Entity_GetTag", entity_get_tag);
	ADD_INTERNAL_CALL("Entity_SetTag", entity_set_tag);
	ADD_INTERNAL_CALL("Entity_GetEnabled", entity_get_enabled);
	ADD_INTERNAL_CALL("Entity_SetEnabled", entity_set_enabled);
	ADD_INTERNAL_CALL("Entity_AddComponent", entity_add_component);
	ADD_INTERNAL_CALL("Entity_GetComponent", entity_get_component);
	ADD_INTERNAL_CALL("Entity_RemoveComponent", entity_remove_component);
	ADD_INTERNAL_CALL("Entity_GetParent", entity_get_parent);
	ADD_INTERNAL_CALL("Entity_SetParent", entity_set_parent);
	ADD_INTERNAL_CALL("Entity_GetChildCount", entity_get_child_count);
	ADD_INTERNAL_CALL("Entity_GetChild", entity_get_child);
	ADD_INTERNAL_CALL("Entity_Clone", entity_clone);
#pragma endregion

#pragma region Window
	ADD_INTERNAL_CALL("Window_GetTitle", window_get_title);
	ADD_INTERNAL_CALL("Window_SetTitle", window_set_title);
	ADD_INTERNAL_CALL("Window_SetIcon", window_set_icon);
	ADD_INTERNAL_CALL("Window_IsOpen", window_is_open);
	ADD_INTERNAL_CALL("Window_Close", window_close);
	ADD_INTERNAL_CALL("Window_Maximize", window_maximize);
	ADD_INTERNAL_CALL("Window_Minimize", window_minimize);
	ADD_INTERNAL_CALL("Window_Restore", window_restore);
#pragma endregion

#pragma region Components

#pragma region Camera
	ADD_INTERNAL_CALL("Camera_GetPerspective", camera_get_perspective);
	ADD_INTERNAL_CALL("Camera_SetPerspective", camera_set_perspective);
	ADD_INTERNAL_CALL("Camera_GetFov", camera_get_fov);
	ADD_INTERNAL_CALL("Camera_SetFov", camera_set_fov);
	ADD_INTERNAL_CALL("Camera_GetNear", camera_get_near);
	ADD_INTERNAL_CALL("Camera_SetNear", camera_set_near);
	ADD_INTERNAL_CALL("Camera_GetFar", camera_get_far);
	ADD_INTERNAL_CALL("Camera_SetFar", camera_set_far);
	ADD_INTERNAL_CALL("Camera_SetAsMain", camera_set_as_main);
	ADD_INTERNAL_CALL("Camera_GetColor", camera_get_color);
	ADD_INTERNAL_CALL("Camera_SetColor", camera_set_color);
#pragma endregion

#pragma region Transform
	ADD_INTERNAL_CALL("Transform_GetLocalPosition", transform_get_local_position);
	ADD_INTERNAL_CALL("Transform_SetLocalPosition", transform_set_local_position);
	ADD_INTERNAL_CALL("Transform_GetLocalRotation", transform_get_local_rotation);
	ADD_INTERNAL_CALL("Transform_SetLocalRotation", transform_set_local_rotation);
	ADD_INTERNAL_CALL("Transform_GetLocalScale", transform_get_local_scale);
	ADD_INTERNAL_CALL("Transform_SetLocalScale", transform_set_local_scale);
	ADD_INTERNAL_CALL("Transform_GetRight", transform_get_right);
	ADD_INTERNAL_CALL("Transform_GetUp", transform_get_up);
	ADD_INTERNAL_CALL("Transform_GetForward", transform_get_forward);
#pragma endregion

#pragma endregion
}

#undef ADD_INTERNAL_CALL