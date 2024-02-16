#include "pch.h"
#include "M_Script.h"

#include "M_Mono.h"

#include "M_ScriptObject.h"
#include "M_ScriptEngine.h"

using namespace minty;

minty::Script::Script(ScriptEngine& engine, AssemblyType const assm, MonoClass& klass)
	: _engine(&engine)
	, _assm(assm)
	, _class(&klass)
{}

ScriptEngine& minty::Script::get_engine() const
{
	return *_engine;
}

AssemblyType minty::Script::get_assembly_type() const
{
	return _assm;
}

MonoClass* minty::Script::get_class() const
{
	return _class;
}

MonoObject* minty::Script::create_instance() const
{
	return _engine->create_instance(_class);
}

bool minty::Script::has_method(String const& name) const
{
	return _engine->get_method(_class, name);
}

void minty::Script::serialize(Writer& writer) const
{

}

void minty::Script::deserialize(Reader const& reader)
{
	
}
