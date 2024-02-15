#pragma once
#include "M_Engine.h"

#include "M_Mono.h"
#include "M_AssemblyType.h"
#include "M_Accessibility.h"
#include <unordered_map>

namespace minty::Scripting
{
	class Script;
}

namespace minty
{
	class ScriptEngine
		: public Engine
	{
	private:
		struct AssemblyInfo
		{
			MonoAssembly* assembly;
			std::unordered_map<String, Scripting::Script*> scripts;
		};
	private:
		MonoDomain* _rootDomain;
		MonoDomain* _appDomain;
		std::unordered_map<AssemblyType, AssemblyInfo> _assemblies;

	public:
		ScriptEngine();

		virtual ~ScriptEngine();

#pragma region Assemblies

	public:
		bool load_assembly(AssemblyType const type, Path const& path);

		void unload_assembly(AssemblyType const type);

	private:
		MonoAssembly* load_mono_assembly(Path const& path) const;

#pragma endregion

#pragma region Scripting

	public:
		MonoAssembly* get_assembly(AssemblyType const assm) const;

		MonoClass* get_class(AssemblyType const assm, String const& namespaceName, String const& className) const;

		std::vector<MonoClass*> get_classes(AssemblyType const assm, MonoClass* const baseKlass = nullptr) const;

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

		void invoke_method(MonoObject* const object, MonoMethod* const method) const;

		void invoke_method(MonoObject* const object, MonoMethod* const method, std::vector<void*>& arguments) const;

		MonoObject* create_instance(MonoClass* const classType) const;

#pragma endregion
	};
}