#include "pch.h"
#include "M_Scene.h"

#include "M_Runtime.h"
#include "M_AssetEngine.h"
#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"

#include "M_TransformComponent.h"
#include "M_RelationshipComponent.h"
#include "M_DirtyComponent.h"
#include "M_CameraComponent.h"
#include "M_RenderSystem.h"
#include "M_Reader.h"
#include "M_Writer.h"
#include "M_SerializationData.h"

using namespace minty;

minty::Scene::Scene(SceneBuilder const& builder, Runtime& engine)
	: Asset(builder.id, builder.path, engine)
	, _entities(new EntityRegistry(engine, *this))
	, _systems(new SystemRegistry(engine, *this))
	, _loaded()
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
{}

Scene& minty::Scene::operator=(Scene&& other) noexcept
{
	if (this != &other)
	{
		_entities = std::move(other._entities);
		_systems = std::move(other._systems);
		_loaded = std::move(other._loaded);
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
	_loaded = true;
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
			RelationshipComponent const& leftRelationship = er->get<RelationshipComponent>(left);
			RelationshipComponent const& rightRelationship = er->get<RelationshipComponent>(right);

			return
				rightRelationship.parent == left || // put parents on left of children
				leftRelationship.next == right || // put siblings in order
				// put in order based on parent sibling index
				((leftRelationship.parent != right && rightRelationship.next != left) && (leftRelationship.parent < rightRelationship.parent || (leftRelationship.parent == rightRelationship.parent && left < right)));
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
	_loaded = false;
	_systems->unload();
	unload_registered_assets();
}

void minty::Scene::finalize()
{
	// update all the dirty tags
	
	// sort the hierarchy
	sort();

	// update dirty transform group with relationships
	auto view = _entities->view<DirtyComponent const, TransformComponent, RelationshipComponent>();
	view.use<RelationshipComponent const>();
	for (auto [entity, dirty, transform, relationship] : view.each())
	{
		// if parent, apply local to parent global for this global
		// if no parent, set global to local
		if (relationship.parent != NULL_ENTITY)
		{
			// parent

			// get parent Transform
			TransformComponent& parentTransform = _entities->get<TransformComponent>(relationship.parent);

			transform.globalMatrix = parentTransform.globalMatrix * transform.get_local_matrix();
		}
		else
		{
			// no parent
			transform.globalMatrix = transform.get_local_matrix();
		}
	}
	
	// update dirty transform group with no relationships
	for (auto [entity, dirty, transform] : _entities->view<DirtyComponent const, TransformComponent>(entt::exclude<RelationshipComponent>).each())
	{
		// no parent, set to self matrix
		transform.globalMatrix = transform.get_local_matrix();
	}

	// remove all dirty tags
	_entities->clear<DirtyComponent>();

	// destroy all entities tagged with the destroy tag
	_entities->destroy_queued();
}

void minty::Scene::load_registered_assets()
{
	// load each asset into the engine and save its ID so it can be unloaded later
	AssetEngine& assets = get_runtime().get_asset_engine();

	for (auto const& path : _unloadedAssets)
	{
		if (Asset* asset = assets.load_asset(path))
		{
			_loadedAssets.emplace(asset->get_id());
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

	reader.read_vector("assets", _unloadedAssets);
	reader.read_serializable("systems", *_systems);

	// this is done in the load function
	//reader.read_serializable("entities", *_entities);
}

String minty::to_string(Scene const& value)
{
	return std::format("Scene({}, {})", to_string(*value._systems), to_string(*value._entities));
}
