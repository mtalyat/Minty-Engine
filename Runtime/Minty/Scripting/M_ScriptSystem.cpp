#include "pch.h"
#include "Minty/Scripting/M_ScriptSystem.h"

#include "Minty/Core/M_Constants.h"
#include "Minty/Scripting/M_ScriptComponent.h"
#include "Minty/Components/M_EnabledComponent.h"
#include "Minty/Scripting/M_ScriptObject.h"
#include "Minty/Entities/M_EntityRegistry.h"
#include "Minty/Scripting/M_ScriptEngine.h"
#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

using namespace Minty;

Minty::ScriptSystem::ScriptSystem(Scene& scene)
	: System("Script", scene)
{}

void Minty::ScriptSystem::load()
{
	System::load();

	EntityRegistry& registry = get_entity_registry();

	for (auto [entity, script, onload, enabled] : registry.view<ScriptComponent const, ScriptOnLoadComponent const, EnabledComponent const>().each())
	{
		for (auto const id : onload.scriptIds)
		{
			script.scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONLOAD);
		}
	}

	for (auto [entity, script, onenable, enabled] : registry.view<ScriptComponent const, ScriptOnEnableComponent const, EnabledComponent const>().each())
	{
		for (auto const id : onenable.scriptIds)
		{
			script.scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONENABLE);
		}
	}

	// remove triggers
	registry.clear<TriggerScriptEvents>();
}

void Minty::ScriptSystem::update(Time const time)
{
	EntityRegistry& registry = get_entity_registry();

	// trigger new entities
	for (auto [entity, script, onload, enabled] : registry.view<ScriptComponent const, ScriptOnLoadComponent const, EnabledComponent const, TriggerScriptEvents const>().each())
	{
		for (auto const id : onload.scriptIds)
		{
			script.scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONLOAD);
		}
	}

	for (auto [entity, script, onenable, enabled] : registry.view<ScriptComponent const, ScriptOnEnableComponent const, EnabledComponent const, TriggerScriptEvents const>().each())
	{
		for (auto const id : onenable.scriptIds)
		{
			script.scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONENABLE);
		}
	}

	// remove triggers
	registry.clear<TriggerScriptEvents>();

	// update as per normal
	for (auto [entity, script, onupdate, enabled] : registry.view<ScriptComponent const, ScriptOnUpdateComponent const, EnabledComponent const>().each())
	{
		for (auto const id : onupdate.scriptIds)
		{
			script.scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONUPDATE);
		}
	}
	
	// unloading handled in finalize
}

void Minty::ScriptSystem::finalize()
{
	EntityRegistry& registry = get_entity_registry();

	// finalize as per normal
	for (auto [entity, script, onfinalize, enabled] : registry.view<ScriptComponent const, ScriptOnFinalizeComponent const, EnabledComponent const>().each())
	{
		for (auto const id : onfinalize.scriptIds)
		{
			script.scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONFINALIZE);
		}
	}
}

void Minty::ScriptSystem::unload()
{
	EntityRegistry& registry = get_entity_registry();

	for (auto [entity, script, onload, enabled] : registry.view<ScriptComponent const, ScriptOnDisableComponent const, EnabledComponent const>().each())
	{
		for (auto const id : onload.scriptIds)
		{
			script.scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONDISABLE);
		}
	}

	for (auto [entity, script, onunload] : registry.view<ScriptComponent const, ScriptOnUnloadComponent const>().each())
	{
		for (auto const id : onunload.scriptIds)
		{
			script.scripts.at(id).invoke(SCRIPT_METHOD_NAME_ONUNLOAD);
		}
	}

	System::unload();
}
