#include "pch.h"
#include "M_System.h"

#include "M_Runtime.h"
#include "M_AssetEngine.h"
#include "M_Scene.h"
#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"

#include "M_Reader.h"

using namespace minty;

minty::System::System(String const& name, Runtime& engine, Scene& scene)
	: SceneObject(engine, scene)
	, _enabled(true)
	, _name(name)
{}

minty::System::~System()
{}

String const& minty::System::get_name() const
{
	return _name;
}

EntityRegistry& minty::System::get_entity_registry() const
{
	return get_scene().get_entity_registry();
}

SystemRegistry& minty::System::get_system_registry() const
{
	return get_scene().get_system_registry();
}

void minty::System::reset()
{}

void minty::System::set_enabled(bool const enabled)
{
	_enabled = enabled;
}

bool minty::System::is_enabled() const
{
	return _enabled;
}

void minty::System::load()
{
	load_registered_assets();
}

void minty::System::unload()
{
	unload_registered_assets();
}

void minty::System::register_assets(std::vector<Path> const& paths, RegisterFunc const& func)
{
	// add to unloaded
	_unloadedAssets.push_back({ paths, func });
}

void minty::System::register_assets(Reader const& reader, String const& name, RegisterFunc const& func)
{
	std::vector<Path> paths;
	if (reader.try_read_vector<Path>(name, paths))
	{
		register_assets(paths, func);
	}
}

void minty::System::load_registered_assets()
{
	// load each asset into the engine and save its ID so it can be unloaded later
	AssetEngine& assets = get_runtime().get_asset_engine();

	for (auto const& [paths, func] : _unloadedAssets)
	{
		for (auto const& path : paths)
		{
			if (Asset* asset = func(assets, path))
			{
				_loadedAssets.emplace(asset->get_id());
			}
		}
	}
}

void minty::System::unload_registered_assets()
{
	// unload each asset from the engine
	AssetEngine& assets = get_runtime().get_asset_engine();

	for (auto const id : _loadedAssets)
	{
		assets.unload(id);
	}
}

String minty::to_string(System const& value)
{
	return std::format("System()");
}
