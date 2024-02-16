#include "pch.h"
#include "M_ScriptSystem.h"

#include "M_Constants.h"
#include "M_ScriptComponent.h"
#include "M_ScriptObject.h"
#include "M_EntityRegistry.h"

using namespace minty;

minty::ScriptSystem::ScriptSystem(Runtime& runtime, ID const sceneId)
	: System(runtime, sceneId)
{}

void minty::ScriptSystem::load()
{
	EntityRegistry& registry = get_entity_registry();

	for (auto [entity, script] : registry.view<ScriptComponent>().each())
	{
		for (auto const& pair : script.scripts)
		{
			pair.second.invoke(SCRIPT_METHOD_NAME_ONLOAD);
		}
	}
}

void minty::ScriptSystem::update()
{
	EntityRegistry& registry = get_entity_registry();

	for (auto [entity, script] : registry.view<ScriptComponent>().each())
	{
		for (auto const& pair : script.scripts)
		{
			pair.second.invoke(SCRIPT_METHOD_NAME_ONUPDATE);
		}
	}
}

void minty::ScriptSystem::unload()
{
	EntityRegistry& registry = get_entity_registry();

	for (auto [entity, script] : registry.view<ScriptComponent>().each())
	{
		for (auto const& pair : script.scripts)
		{
			pair.second.invoke(SCRIPT_METHOD_NAME_ONUNLOAD);
		}
	}
}
