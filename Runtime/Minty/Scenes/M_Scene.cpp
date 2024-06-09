#include "pch.h"
#include "Minty/Scenes/M_Scene.h"

#include "Minty/Core/M_Application.h"
#include "Minty/Core/M_Window.h"
#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Entities/M_EntityRegistry.h"
#include "Minty/Systems/M_SystemRegistry.h"

#include "Minty/Components/M_TransformComponent.h"
#include "Minty/UI/M_UITransformComponent.h"
#include "Minty/UI/M_CanvasComponent.h"
#include "Minty/Components/M_RelationshipComponent.h"
#include "Minty/Rendering/M_SpriteComponent.h"
#include "Minty/Components/M_DirtyComponent.h"
#include "Minty/Rendering/M_CameraComponent.h"
#include "Minty/Rendering/M_RenderSystem.h"
#include "Minty/Scripting/M_ScriptComponent.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"
#include "Minty/Serialization/M_SerializationData.h"

using namespace Minty;

Minty::Scene::Scene(SceneBuilder const& builder)
	: Asset(builder.id, builder.path)
	, _entities(new EntityRegistry(*this))
	, _systems(new SystemRegistry(*this))
	, _loaded()
	, _registeredAssets()
	, _unloadedAssets()
	, _loadedAssets()
{}

Minty::Scene::~Scene()
{
	delete _entities;
	delete _systems;
}

Minty::Scene::Scene(Scene&& other) noexcept
	: Asset(std::move(other))
	, _entities(std::move(other._entities))
	, _systems(std::move(other._systems))
	, _loaded(std::move(other._loaded))
	, _registeredAssets(std::move(other._registeredAssets))
	, _unloadedAssets(std::move(other._unloadedAssets))
	, _loadedAssets(std::move(other._loadedAssets))
{}

Scene& Minty::Scene::operator=(Scene&& other) noexcept
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

EntityRegistry& Minty::Scene::get_entity_registry() const
{
	return *_entities;
}

SystemRegistry& Minty::Scene::get_system_registry() const
{
	return *_systems;
}

bool Minty::Scene::is_loaded() const
{
	return _loaded;
}

void Minty::Scene::load()
{
	load_registered_assets();
	_systems->load();

	AssetEngine& assets = AssetEngine::instance();

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
		renderSystem->set_camera(_entities->find_by_type<CameraComponent>());
	}

	// finally mark as loaded
	_loaded = true;

	// call all load/enable/etc. events on entities already in the system
	for (auto&& [entity, script, onLoad] : _entities->view<ScriptComponent const, ScriptOnLoadComponent const>().each())
	{
		onLoad.invoke(script);
	}

	for (auto&& [entity, script, onEnable] : _entities->view<ScriptComponent const, ScriptOnEnableComponent const>().each())
	{
		onEnable.invoke(script);
	}
}

void Minty::Scene::update(Time const time)
{
	if (Application::instance().get_mode() == ApplicationMode::Normal)
	{
		// update systems
		_systems->update(time);
	}
}

void Minty::Scene::sort()
{
	EntityRegistry const* er = _entities;

	// compute ancestors
	std::unordered_map<Entity, std::pair<std::unordered_set<Entity>, std::vector<Entity>>> ancestors;
	for (auto const& [entity, relationship] : _entities->view<RelationshipComponent const>().each())
	{
		ancestors.emplace(entity, std::pair{ std::unordered_set<Entity>(), std::vector<Entity>{ entity } });

		// skip if no parent
		if (relationship.parent == NULL_ENTITY) continue;

		// add parent
		std::unordered_set<Entity>& set = ancestors.at(entity).first;
		std::vector<Entity>& list = ancestors.at(entity).second;

		// all all ancestors to set
		Entity ancestor = relationship.parent;
		while (ancestor != NULL_ENTITY)
		{
			set.emplace(ancestor);
			list.push_back(ancestor);

			// get next parent up
			ancestor = _entities->get_parent(ancestor);
		}
	}

	// sort relationships
	_entities->sort<RelationshipComponent>([er, &ancestors](Entity const left, Entity const right)
		{
			if (ancestors.at(right).first.contains(left))
			{
				// right is an ancestor of left
				return true;
			}
			else if (ancestors.at(left).first.contains(right))
			{
				// left is an ancestor of right
				return false;
			}

			// check ancestors if they are related
			for (Entity eLeft : ancestors.at(left).second)
			{
				RelationshipComponent const& eLeftRelationship = er->get<RelationshipComponent>(eLeft);
				for (Entity eRight : ancestors.at(right).second)
				{
					RelationshipComponent const& eRightRelationship = er->get<RelationshipComponent>(eRight);

					if (eLeftRelationship.parent == eRightRelationship.parent)
					{
						// if null, go by ID value, if not null, go by sibling index
						// if null, all sibling indices are zero, so sorting breaks
						if (eLeftRelationship.parent == NULL_ENTITY)
						{
							return eLeft < eRight;
						}
						else
						{
							return eLeftRelationship.index < eRightRelationship.index;
						}
					}
				}
			}

			// default ordering: should not be here
			MINTY_WARN_FORMAT("Scene sort: Used default sort for {} and {}.", er->get_name(left), er->get_name(right));
			return left < right;
		});
}

void Minty::Scene::fixed_update(Time const time)
{
	if (Application::instance().get_mode() == ApplicationMode::Normal)
	{
		// update systems
		_systems->fixed_update(time);

		// TODO: update transforms...
	}
}

void Minty::Scene::unload()
{
	_systems->unload();
	unload_registered_assets();

	_loaded = false;
}

void Minty::Scene::finalize()
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

	Window& window = Window::main();
	RectF windowRect(0, 0, window.get_frame_width(), window.get_frame_height());

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
		else if (CanvasComponent* canvas = _entities->try_get<CanvasComponent>(entity))
		{
			// canvas
			transform.update_global_rect(canvas->toRect());
		}
		else
		{
			// no parent, not a canvas

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

void Minty::Scene::register_asset(Path const& path)
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
		AssetEngine& assets = AssetEngine::instance();

		if (Ref<Asset> asset = assets.load_asset(path))
		{
			_loadedAssets.emplace(asset->get_id());
			data.id = asset->get_id();
		}
	}

	_registeredAssets.emplace(path, data);

	// sort all since a new thing was added
	sort_registered_assets();
}

void Minty::Scene::unregister_asset(Path const& path)
{
	MINTY_ASSERT(_registeredAssets.contains(path));

	AssetEngine& assets = AssetEngine::instance();

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

bool Minty::Scene::is_registered(Path const& assetPath)
{
	return _registeredAssets.contains(assetPath);
}

void Minty::Scene::load_registered_assets()
{
	// load each asset into the engine and save its ID so it can be unloaded later
	AssetEngine& assets = AssetEngine::instance();

	for (auto const& path : _unloadedAssets)
	{
		if (Ref<Asset> asset = assets.load_asset(path))
		{
			AssetData& data = _registeredAssets.at(path);

			_loadedAssets.emplace(asset->get_id());
			data.id = asset->get_id();
		}
	}
}

void Minty::Scene::unload_registered_assets()
{
	// unload each asset from the engine
	AssetEngine& assets = AssetEngine::instance();

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

void Minty::Scene::sort_registered_assets()
{
	// sort the paths
	std::sort(_unloadedAssets.begin(), _unloadedAssets.end(), registered_assets_sort);

	// update registered assets indices
	update_registered_indices();
}

void Minty::Scene::update_registered_indices()
{
	for (size_t i = 0; i < _unloadedAssets.size(); i++)
	{
		_registeredAssets.at(_unloadedAssets.at(i)).index = i;
	}
}

void Minty::Scene::serialize(Writer& writer) const
{
	writer.write("assets", _unloadedAssets);
	writer.write("systems", *_systems);
	writer.write("entities", *_entities);
}

void Minty::Scene::deserialize(Reader const& reader)
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

String Minty::to_string(Scene const& value)
{
	return std::format("Scene({}, {})", to_string(*value._systems), to_string(*value._entities));
}
