#pragma once

#include "Minty/Core/Path.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/String.h"
#include "Minty/Core/Type.h"
#include "Minty/Script/ScriptAssembly.h"
#include "Minty/Script/ScriptObject.h"

namespace Minty
{
	struct ScriptEngineBuilder
	{

	};

	class ScriptEngine
	{
	private:
		static std::unordered_map<String, Owner<ScriptAssembly>> s_assemblies;

		static std::unordered_map<UUID, Owner<ScriptObject>> s_objects;

	private:
		ScriptEngine() = default;
		~ScriptEngine() = default;

	public:
		static void initialize(ScriptEngineBuilder const& builder);

		static void shutdown();

#pragma region Assembly

	public:
		static Ref<ScriptAssembly> load_assembly(String const& name, Path const& path, Bool const referenceOnly = false);

		static void unload_assembly(String const& name);

		static void reload_assembly(String const& name);

		static Ref<ScriptAssembly> get_assembly(String const& name);

#pragma endregion

#pragma region Class

	public:
		static Ref<ScriptClass> find_class(String const& fullName);

		static Ref<ScriptClass> find_class(UUID const id);

#pragma endregion
		
#pragma region Object

		static Ref<ScriptObject> create_object(UUID const id, Ref<ScriptClass> const scriptClass);

		static Ref<ScriptObject> create_object(UUID const id, Ref<ScriptClass> const scriptClass, ScriptArguments& args);

		static Ref<ScriptObject> get_object(UUID const id);

		static Ref<ScriptObject> get_or_create_object(UUID const id, Ref<ScriptClass> const scriptClass);

		static Ref<ScriptObject> get_or_create_object(UUID const id, Ref<ScriptClass> const scriptClass, ScriptArguments& scriptArguments);

		static Ref<ScriptObject> create_object_entity(UUID const id);

		static Ref<ScriptObject> get_or_create_object_entity(UUID const id);

		static Ref<ScriptObject> create_object_component(Ref<ScriptObject> const entityObject, Ref<ScriptClass> const scriptClass);

		static Ref<ScriptObject> get_or_create_object_component(UUID const id, Ref<ScriptObject> const entityObject, Ref<ScriptClass> const scriptClass);

		static Bool contains_object(UUID const id);

		static void destroy_object(UUID const id);

#pragma endregion

#pragma region Utility

	public:
		static String get_full_name(String const& namespaceName, String const& className);

#pragma endregion

	};
}