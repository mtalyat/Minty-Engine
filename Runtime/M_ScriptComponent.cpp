#include "pch.h"
#include "M_ScriptComponent.h"

using namespace minty;

void minty::ScriptEventComponent::invoke(String const& method, ScriptComponent const& script) const
{
	for (ID const id : scriptIds)
	{
		script.scripts.at(id).invoke(method);
	}
}
