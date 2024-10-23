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
	initialize_methods(m_methods);
	initialize_fields(m_fields);
	initialize_properties(m_properties);

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
	initialize_methods(m_methods);
	initialize_fields(m_fields);
	initialize_properties(m_properties);

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

void Minty::CsScriptObject::initialize_fields(std::unordered_map<String, Owner<ScriptField>>& fields)
{
	Ref<ScriptClass> scriptClass = get_class();
	MonoClass* klass = static_cast<MonoClass*>(scriptClass->get_native());

	ScriptFieldBuilder builder{};
	builder.object = create_ref();

	void* iter;
	MonoClassField* field;
	String name;
	Owner<ScriptField> scriptField;
	while (klass != nullptr)
	{
		iter = nullptr;
		while (field = mono_class_get_fields(klass, &iter))
		{
			name = String(mono_field_get_name(field));
			scriptField = Owner<CsScriptField>(builder, field);
			fields.emplace(name, std::move(scriptField));
		}

		// get inherited values as well
		klass = mono_class_get_parent(klass);
	}
}

void Minty::CsScriptObject::initialize_properties(std::unordered_map<String, Owner<ScriptProperty>>& properties)
{
	Ref<ScriptClass> scriptClass = get_class();
	MonoClass* klass = static_cast<MonoClass*>(scriptClass->get_native());

	ScriptPropertyBuilder builder{};
	builder.object = create_ref();

	void* iter;
	MonoProperty* prop;
	String name;
	Owner<ScriptProperty> scriptProperty;
	while (klass != nullptr)
	{
		iter = nullptr;
		while (prop = mono_class_get_properties(klass, &iter))
		{
			name = String(mono_property_get_name(prop));
			scriptProperty = Owner<CsScriptProperty>(builder, prop);
			properties.emplace(name, std::move(scriptProperty));
		}

		// get inherited values as well
		klass = mono_class_get_parent(klass);
	}
}

void Minty::CsScriptObject::initialize_methods(std::unordered_map<String, Owner<ScriptMethod>>& methods)
{
	Ref<ScriptClass> scriptClass = get_class();
	MonoClass* klass = static_cast<MonoClass*>(scriptClass->get_native());

	ScriptMethodBuilder builder{};
	builder.object = create_ref();

	void* iter;
	MonoMethod* method;
	String name;
	Owner<ScriptMethod> scriptMethod;
	while (klass != nullptr)
	{
		iter = nullptr;
		while (method = mono_class_get_methods(klass, &iter))
		{
			name = String(mono_method_get_name(method));
			scriptMethod = Owner<CsScriptMethod>(builder, method);
			methods.emplace(name, std::move(scriptMethod));
		}

		// get inherited values as well
		klass = mono_class_get_parent(klass);
	}
}