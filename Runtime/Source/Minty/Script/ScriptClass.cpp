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
