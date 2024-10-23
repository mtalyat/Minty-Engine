#include "pch.h"
#include "ScriptAssembly.h"

#include "CS/CsScriptAssembly.h"

using namespace Minty;

void Minty::ScriptAssembly::emplace_class(Owner<ScriptClass> const klass)
{
    m_classesName.emplace(klass->get_full_name(), klass);
    m_classesId.emplace(klass->id(), klass);
}

Ref<ScriptClass> Minty::ScriptAssembly::get_class(String const& fullName) const
{
    auto found = m_classesName.find(fullName);

    if (found == m_classesName.end())
    {
        return Ref<ScriptClass>();
    }

    return found->second.create_ref();
}

Ref<ScriptClass> Minty::ScriptAssembly::get_class(UUID const id) const
{
    auto found = m_classesId.find(id);

    if (found == m_classesId.end())
    {
        return Ref<ScriptClass>();
    }

    return found->second.create_ref();
}

Owner<ScriptAssembly> Minty::ScriptAssembly::create(ScriptAssemblyBuilder const& builder)
{
    return Owner<CsScriptAssembly>(builder);
}
