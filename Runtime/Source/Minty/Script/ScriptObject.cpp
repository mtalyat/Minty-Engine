#include "pch.h"
#include "ScriptObject.h"

#include "Minty/Script/CS/CsScriptObject.h"
#include "Minty/Script/ScriptClass.h"

using namespace Minty;

Minty::ScriptObject::ScriptObject(ScriptObjectBuilder const& builder)
    : m_class(builder.klass)
    , m_methods()
    , m_fields()
    , m_properties()
{
    id = builder.id;
}

Owner<ScriptObject> Minty::ScriptObject::create(ScriptObjectBuilder const& builder)
{
    return Owner<CsScriptObject>(builder);
}

Owner<ScriptObject> Minty::ScriptObject::create(ScriptObjectBuilder const& builder, ScriptArguments& args)
{
    return Owner<CsScriptObject>(builder, args);
}

void Minty::ScriptObject::serialize(Writer& writer) const
{
    // write all fields
    for (auto const& [name, field] : m_fields)
    {
        Type type = field->get_type();
        Byte* data = new Byte[sizeof_type(type)];
        field->get(data);
        writer.write(name, data, type);
        delete[] data;
    }
}

void Minty::ScriptObject::deserialize(Reader& reader)
{
    // read all fields, if able
    for (auto const& [name, field] : m_fields)
    {
        Type type = field->get_type();
        if (type == Type::Undefined) continue;
        Byte* data = new Byte[sizeof_type(type)];
        if (reader.read(name, data, type))
        {
            field->set(data);
        }
        delete[] data;
    }
}

Ref<ScriptField> Minty::ScriptObject::get_field(String const& name)
{
    auto found = m_fields.find(name);

    if (found == m_fields.end())
    {
        Owner<ScriptField> field = create_field(name);
        Ref<ScriptField> ref = field.create_ref();
        m_fields.emplace(name, std::move(field));
        return ref;
    }

    return found->second.create_ref();
}

std::vector<Ref<ScriptField>> Minty::ScriptObject::get_fields() const
{
    std::vector<Ref<ScriptField>> fields;
    fields.reserve(m_fields.size());

    for (auto const& [name, field] : m_fields)
    {
        fields.push_back(field);
    }

    return fields;
}

Ref<ScriptProperty> Minty::ScriptObject::get_property(String const& name)
{
    auto found = m_properties.find(name);

    if (found == m_properties.end())
    {
        Owner<ScriptProperty> prop = create_property(name);
        Ref<ScriptProperty> ref = prop.create_ref();
        m_properties.emplace(name, std::move(prop));
        return ref;
    }

    return found->second.create_ref();
}

std::vector<Ref<ScriptProperty>> Minty::ScriptObject::get_properties() const
{
    std::vector<Ref<ScriptProperty>> properties;
    properties.reserve(m_properties.size());

    for (auto const& [name, prop] : m_properties)
    {
        properties.push_back(prop);
    }

    return properties;
}

Ref<ScriptMethod> Minty::ScriptObject::get_method(String const& name, Int const parameterCount)
{
    auto found = m_methods.find(name);

    if (found == m_methods.end())
    {
        Owner<ScriptMethod> method = create_method(name, parameterCount);
        Ref<ScriptMethod> ref = method.create_ref();
        m_methods.emplace(name, std::move(method));
        return ref;
    }

    return found->second.create_ref();
}

std::vector<Ref<ScriptMethod>> Minty::ScriptObject::get_methods() const
{
    std::vector<Ref<ScriptMethod>> methods;
    methods.reserve(m_methods.size());

    for (auto const& [name, method] : m_methods)
    {
        methods.push_back(method);
    }

    return methods;
}

void Minty::ScriptObject::invoke(String const& name)
{
    Ref<ScriptMethod> method = get_method(name, 0);

    MINTY_ASSERT_FORMAT(method != nullptr, "{} does not contain a method named \"{}\".", get_class()->get_full_name(), name);

    method->invoke();
}

void Minty::ScriptObject::invoke(String const& name, void** const argv, Size const argc)
{
    Ref<ScriptMethod> method = get_method(name, static_cast<Int>(argc));

    MINTY_ASSERT_FORMAT(method != nullptr, "{} does not contain a method named \"{}\".", get_class()->get_full_name(), name);

    method->invoke(argv, argc);
}

Bool Minty::ScriptObject::try_invoke(String const& name)
{
    Ref<ScriptMethod> method = get_method(name, 0);

    if (method == nullptr)
    {
        return false;
    }

    method->invoke();

    return true;
}

Bool Minty::ScriptObject::try_invoke(String const& name, void** const argv, Size const argc)
{
    Ref<ScriptMethod> method = get_method(name, static_cast<Int>(argc));

    if (method == nullptr)
    {
        return false;
    }

    method->invoke(argv, argc);

    return true;
}

void Minty::ScriptObject::set_field(String const& name, void* const value)
{
    Ref<ScriptField> field = get_field(name);

    MINTY_ASSERT_FORMAT(field != nullptr, "{} does not contain a field named \"{}\".", get_class()->get_full_name(), name);

    field->set(value);
}

void Minty::ScriptObject::get_field(String const& name, void* const value)
{
    Ref<ScriptField> field = get_field(name);

    MINTY_ASSERT_FORMAT(field != nullptr, "{} does not contain a field named \"{}\".", get_class()->get_full_name(), name);

    field->get(value);
}

void Minty::ScriptObject::set_property(String const& name, void** const value)
{
    Ref<ScriptProperty> prop = get_property(name);

    MINTY_ASSERT_FORMAT(prop != nullptr, "{} does not contain a property named \"{}\".", get_class()->get_full_name(), name);

    prop->set(value);
}

void Minty::ScriptObject::get_property(String const& name, void** const value)
{
    Ref<ScriptProperty> prop = get_property(name);

    MINTY_ASSERT_FORMAT(prop != nullptr, "{} does not contain a property named \"{}\".", get_class()->get_full_name(), name);

    prop->set(value);
}