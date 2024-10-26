#include "pch.h"
#include "CsScriptObject.h"

#include "Minty/Script/CS/CsScriptClass.h"
#include "Minty/Script/CS/CsScriptEngine.h"
#include "Minty/Script/CS/CsScriptField.h"
#include "Minty/Script/CS/CsScriptMethod.h"
#include "Minty/Script/CS/CsScriptProperty.h"

using namespace Minty;

Minty::CsScriptObject::CsScriptObject(ScriptObjectBuilder const& builder)
	: ScriptObject(builder)
	, mp_object(nullptr)
	, m_gcHandle(0)
{
	// get script
	Ref<CsScriptClass> scriptClass = static_cast<Ref<CsScriptClass>>(builder.klass);
	MonoClass* klass = scriptClass->get_class();

	// create instance
	mp_object = mono_object_new(CsScriptEngine::get_app_domain(), scriptClass->get_class());

	MINTY_ASSERT_FORMAT(mp_object != nullptr, "Failed to create an instance of script class \"{}\".", builder.klass->get_full_name());

	// all the default constructor
	mono_runtime_object_init(mp_object);

	// get garbage collector handle
	m_gcHandle = mono_gchandle_new(mp_object, false);
}

Minty::CsScriptObject::CsScriptObject(ScriptObjectBuilder const& builder, ScriptArguments& args)
	: ScriptObject(builder)
	, mp_object(nullptr)
	, m_gcHandle(0)
{
	// get script
	Ref<CsScriptClass> scriptClass = static_cast<Ref<CsScriptClass>>(builder.klass);
	MonoClass* klass = scriptClass->get_class();

	// create instance
	mp_object = mono_object_new(CsScriptEngine::get_app_domain(), scriptClass->get_class());

	MINTY_ASSERT_FORMAT(mp_object != nullptr, "Failed to create an instance of script class \"{}\".", builder.klass->get_full_name());

	Int argc = static_cast<Int>(args.size());

	if (argc)
	{
		// call the constructor with size arguments

		// use first one found in self or parents
		MonoMethod* ctor = mono_class_get_method_from_name(klass, ".ctor", argc);
		MonoClass* parentKlass = mono_class_get_parent(klass);

		while (ctor == nullptr && parentKlass != nullptr)
		{
			ctor = mono_class_get_method_from_name(parentKlass, ".ctor", argc);
			parentKlass = mono_class_get_parent(parentKlass);
		}
		MINTY_ASSERT_FORMAT(ctor != nullptr, "No constructor for {} found with {} arguments.", scriptClass->get_full_name(), argc);

		// call the constructor
		MonoObject* exception = nullptr;
		mono_runtime_invoke(ctor, mp_object, args.get_values().data(), &exception);
	}
	else
	{
		// all the default constructor
		mono_runtime_object_init(mp_object);
	}

	// get garbage collector handle
	m_gcHandle = mono_gchandle_new(mp_object, false);
}

Minty::CsScriptObject::~CsScriptObject()
{
	// free object
	mono_gchandle_free(m_gcHandle);
}

Owner<ScriptField> Minty::CsScriptObject::create_field(String const& name)
{
	Ref<ScriptClass> scriptClass = get_class();
	MonoClass* klass = static_cast<MonoClass*>(scriptClass->get_native());

	ScriptFieldBuilder builder{};
	builder.object = create_ref();

	MonoClassField* field;
	while (klass != nullptr)
	{
		if(field = mono_class_get_field_from_name(klass, name.c_str()))
		{
			return Owner<CsScriptField>(builder, field);
		}

		// check inherited values as well
		klass = mono_class_get_parent(klass);
	}

	return {};
}

Owner<ScriptProperty> Minty::CsScriptObject::create_property(String const& name)
{
	Ref<ScriptClass> scriptClass = get_class();
	MonoClass* klass = static_cast<MonoClass*>(scriptClass->get_native());

	ScriptPropertyBuilder builder{};
	builder.object = create_ref();

	MonoProperty* prop;
	while (klass != nullptr)
	{
		if (prop = mono_class_get_property_from_name(klass, name.c_str()))
		{
			return Owner<CsScriptProperty>(builder, prop);
		}

		// check inherited values as well
		klass = mono_class_get_parent(klass);
	}

	return {};
}

Owner<ScriptMethod> Minty::CsScriptObject::create_method(String const& name, Int const parameterCount)
{
	Ref<ScriptClass> scriptClass = get_class();
	MonoClass* klass = static_cast<MonoClass*>(scriptClass->get_native());

	ScriptMethodBuilder builder{};
	builder.object = create_ref();

	MonoMethod* method;
	while (klass != nullptr)
	{
		if (method = mono_class_get_method_from_name(klass, name.c_str(), parameterCount))
		{
			return Owner<CsScriptMethod>(builder, method);
		}

		// check inherited values as well
		klass = mono_class_get_parent(klass);
	}

	return {};
}