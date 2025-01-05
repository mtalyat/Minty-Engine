#include "pch.h"
#include "ScriptClass.h"

#include "CS/CsScriptClass.h"
#include "CS/CsScriptEngine.h"

using namespace Minty;

String Minty::ScriptClass::get_full_name() const
{
    return ScriptEngine::get_full_name(m_namespace, m_class);
}

Owner<ScriptClass> Minty::ScriptClass::create(ScriptClassBuilder const& builder)
{
    return Owner<CsScriptClass>(builder);
}

Ref<ScriptMethod> Minty::ScriptClass::get_method(String const& name, Int const parameterCount)
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

std::vector<Ref<ScriptMethod>> Minty::ScriptClass::get_methods() const
{
    std::vector<Ref<ScriptMethod>> methods;
    methods.reserve(m_methods.size());

    for (auto const& [name, method] : m_methods)
    {
        methods.push_back(method);
    }

    return methods;
}

void Minty::ScriptClass::invoke(String const& name)
{
    Ref<ScriptMethod> method = get_method(name, 0);

    MINTY_ASSERT_FORMAT(method != nullptr, "{} does not contain a static method named \"{}\".", get_full_name(), name);

    method->invoke();
}

void Minty::ScriptClass::invoke(String const& name, void** const argv, Size const argc)
{
    Ref<ScriptMethod> method = get_method(name, static_cast<Int>(argc));

    MINTY_ASSERT_FORMAT(method != nullptr, "{} does not contain a static method named \"{}\".", get_full_name(), name);

    method->invoke(argv, argc);
}

Bool Minty::ScriptClass::try_invoke(String const& name)
{
    Ref<ScriptMethod> method = get_method(name, 0);

    if (method == nullptr)
    {
        return false;
    }

    method->invoke();

    return true;
}

Bool Minty::ScriptClass::try_invoke(String const& name, void** const argv, Size const argc)
{
    Ref<ScriptMethod> method = get_method(name, static_cast<Int>(argc));

    if (method == nullptr)
    {
        return false;
    }

    method->invoke(argv, argc);

    return true;
}
