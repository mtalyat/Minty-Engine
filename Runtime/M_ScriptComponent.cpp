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

void minty::ScriptEventComponent::invoke(String const& method, ScriptComponent const& script, std::unordered_set<String> const& componentNames) const
{
	for (String const& name : componentNames)
	{
		ID id = script.scripts.find(name);

		if (id == ERROR_ID) continue;

		script.scripts.at(id).invoke(method);
	}
}
