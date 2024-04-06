#include "pch.h"
#include "M_Scene.h"

#include "M_Runtime.h"
#include "M_AssetEngine.h"
#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"

#include "M_TransformComponent.h"
#include "M_UITransformComponent.h"
#include "M_CanvasComponent.h"
#include "M_RelationshipComponent.h"
#include "M_DirtyComponent.h"
#include "M_CameraComponent.h"
#include "M_RenderSystem.h"
#include "M_ScriptComponent.h"
#include "M_Reader.h"
#include "M_Writer.h"
#include "M_SerializationData.h"

using namespace minty;

minty::Scene::Scene(SceneBuilder const& builder, Runtime& engine)
	: Asset(builder.id, builder.path, engine)
	, _entities(new EntityRegistry(engine, *this))
	, _systems(new SystemRegistry(engine, *this))
	, _loaded()
	, _registeredAssets()
	, _unloadedAssets()
	, _loadedAssets()
{}

minty::Scene::~Scene()
{
	delete _entities;
	delete _systems;
}

minty::Scene::Scene(Scene&& other) noexcept
	: Asset(std::move(other))
	, _entities(std::move(other._entities))
	, _systems(std::move(other._systems))
	, _loaded(std::move(other._loaded))
	, _registeredAssets(std::move(other._registeredAssets))
	, _unloadedAssets(std::move(other._unloadedAssets))
	, _loadedAssets(std::move(other._loadedAssets))
{}

Scene& minty::Scene::operator=(Scene&& other) noexcept
{
	if (this != &other)
	{
		_entities = std::move(other._entities);
		_systems = std::move(other._systems);
		_loaded = std::move(other._loaded);
		_registeredAssets = std::move(other._registeredAssets);
		_unloadedAssets = std::move(other._unloadedAssets);
		_loadedAssets = std::move(other._loadedAssets);
	}

	return *this;
}

EntityRegistry& minty::Scene::get_entity_registry() const
{
	return *_entities;
}

SystemRegistry& minty::Scene::get_system_registry() const
{
	return *_systems;
}

bool minty::Scene::is_loaded() const
{
	return _loaded;
}

void minty::Scene::load()
{
	load_registered_assets();
	_systems->load();

	AssetEngine& assets = get_runtime().get_asset_engine();
	
	// read scene data from disk
	Node node = assets.read_file_node(get_path());
	SerializationData data =
	{
		.scene = this,
		.entity = NULL_ENTITY
	};
	Reader reader(node, &data);
	reader.read_serializable("entities", *_entities);

	// sort quick after loading in case entities are out of order, if the file was manually edited
	sort();

	// select camera if needed
	if (RenderSystem* renderSystem = _systems->find<RenderSystem>())
	{
		renderSystem->set_main_camera(_entities->find_by_type<CameraComponent>());
	}

	// finally mark as loaded
	_loaded = true;

	// call all load/enable/etc. events on entities already in the system
	for (auto&& [entity, script, onLoad] : _entities->view<ScriptComponent const, ScriptOnLoadComponent const>().each())
	{
		onLoad.invoke(SCRIPT_METHOD_NAME_ONLOAD, script);
	}

	for (auto&& [entity, script, onEnable] : _entities->view<ScriptComponent const, ScriptOnEnableComponent const>().each())
	{
		onEnable.invoke(SCRIPT_METHOD_NAME_ONENABLE, script);
	}
}

void minty::Scene::update()
{
	if (get_runtime().get_mode() == RunMode::Normal)
	{
		// update systems
		_systems->update();
	}
}

void minty::Scene::sort()
{
	EntityRegistry const* er = _entities;

	_entities->sort<RelationshipComponent>([er](Entity const left, Entity const right)
		{
			RelationshipComponent const& leftR = er->get<RelationshipComponent>(left);
			RelationshipComponent const& rightR = er->get<RelationshipComponent>(right);

			bool value = rightR.parent == left || // put parents on left of children
				leftR.next == right || // put siblings in order
				// put in order based on parent sibling index
				(!(leftR.parent == right || rightR.next == left) && (leftR.parent < rightR.parent || (leftR.parent == rightR.parent && left < right)));

			return value;
		});
}

void minty::Scene::fixed_update()
{
	if (get_runtime().get_mode() == RunMode::Normal)
	{
		// update systems
		_systems->fixed_update();

		// TODO: update transforms...
	}
}

void minty::Scene::unload()
{
	_systems->unload();
	unload_registered_assets();

	_loaded = false;
}

void minty::Scene::finalize()
{
	// update all the dirty tags
	
	// sort the hierarchy
	sort();

	// update dirty transform group with relationships
	auto view = _entities->view<DirtyComponent const, TransformComponent, RelationshipComponent const>();
	view.use<RelationshipComponent const>();
	for (auto [entity, dirty, transform, relationship] : view.each())
	{
		// if parent, apply local to parent global for this global
		// if no parent, set global to local
		if (relationship.parent != NULL_ENTITY)
		{
			// parent

			// get parent Transform, if any
			if (TransformComponent* parentTransform = _entities->try_get<TransformComponent>(relationship.parent))
			{
				transform.globalMatrix = parentTransform->globalMatrix * transform.get_local_matrix();

				continue;
			}
		}

		// no parent
		transform.globalMatrix = transform.get_local_matrix();
	}
	
	// update dirty transform group with no relationships
	for (auto [entity, dirty, transform] : _entities->view<DirtyComponent const, TransformComponent>(entt::exclude<RelationshipComponent>).each())
	{
		// no parent, set to self matrix
		transform.globalMatrix = transform.get_local_matrix();
	}

	Window const& window = get_runtime().get_window();
	RectF windowRect(0, 0, window.get_frame_width(), window.get_frame_height());

	// update canvases?

	// update dirty UI transforms
	auto uiView = _entities->view<DirtyComponent const, UITransformComponent, RelationshipComponent const>();
	uiView.use<RelationshipComponent const>();
	for (auto [entity, dirty, transform, relationship] : uiView.each())
	{
		// if parent, apply local to parent global for this global
		// if no parent, set global to local
		if (relationship.parent != NULL_ENTITY)
		{
			// parent

			UITransformComponent& parentTransform = _entities->get<UITransformComponent>(relationship.parent);
			transform.update_global_rect(parentTransform.globalRect);
		}
		else
		{
			// no parent

			// set to not draw
			transform.globalRect = RectF();
		}
	}

	// update dirty UI transform group with no relationships
	for (auto [entity, dirty, transform] : _entities->view<DirtyComponent const, UITransformComponent>(entt::exclude<RelationshipComponent>).each())
	{
		// no parent, set to self matrix
		transform.update_global_rect(windowRect);
	}

	// remove all dirty tags
	_entities->clear<DirtyComponent>();

	// destroy all entities tagged with the destroy tag
	_entities->destroy_queued();
}

void minty::Scene::register_asset(Path const& path)
{
	MINTY_ASSERT(!_registeredAssets.contains(path));

	AssetData data
	{
		.index = _unloadedAssets.size(),
		.id = INVALID_UUID,
	};

	_unloadedAssets.push_back(path);

	// load asset if needed
	if (_loaded)
	{
		AssetEngine& assets = get_runtime().get_asset_engine();

		if (Asset* asset = assets.load_asset(path))
		{
			_loadedAssets.emplace(asset->get_id());
			data.id = asset->get_id();
		}
	}

	_registeredAssets.emplace(path, data);

	// sort all since a new thing was added
	sort_registered_assets();
}

void minty::Scene::unregister_asset(Path const& path)
{
	MINTY_ASSERT(_registeredAssets.contains(path));

	AssetEngine& assets = get_runtime().get_asset_engine();

	AssetData& data = _registeredAssets.at(path);

	// unload asset if needed
	if (_loaded)
	{
		if (data.id.valid())
		{
			assets.unload(data.id);
			_loadedAssets.erase(data.id);
		}
	}
	
	_unloadedAssets.erase(_unloadedAssets.begin() + data.index);
	_registeredAssets.erase(path);

	// update indices since an asset was removed in the middle
	update_registered_indices();
}

bool minty::Scene::is_registered(Path const& assetPath)
{
	return _registeredAssets.contains(assetPath);
}

void minty::Scene::load_registered_assets()
{
	// load each asset into the engine and save its ID so it can be unloaded later
	AssetEngine& assets = get_runtime().get_asset_engine();

	for (auto const& path : _unloadedAssets)
	{
		if (Asset* asset = assets.load_asset(path))
		{
			AssetData& data = _registeredAssets.at(path);

			_loadedAssets.emplace(asset->get_id());
			data.id = asset->get_id();
		}
	}
}

void minty::Scene::unload_registered_assets()
{
	// unload each asset from the engine
	AssetEngine& assets = get_runtime().get_asset_engine();

	for (auto const id : _loadedAssets)
	{
		assets.unload(id);
	}

	for (auto& [path, data] : _registeredAssets)
	{
		data.id = INVALID_UUID;
	}
}

bool registered_assets_sort(const Path& a, const Path& b) {
	// sort based on AssetType
	AssetType aType = Asset::get_type(a);
	AssetType bType = Asset::get_type(b);

	if (aType == bType)
	{
		// if the same type, sort alphabetically
		return a < b;
	}
	else
	{
		// if different types, sort by type
		return aType < bType;
	}
}

void minty::Scene::sort_registered_assets()
{
	// sort the paths
	std::sort(_unloadedAssets.begin(), _unloadedAssets.end(), registered_assets_sort);

	// update registered assets indices
	update_registered_indices();
}

void minty::Scene::update_registered_indices()
{
	for (size_t i = 0; i < _unloadedAssets.size(); i++)
	{
		_registeredAssets.at(_unloadedAssets.at(i)).index = i;
	}
}

void minty::Scene::serialize(Writer& writer) const
{
	writer.write("assets", _unloadedAssets);
	writer.write("systems", *_systems);
	writer.write("entities", *_entities);
}

void minty::Scene::deserialize(Reader const& reader)
{
	_unloadedAssets.clear();
	_registeredAssets.clear();

	reader.read_vector("assets", _unloadedAssets);
	reader.read_serializable("systems", *_systems);

	// add all assets to registered list
	_registeredAssets.reserve(_unloadedAssets.size());
	for (size_t i = 0; i < _unloadedAssets.size(); i++)
	{
		_registeredAssets.emplace(_unloadedAssets.at(i), AssetData{
			.index = i,
			.id = INVALID_UUID,
			});
	}

	// sort for good measure
	sort_registered_assets();

	// this is done in the load function
	//reader.read_serializable("entities", *_entities);
}

String minty::to_string(Scene const& value)
{
	return std::format("Scene({}, {})", to_string(*value._systems), to_string(*value._entities));
}
