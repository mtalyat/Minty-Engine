#include "pch.h"
#include "M_Scene.h"

#include "M_Runtime.h"
#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"

#include "M_TransformComponent.h"
#include "M_RelationshipComponent.h"
#include "M_DirtyComponent.h"
#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

minty::Scene::Scene(Runtime& engine, ID const sceneId)
	: _id(sceneId)
	, _engine(&engine)
	, _entities(new EntityRegistry(engine, sceneId))
	, _systems(new SystemRegistry(engine, sceneId))
	, _loaded()
{}

minty::Scene::~Scene()
{
	delete _entities;
	delete _systems;
}

minty::Scene::Scene(Scene&& other) noexcept
	: _id(other._id)
	, _engine(other._engine)
	, _entities(other._entities)
	, _systems(other._systems)
{
	other._id = ERROR_ID;
	other._engine = nullptr;
	other._entities = nullptr;
	other._systems = nullptr;
}

Scene& minty::Scene::operator=(Scene&& other) noexcept
{
	if (this != &other)
	{
		_id = other._id;
		_engine = other._engine;
		_entities = other._entities;
		_systems = other._systems;

		other._id = ERROR_ID;
		other._engine = nullptr;
		other._entities = nullptr;
		other._systems = nullptr;
	}

	return *this;
}

Runtime& minty::Scene::get_runtime() const
{
	return *_engine;
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
	if (get_runtime().get_mode() == Runtime::Mode::Normal)
	{
		_systems->load();
	}
}

void minty::Scene::update()
{
	if (get_runtime().get_mode() == Runtime::Mode::Normal)
	{
		// update systems
		_systems->update();
	}

	EntityRegistry const* er = _entities;

	// TODO: move to fixed_update
	// TODO: use group, only sort dirty components

	// sort the hierarchy
	sort();

	// update group
	for (auto&& [entity, dirty, transform] : _entities->view<DirtyComponent const, TransformComponent>().each())
	{
		// get relationship, if there is one
		RelationshipComponent const* relationshipComponent = er->try_get<RelationshipComponent>(entity);

		// if parent, apply local to parent global for this global
		// if no parent, set global to local
		if (relationshipComponent && relationshipComponent->parent != NULL_ENTITY)
		{
			// parent

			// get parent Transform
			TransformComponent const* parentTransform = er->try_get<TransformComponent>(relationshipComponent->parent);

			if (parentTransform)
			{
				transform.globalMatrix = parentTransform->globalMatrix * transform.get_local_matrix();

				continue;
			}

			// if no transform on parent, treat as if no parent
		}

		// no parent
		transform.globalMatrix = transform.get_local_matrix();
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

			//// get relationships
			//RelationshipComponent const* leftRelationship = er->try_get<RelationshipComponent>(left);
			//RelationshipComponent const* rightRelationship = er->try_get<RelationshipComponent>(right);

			//if (leftRelationship)
			//{
			//	if (rightRelationship)
			//	{
			//		// both exist
			//		return
			//			rightRelationship->parent == left || // put parents on left of children
			//			leftRelationship->next == right || // put siblings in order
			//			// put in order based on parent sibling index
			//			((leftRelationship->parent != right && rightRelationship->next != left) && (leftRelationship->parent < rightRelationship->parent || (leftRelationship->parent == rightRelationship->parent && left < right)));
			//	}
			//	else
			//	{
			//		// right dne
			//		return true;
			//		//return
			//		//	leftRelationship->next == right ||
			//		//	(leftRelationship->parent != right && leftRelationship->parent == NULL_ENTITY && left < right);
			//	}
			//}
			//else
			//{
			//	if (rightRelationship)
			//	{
			//		// left dne
			//		return false;
			//		//return
			//		//	rightRelationship->parent == left ||
			//		//	(rightRelationship->next != left && (rightRelationship->parent != NULL_ENTITY || left < right));
			//	}
			//	else
			//	{
			//		// both dne
			//		// compare entity ID values
			//		return left < right;
			//	}
			//}
		});
}

void minty::Scene::fixed_update()
{
	if (get_runtime().get_mode() == Runtime::Mode::Normal)
	{
		// update systems
		_systems->fixed_update();

		// update transforms
	}
}

void minty::Scene::unload()
{
	_loaded = false;
	if (get_runtime().get_mode() == Runtime::Mode::Normal)
	{
		_systems->unload();
	}
}

void minty::Scene::finalize()
{
	// remove all dirty tags
	_entities->clear<DirtyComponent>();

	// destroy all entities tagged with the destroy tag
	_entities->destroy_queued();
}

void minty::Scene::serialize(Writer& writer) const
{
	writer.write("systems", *_systems);
	writer.write("entities", *_entities);
}

void minty::Scene::deserialize(Reader const& reader)
{
	reader.read_serializable("systems", *_systems);
	reader.read_serializable("entities", *_entities);

	// sort quick after deserialization in case entities are out of order, if the file was manually edited
	sort();
}

String minty::to_string(Scene const& value)
{
	return std::format("Scene({}, {})", to_string(*value._systems), to_string(*value._entities));
}
