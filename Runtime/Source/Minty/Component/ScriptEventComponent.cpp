#include "pch.h"
#include "ScriptEventComponent.h"

using namespace Minty;

void Minty::ScriptEventComponent::invoke() const
{
	for (auto const& [id, name, value] : scriptMethods)
	{
		value->invoke();
	}
}

void Minty::ScriptEventComponent::invoke(UUID const id) const
{
	if (!scriptMethods.contains(id))
	{
		scriptMethods.at(id)->invoke();
	}
}

void Minty::ScriptEventComponent::invoke(String const& name) const
{
	if (!scriptMethods.contains(name))
	{
		scriptMethods.at(name)->invoke();
	}
}
