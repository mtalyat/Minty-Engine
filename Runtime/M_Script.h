#pragma once
#include "M_Object.h"

#include "M_AssemblyType.h"

struct _MonoClass;
typedef struct _MonoClass MonoClass;
struct _MonoObject;
typedef struct _MonoObject MonoObject;

namespace minty
{
	class ScriptEngine;

	/// <summary>
	/// Holds meta data for the C# script.
	/// </summary>
	class Script
		: public Object
	{
	private:
		// all the data is stored on the C# side
		// we can get it through serialization
		ScriptEngine* _engine;
		AssemblyType _assm;
		MonoClass* _class;

	public:
		Script(ScriptEngine& engine, AssemblyType const assm, MonoClass& klass);

		ScriptEngine& get_engine() const;

		AssemblyType get_assembly_type() const;

		MonoClass* get_class() const;

		MonoObject* create_instance() const;

		bool has_method(String const& name) const;
	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}