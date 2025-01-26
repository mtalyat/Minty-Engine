#include "pch.h"
#include "ScriptEngine.h"

#include "CS/CsScriptEngine.h"
#include "CS/CsScriptAssembly.h"

#include "Minty/Asset/AssetManager.h"

using namespace Minty;

std::unordered_map<String, Owner<ScriptAssembly>> ScriptEngine::s_assemblies = {};
std::unordered_map<UUID, Owner<ScriptObject>> ScriptEngine::s_objects = {};

void Minty::ScriptEngine::initialize(ScriptEngineBuilder const& builder)
{
	// initialize all script engines
	CsScriptEngine::initialize(builder);
}

void Minty::ScriptEngine::shutdown()
{
	// destroy all scripting references and objects
	s_objects.clear();
	std::vector<String> assemblyNames;
	assemblyNames.reserve(s_assemblies.size());
	for (auto const& [name, assm] : s_assemblies)
	{
		assemblyNames.push_back(name);
	}
	for (auto const& name : assemblyNames)
	{
		unload_assembly(name);
	}
	s_assemblies.clear();

	// shutdown all script engines
	CsScriptEngine::shutdown();
}

Ref<ScriptAssembly> Minty::ScriptEngine::load_assembly(String const& name, Path const& path, Bool const referenceOnly)
{
	MINTY_ASSERT_FORMAT(!s_assemblies.contains(name), "Already loaded a ScriptAssembly with the name \"{}\".", name);
	MINTY_ASSERT_FORMAT(std::filesystem::exists(path), "The given path to load the assembly does not exist: \"{}\"", std::filesystem::absolute(path).generic_string());

	// load from disk
	ScriptAssemblyBuilder builder{};
	builder.path = path;
	builder.referenceOnly = referenceOnly;
	Owner<ScriptAssembly> scriptAssembly = ScriptAssembly::create(builder);
	Ref<ScriptAssembly> scriptAssemblyRef = scriptAssembly.create_ref();

	// add to references
	s_assemblies.emplace(name, std::move(scriptAssembly));

	Debug::log_info(std::format("Loaded assembly \"{}\" from \"{}\"{}.", name, path.generic_string(), referenceOnly ? " (reference only)" : ""));

	return scriptAssemblyRef;
}

void Minty::ScriptEngine::unload_assembly(String const& name)
{
	// remove from references
	s_assemblies.erase(name);
}

void Minty::ScriptEngine::reload_assembly(String const& name)
{
	MINTY_ASSERT_FORMAT(s_assemblies.contains(name), "No assembly with the name \"{}\" is currently loaded.", name);

	// get meta data so it can be re-loaded
	Ref<ScriptAssembly> assembly = s_assemblies.at(name);
	Bool referenceOnly = assembly->is_reference_only();
	Path path = assembly->get_path();

	unload_assembly(name);
	load_assembly(name, path, referenceOnly);
}

Ref<ScriptAssembly> Minty::ScriptEngine::get_assembly(String const& name)
{
	auto found = s_assemblies.find(name);

	if (found == s_assemblies.end())
	{
		return Ref<ScriptAssembly>();
	}

	return found->second;
}

Ref<ScriptClass> Minty::ScriptEngine::find_class(String const& fullName)
{
	// search each assembly
	Ref<ScriptClass> klass;
	for (auto const& [name, assembly] : s_assemblies)
	{
		klass = assembly->get_class(fullName);
		
		// if found, return it
		if (klass.get())
		{
			return klass;
		}
	}

	// none found
	return Ref<ScriptClass>();
}

Ref<ScriptClass> Minty::ScriptEngine::find_class(UUID const id)
{
	// search each assembly
	Ref<ScriptClass> klass;
	for (auto const& [name, assembly] : s_assemblies)
	{
		klass = assembly->get_class(id);

		// if found, return it
		if (klass.get())
		{
			return klass;
		}
	}

	// none found
	return Ref<ScriptClass>();
}

Ref<ScriptObject> Minty::ScriptEngine::create_object(UUID const id, Ref<ScriptClass> const scriptClass)
{
	MINTY_ASSERT_MESSAGE(id.valid(), "Cannot create a ScriptObject with an invalid ID.");

	MINTY_ASSERT_FORMAT(!s_objects.contains(id), "ScriptObject already created with ID: {}.", to_string(id));

	ScriptObjectBuilder builder{};
	builder.id = id;
	builder.klass = scriptClass;

	Owner<ScriptObject> object = ScriptObject::create(builder);
	Ref<ScriptObject> ref = object.create_ref();
	s_objects.emplace(id, std::move(object));
	return ref;
}

Ref<ScriptObject> Minty::ScriptEngine::create_object(UUID const id, Ref<ScriptClass> const scriptClass, ScriptArguments& args)
{
	MINTY_ASSERT_MESSAGE(id.valid(), "Cannot create a ScriptObject with an invalid ID.");

	MINTY_ASSERT(!s_objects.contains(id));

	ScriptObjectBuilder builder{};
	builder.id = id;
	builder.klass = scriptClass;

	Owner<ScriptObject> object = ScriptObject::create(builder, args);
	Ref<ScriptObject> ref = object.create_ref();
	s_objects.emplace(id, std::move(object));
	return ref;
}

Ref<ScriptObject> Minty::ScriptEngine::get_object(UUID const id)
{
	auto found = s_objects.find(id);

	if (found == s_objects.end())
	{
		return Ref<ScriptObject>();
	}

	return found->second.create_ref();
}

UUID Minty::ScriptEngine::get_uuid(void* const nativeObject)
{
	return CsScriptEngine::get_uuid(static_cast<MonoObject* const>(nativeObject));
}

UUID Minty::ScriptEngine::get_entity_uuid(void* const nativeObject)
{
	return CsScriptEngine::get_entity_uuid(static_cast<MonoObject* const>(nativeObject));
}

Ref<ScriptObject> Minty::ScriptEngine::get_or_create_object(UUID const id, Ref<ScriptClass> const scriptClass)
{
	auto found = s_objects.find(id);

	if (found == s_objects.end())
	{
		return create_object(id, scriptClass);
	}

	return found->second.create_ref();
}

Ref<ScriptObject> Minty::ScriptEngine::get_or_create_object(UUID const id, Ref<ScriptClass> const scriptClass, ScriptArguments& scriptArguments)
{
	auto found = s_objects.find(id);

	if (found == s_objects.end())
	{
		return create_object(id, scriptClass, scriptArguments);
	}

	return found->second.create_ref();
}

Ref<ScriptObject> Minty::ScriptEngine::create_object_entity(UUID const id)
{
	// get engine assembly
	Ref<ScriptAssembly> engineAssembly = get_assembly(MINTY_NAME_ENGINE);
	MINTY_ASSERT_MESSAGE(engineAssembly != nullptr, "Cannot create an Entity ScriptObject when the MintyEngine assembly has not been loaded.");

	// get Entity class
	Ref<ScriptClass> entityClass = engineAssembly->get_class(get_full_name(MINTY_NAME_ENGINE, "Entity"));
	MINTY_ASSERT(entityClass != nullptr);

	// create instance of it
	uint64_t rawId = id.data();
	ScriptArguments args({ &rawId });
	return create_object(id, entityClass, args);
}

Ref<ScriptObject> Minty::ScriptEngine::get_or_create_object_entity(UUID const id)
{
	auto found = s_objects.find(id);

	if (found == s_objects.end())
	{
		return create_object_entity(id);
	}

	return found->second;
}

Ref<ScriptObject> Minty::ScriptEngine::create_object_component(UUID const entityId, Ref<ScriptClass> const scriptClass)
{
	// check if entity valid
	MINTY_ASSERT_MESSAGE(entityId.valid(), "Cannot create a Component ScriptObject when the given Entity ID is invalid.");
	// check if script valid
	MINTY_ASSERT_MESSAGE(scriptClass != nullptr, "Cannot create a Component ScriptObject when the given class ScriptObject is null.");

	// get engine assembly
	Ref<ScriptAssembly> engineAssembly = get_assembly(MINTY_NAME_ENGINE);
	MINTY_ASSERT_MESSAGE(engineAssembly != nullptr, "Cannot create a Component ScriptObject when the Minty engine assembly has not been loaded.");

	// get Component class
	Ref<ScriptClass> componentClass = engineAssembly->get_class(get_full_name(MINTY_NAME_ENGINE, "Component"));
	MINTY_ASSERT(componentClass != nullptr);

	// check if given class is derived from component
	MINTY_ASSERT_FORMAT(scriptClass->is_derived_from(componentClass), "Cannot create a Component ScriptObject when \"{}\" does not derive from \"{}\".", scriptClass->get_full_name(), componentClass->get_full_name());

	UUID componentId = UUID::create();
	ScriptArguments args({ &componentId });
	Ref<ScriptObject> object = create_object(componentId, scriptClass, args);

	// set entity property
	Ref<ScriptObject> entityObject = get_or_create_object_entity(entityId);
	void* entityObjectObject = entityObject->get_native();
	object->set_field("_entity", entityObjectObject);

	return object;
}

Ref<ScriptObject> Minty::ScriptEngine::get_or_create_object_component(UUID const id, UUID const entityId, Ref<ScriptClass> const scriptClass)
{
	auto found = s_objects.find(id);

	if (found == s_objects.end())
	{
		return create_object_component(entityId, scriptClass);
	}

	return found->second;
}

Ref<ScriptObject> Minty::ScriptEngine::create_object_asset(UUID const id, Ref<ScriptClass> const& scriptClass)
{
	MINTY_ASSERT_MESSAGE(id.valid(), "Cannot create an Asset ScriptObject with an invalid ID.");
	MINTY_ASSERT_MESSAGE(scriptClass != nullptr, "Cannot create an Asset ScriptObject with a null ScriptClass.");

	// get engine assembly
	Ref<ScriptAssembly> engineAssembly = get_assembly(MINTY_NAME_ENGINE);
	MINTY_ASSERT_MESSAGE(engineAssembly != nullptr, "Cannot create an Asset ScriptObject when the Minty engine assembly has not been loaded.");

	// get Asset class
	Ref<ScriptClass> assetClass = engineAssembly->get_class(get_full_name(MINTY_NAME_ENGINE, "Asset"));
	MINTY_ASSERT(assetClass != nullptr);

	// check if given class is derived from component
	MINTY_ASSERT_FORMAT(scriptClass->is_derived_from(assetClass), "Cannot create an Asset ScriptObject when \"{}\" does not derive from \"{}\".", scriptClass->get_full_name(), assetClass->get_full_name());

	UUID assetId = id;
	ScriptArguments args({ &assetId });
	Ref<ScriptObject> object = create_object(id, scriptClass, args);

	return object;
}

Ref<ScriptObject> Minty::ScriptEngine::get_or_create_object_asset(UUID const id, Ref<ScriptClass> const& scriptClass)
{
	auto found = s_objects.find(id);

	if (found == s_objects.end())
	{
		return create_object_asset(id, scriptClass);
	}

	return found->second;
}

Bool Minty::ScriptEngine::contains_object(UUID const id)
{
	return s_objects.contains(id);
}

void Minty::ScriptEngine::destroy_object(UUID const id)
{
	// ScriptEngine owns the object, so it should deconstruct after erasure
	s_objects.erase(id);
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
