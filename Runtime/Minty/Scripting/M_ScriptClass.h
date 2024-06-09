#pragma once
#include "Minty/Types/M_Object.h"

#include "Minty/Scripting/M_ScriptObject.h"

struct _MonoClass;
typedef struct _MonoClass MonoClass;
struct _MonoObject;
typedef struct _MonoObject MonoObject;
struct _MonoType;
typedef struct _MonoType MonoType;

namespace Minty
{
	class ScriptAssembly;

	/// <summary>
	/// Holds meta data for the C# script.
	/// </summary>
	class ScriptClass
		: public Object
	{
		friend class ScriptObject;

	private:
		// all the data is stored on the C# side
		// we can get it through serialization
		String _namespace;
		String _name;
		ScriptAssembly* _assembly;
		MonoClass* _class;

	public:
		ScriptClass(String const& namespaceName, String const& className, ScriptAssembly& assembly);

		ScriptAssembly& get_assembly() const { return *_assembly; }

		String const& get_namespace() const { return _namespace; }

		String const& get_name() const { return _name; }

		String get_full_name() const;

		bool is_derived_from(ScriptClass const& other) const;

		void invoke(String const& name) const;

		void invoke(String const& name, ScriptArguments& arguments) const;

		bool try_invoke(String const& name) const;

		bool try_invoke(String const& name, ScriptArguments& arguments) const;

		ScriptObject create_object(UUID const id = UUID::create()) const;

		bool has_method(String const& name) const;

		MonoType* get_type() const;
	};
}