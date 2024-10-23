#include "pch.h"
#include "ScriptSystem.h"

#include "Minty/Core/Constants.h"
#include "Minty/Component/AllComponents.h"
#include "Minty/Script/ScriptEngine.h"
#include "Minty/Script/ScriptObject.h"
#include "Minty/Entity/EntityRegistry.h"
#include "Minty/Asset/AssetManager.h"
#include "Minty/Serialize/Reader.h"
#include "Minty/Serialize/Writer.h"

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
		for (auto const& method : onload.scriptMethods)
		{
			method.value->invoke();
		}
	}

	for (auto [entity, script, onenable, enabled] : registry.view<ScriptComponent const, ScriptOnEnableComponent const, EnabledComponent const>().each())
	{
		for (auto const& method : onenable.scriptMethods)
		{
			method.value->invoke();
		}
	}

	// remove triggers
	registry.clear<TriggerScriptEvents>();
}

void Minty::ScriptSystem::update(Time const& time)
{
	EntityRegistry& registry = get_entity_registry();

	// trigger new entities
	for (auto [entity, script, onload, enabled] : registry.view<ScriptComponent const, ScriptOnLoadComponent const, EnabledComponent const, TriggerScriptEvents const>().each())
	{
		for (auto const& method : onload.scriptMethods)
		{
			method.value->invoke();
		}
	}

	for (auto [entity, script, onenable, enabled] : registry.view<ScriptComponent const, ScriptOnEnableComponent const, EnabledComponent const, TriggerScriptEvents const>().each())
	{
		for (auto const& method : onenable.scriptMethods)
		{
			method.value->invoke();
		}
	}

	// remove triggers
	registry.clear<TriggerScriptEvents>();

	// update as per normal
	for (auto [entity, script, onupdate, enabled] : registry.view<ScriptComponent const, ScriptOnUpdateComponent const, EnabledComponent const>().each())
	{
		for (auto const& method : onupdate.scriptMethods)
		{
			method.value->invoke();
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
		for (auto const& method : onfinalize.scriptMethods)
		{
			method.value->invoke();
		}
	}
}

void Minty::ScriptSystem::unload()
{
	EntityRegistry& registry = get_entity_registry();

	for (auto [entity, script, onload, enabled] : registry.view<ScriptComponent const, ScriptOnDisableComponent const, EnabledComponent const>().each())
	{
		for (auto const& method : onload.scriptMethods)
		{
			method.value->invoke();
		}
	}

	for (auto [entity, script, onunload] : registry.view<ScriptComponent const, ScriptOnUnloadComponent const>().each())
	{
		for (auto const& method : onunload.scriptMethods)
		{
			method.value->invoke();
		}
	}

	System::unload();
}
