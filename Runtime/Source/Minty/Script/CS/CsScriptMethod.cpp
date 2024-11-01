#include "pch.h"
#include "CsScriptMethod.h"

#include "Minty/Script/CS/CsScriptEngine.h"
#include "Minty/Script/ScriptClass.h"
#include "Minty/Script/ScriptObject.h"

using namespace Minty;

Minty::CsScriptMethod::CsScriptMethod(ScriptMethodBuilder const& builder, MonoMethod* const method)
	: ScriptMethod(builder)
	, mp_method(method)
{}

String Minty::CsScriptMethod::get_name() const
{
	return String(mono_method_get_name(mp_method));
}

void Minty::CsScriptMethod::invoke() const
{
	Ref<ScriptObject> scriptObject = get_object();
	MonoObject* object = nullptr;
	if (scriptObject != nullptr)
	{
		object = static_cast<MonoObject*>(scriptObject->get_native());
	}

	MonoObject* exception = nullptr;
	mono_runtime_invoke(mp_method, object, nullptr, &exception);

	if (exception)
	{
		MINTY_ERROR(CsScriptEngine::get_exception_message(exception));
	}
}

void Minty::CsScriptMethod::invoke(void** const argv, Size const argc) const
{
	Ref<ScriptObject> scriptObject = get_object();
	MonoObject* object = nullptr;
	if (scriptObject != nullptr)
	{
		object = static_cast<MonoObject*>(scriptObject->get_native());
	}

	MonoObject* exception;
	mono_runtime_invoke(mp_method, object, argv, &exception);

	if (exception)
	{
		MINTY_ERROR(CsScriptEngine::get_exception_message(exception));
	}
}
