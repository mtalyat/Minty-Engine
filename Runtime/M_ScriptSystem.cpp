#include "pch.h"
#include "M_ScriptSystem.h"

#include "M_Constants.h"
#include "M_ScriptComponent.h"
#include "M_EnabledComponent.h"
#include "M_ScriptObject.h"
#include "M_EntityRegistry.h"
#include "M_Runtime.h"
#include "M_ScriptEngine.h"
#include "M_AssetEngine.h"
#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

minty::ScriptSystem::ScriptSystem(Runtime& runtime, Scene& scene)
	: System("Script", runtime, scene)
	, _ids()
{}

void minty::ScriptSystem::load()
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
}

void minty::ScriptSystem::update()
{
	EntityRegistry& registry = get_entity_registry();

	for (auto [entity, script, onupdate, enabled] : registry.view<ScriptComponent const, ScriptOnUpdateComponent const, EnabledComponent const>().each())
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

void minty::ScriptSystem::deserialize(Reader const& reader)
{
	// load all of the scripts and their ids
	register_assets("scripts", reader, [this](AssetEngine& engine, Path const& path)
		{
			Asset* asset = engine.load_script(path);

			_ids.emplace(asset->get_id());
			get_runtime().get_script_engine().register_script_id(asset->get_id(), path.stem().string());

			return asset;
		});
}
