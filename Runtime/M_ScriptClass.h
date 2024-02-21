#pragma once
#include "M_Object.h"

#include "M_ScriptObject.h"

struct _MonoClass;
typedef struct _MonoClass MonoClass;
struct _MonoObject;
typedef struct _MonoObject MonoObject;
struct _MonoType;
typedef struct _MonoType MonoType;

namespace minty
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

		ScriptAssembly& get_assembly() const;

		String const& get_namespace() const;

		String const& get_name() const;

		String get_full_name() const;

		bool is_derived_from(ScriptClass const& other) const;

		ScriptObject create_object(UUID const id = UUID()) const;

		bool has_method(String const& name) const;

		MonoType* get_type() const;
	};
}