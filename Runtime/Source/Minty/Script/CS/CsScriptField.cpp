#include "pch.h"
#include "CsScriptField.h"

#include "Minty/Script/CS/CsScriptEngine.h"
#include "Minty/Script/ScriptObject.h"

using namespace Minty;

Minty::CsScriptField::CsScriptField(ScriptFieldBuilder const& builder, MonoClassField* const field)
	: ScriptField(builder)
	, mp_field(field)
{}

String Minty::CsScriptField::get_name() const
{
	return String(mono_field_get_name(mp_field));
}

void Minty::CsScriptField::set(void* const value) const
{
	MonoObject* object = static_cast<MonoObject*>(get_object()->get_native());
	mono_field_set_value(object, mp_field, value);
}

void Minty::CsScriptField::get(void* const value) const
{
	MonoObject* object = static_cast<MonoObject*>(get_object()->get_native());
	mono_field_get_value(object, mp_field, value);
}

Type Minty::CsScriptField::get_type() const
{
	MonoType* type = mono_field_get_type(mp_field);
	return CsScriptEngine::to_type(type);
}
