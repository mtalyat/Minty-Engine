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

Accessibility Minty::CsScriptField::get_accessibility() const
{
	return CsScriptEngine::get_field_accessibility(mp_field);
}

Bool Minty::CsScriptField::is_static() const
{
	uint32_t flags = mono_field_get_flags(mp_field);
	return static_cast<Bool>(flags & MONO_FIELD_ATTR_STATIC);
}

Type Minty::CsScriptField::get_type() const
{
	MonoType* type = mono_field_get_type(mp_field);
	return CsScriptEngine::to_type(type);
}

String Minty::CsScriptField::get_type_full_name() const
{
	MonoType* type = mono_field_get_type(mp_field);
	return String(mono_type_get_name_full(type, MonoTypeNameFormat::MONO_TYPE_NAME_FORMAT_FULL_NAME));
}
