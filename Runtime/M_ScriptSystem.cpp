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

	for (auto [entity, script, onload] : registry.view<ScriptComponent const, ScriptOnLoadComponent const>().each())
	{
		for (auto const id : onload.scriptIds)
		{
			script.scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONLOAD);
		}
	}
}

void minty::ScriptSystem::update()
{
	EntityRegistry& registry = get_entity_registry();

	for (auto [entity, script, onupdate] : registry.view<ScriptComponent const, ScriptOnUpdateComponent const>().each())
	{
		for (auto const id : onupdate.scriptIds)
		{
			script.scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONUPDATE);
		}
	}
}

void minty::ScriptSystem::unload()
{
	EntityRegistry& registry = get_entity_registry();

	for (auto [entity, script, onunload] : registry.view<ScriptComponent const, ScriptOnUnloadComponent const>().each())
	{
		for (auto const id : onunload.scriptIds)
		{
			script.scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONUNLOAD);
		}
	}
}
