#include "pch.h"
#include "CsScriptProperty.h"

#include "Minty/Script/CS/CsScriptEngine.h"
#include "Minty/Script/ScriptObject.h"

using namespace Minty;

Minty::CsScriptProperty::CsScriptProperty(ScriptPropertyBuilder const& builder, MonoProperty* const property)
	: ScriptProperty(builder)
	, mp_property(property)
{}

String Minty::CsScriptProperty::get_name() const
{
	return String(mono_property_get_name(mp_property));
}

void Minty::CsScriptProperty::set(void** const value) const
{
	MonoObject* object = static_cast<MonoObject*>(get_object()->get_native());

	MonoObject* exception;

	mono_property_set_value(mp_property, object, value, &exception);

	if (exception)
	{
		MINTY_ERROR(CsScriptEngine::get_exception_message(exception));
	}
}

void Minty::CsScriptProperty::get(void** const value) const
{
	MonoObject* object = static_cast<MonoObject*>(get_object()->get_native());

	MonoObject* exception;

	mono_property_get_value(mp_property, object, value, &exception);

	if (exception)
	{
		MINTY_ERROR(CsScriptEngine::get_exception_message(exception));
	}
}

Type Minty::CsScriptProperty::get_type() const
{
	MonoMethod* getterMethod = mono_property_get_get_method(mp_property);
	if (!getterMethod)
	{
		return Type::Undefined;
	}

	MonoMethodSignature* methodSignature = mono_method_signature(getterMethod);
	if (!methodSignature)
	{
		return Type::Undefined;
	}

	MonoType* returnType = mono_signature_get_return_type(methodSignature);
	return CsScriptEngine::to_type(returnType);
}
