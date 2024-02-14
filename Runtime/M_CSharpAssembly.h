#pragma once
#include "M_Assembly.h"

namespace minty
{
	/// <summary>
	/// A C-Sharp assembly.
	/// </summary>
	class CSharpAssembly
		: public Assembly
	{
	private:
		MonoDomain* _rootDomain;
		MonoDomain* _appDomain;
		MonoAssembly* _assembly;

	public:
		CSharpAssembly();

		~CSharpAssembly();

		void load(Path const& path) override;

	private:
		MonoClass* get_class(String const& namespaceName, String const& className) const;

		MonoMethod* get_method(MonoClass* const klass, String const& methodName, int const paramCount = 0) const;

		MonoClassField* get_field(MonoClass* const klass, String const& fieldName) const;

		void get_field_value(MonoObject* const object, MonoClassField* const field, void* out) const;

		void set_field_value(MonoObject* const object, MonoClassField* const field, void* in) const;

		MonoProperty* get_property(MonoClass* const klass, String const& propertyName) const;

		void invoke_method(MonoObject* const object, MonoMethod* const method) const;

		void invoke_method(MonoObject* const object, MonoMethod* const method, std::vector<void*>& arguments) const;

		MonoObject* create_instance(MonoClass* const classType) const;
	};
}