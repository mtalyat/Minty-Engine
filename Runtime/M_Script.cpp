#include "pch.h"
#include "M_Script.h"

#include "M_Mono.h"

using namespace minty;
using namespace minty::Scripting;

minty::Scripting::Script::Script(ScriptEngine& engine, AssemblyType const assm, MonoClass& klass)
	: _engine(&engine)
	, _assm(assm)
	, _class(&klass)
{}

ScriptEngine& minty::Scripting::Script::get_engine() const
{
	return *_engine;
}

AssemblyType minty::Scripting::Script::get_assembly_type() const
{
	return _assm;
}

MonoClass* minty::Scripting::Script::get_class() const
{
	return _class;
}

void minty::Scripting::Script::serialize(Writer& writer) const
{

}

void minty::Scripting::Script::deserialize(Reader const& reader)
{
	
}
