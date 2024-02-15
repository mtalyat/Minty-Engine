#pragma once
#include "M_Object.h"

#include "M_AssemblyType.h"
#include "M_Mono.h"

namespace minty
{
	class ScriptEngine;
}

namespace minty::Scripting
{
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

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}