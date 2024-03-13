#pragma once
#include "M_Engine.h"

#include "M_Accessibility.h"
#include "M_ScriptAssembly.h"
#include <unordered_map>
#include <unordered_set>

struct _MonoDomain;
typedef struct _MonoDomain MonoDomain;
struct _MonoAssembly;
typedef struct _MonoAssembly MonoAssembly;
struct _MonoClass;
typedef struct _MonoClass MonoClass;
struct _MonoMethod;
typedef struct _MonoMethod MonoMethod;
struct _MonoClassField;
typedef struct _MonoClassField MonoClassField;
struct _MonoType;
typedef struct _MonoType MonoType;
struct _MonoProperty;
typedef struct _MonoProperty MonoProperty;
struct _MonoString;
typedef struct _MonoString MonoString;

namespace minty
{
	class ScriptClass;
	class ScriptArguments;

	class ScriptEngine
		: public Engine
	{
		friend ScriptAssembly;
		friend ScriptClass;
		friend ScriptObject;

	private:
		MonoDomain* _rootDomain;
		MonoDomain* _appDomain;
		std::unordered_map<String, ScriptAssembly*> _assemblies;
	public:
		ScriptEngine(Runtime& runtime);

		virtual ~ScriptEngine();

	public:
		static void link();

#pragma region Assemblies

	public:
		/// <summary>
		/// Loads the assembly from the given path. The name is automatically set to the stem of the given path.
		/// </summary>
		/// <param name="path">The path to the assembly.</param>
		/// <returns>True if the assembly was successfully loaded.</returns>
		bool load_assembly(Path const& path, bool const referenceOnly = false);

		/// <summary>
		/// Gets the assembly with the given name.
		/// </summary>
		/// <param name="name">The name of the assembly.</param>
		/// <returns></returns>
		ScriptAssembly const* get_assembly(String const& name) const;

		/// <summary>
		/// Finds the first assembly that contains a class with the given namespace and class names.
		/// </summary>
		/// <param name="namespaceName"></param>
		/// <param name="className"></param>
		/// <returns></returns>
		ScriptAssembly const* find_assembly(String const& namespaceName, String const& className) const;

		/// <summary>
		/// Reloads the assembly with the given name.
		/// </summary>
		/// <param name="name"></param>
		void reload_assembly(String const& name);

		/// <summary>
		/// Unloads the assembly with the given name.
		/// </summary>
		/// <param name="name"></param>
		void unload_assembly(String const& name);

#pragma endregion

#pragma region Mono

	public:
		MonoString* to_mono_string(String const& string) const;

		static String from_mono_string(MonoString* const string);

	private:
		String get_class_name(MonoClass* const klass) const;

		String get_class_namespace(MonoClass* const klass) const;

		bool check_class_inheritance(MonoClass* const klass, MonoClass* const baseKlass) const;

		MonoMethod* get_method(MonoClass* const klass, String const& methodName, int const paramCount = 0) const;

		std::vector<MonoMethod*> get_methods(MonoClass* const klass) const;

		std::vector<MonoMethod*> get_methods(MonoClass* const klass, Scripting::Accessibility const accessibility) const;

		Scripting::Accessibility get_method_accessibility(MonoMethod* const method) const;

		String get_method_name(MonoMethod* const method) const;

		MonoClassField* get_field(MonoClass* const klass, String const& fieldName) const;

		Scripting::Accessibility get_field_accessibility(MonoClassField* const field) const;

		std::vector<MonoClassField*> get_fields(MonoClass* const klass) const;

		std::vector<MonoClassField*> get_fields(MonoClass* const klass, Scripting::Accessibility const accessibility) const;

		String get_field_name(MonoClassField* const field) const;

		MonoType* get_field_type(MonoClassField* const field) const;

		void get_field_value(MonoObject* const object, MonoClassField* const field, void* out) const;

		void set_field_value(MonoObject* const object, MonoClassField* const field, void* in) const;

		MonoProperty* get_property(MonoClass* const klass, String const& propertyName) const;

		void get_property_value(MonoObject* const object, MonoProperty* const prop, void* out) const;

		void set_property_value(MonoObject* const object, MonoProperty* const prop, void* in) const;

		void invoke_method(MonoObject* const object, MonoMethod* const method) const;

		void invoke_method(MonoObject* const object, MonoMethod* const method, std::vector<void*>& arguments) const;

		String get_exception_message(MonoObject* const exception) const;

		MonoObject* create_instance(MonoClass* const classType) const;

		MonoObject* create_instance(MonoClass* const klass, std::vector<void*>& values) const;

		void* unbox(MonoObject* const object) const;
	public:
		static String get_full_name(String const& namespaceName, String const& className);

#pragma endregion

#pragma region Scripting

	public:
		ScriptClass const* find_class(String const& namespaceName, String const& className) const;

		ScriptClass const* find_class(String const& fullName) const;

		ScriptClass const* search_for_class(String const& name) const;

		ScriptObject const& create_object(ScriptClass const& script, UUID id) const;

		ScriptObject const& create_object(ScriptClass const& script, UUID id, ScriptArguments& scriptArguments) const;

		ScriptObject const* get_object(UUID id) const;

		ScriptObject const& get_or_create_object(UUID id, ScriptClass const& script) const;

		ScriptObject const& get_or_create_object(UUID id, ScriptClass const& script, ScriptArguments& scriptArguments) const;

		ScriptObject const& get_or_create_entity(UUID id) const;

		ScriptObject const& get_or_create_component(UUID id, UUID const entityId, ScriptClass const& script) const;

		ScriptObject const& create_object_entity(UUID id) const;

		ScriptObject const& create_object_component(UUID id, UUID const entityId, ScriptClass const& script) const;

		void destroy_object(UUID id);

	public:
		static void link_script(String const& fullName);

		static void link_script(ScriptClass const& script);

#pragma endregion

	};
}