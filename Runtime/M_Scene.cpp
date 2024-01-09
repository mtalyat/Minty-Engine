#include "pch.h"
#include "M_Scene.h"

#include "M_Engine.h"

#include "M_TransformComponent.h"
#include "M_RelationshipComponent.h"
#include "M_DirtyComponent.h"

using namespace minty;

minty::Scene::Scene(Engine* const engine)
	: _engine(engine)
	, _entities(new EntityRegistry())
	, _systems(new SystemRegistry(engine, _entities))
{}

minty::Scene::~Scene()
{
	delete _entities;
	_entities = nullptr;
	delete _systems;
	_systems = nullptr;
}

minty::Scene::Scene(Scene&& other) noexcept
	: _engine(other._engine)
	, _entities(other._entities)
	, _systems(other._systems)
{
	other._engine = nullptr;
	other._entities = nullptr;
	other._systems = nullptr;
}

Scene& minty::Scene::operator=(Scene&& other) noexcept
{
	if (this != &other)
	{
		_engine = other._engine;
		_entities = other._entities;
		_systems = other._systems;

		other._engine = nullptr;
		other._entities = nullptr;
		other._systems = nullptr;
	}

	return *this;
}

Engine* minty::Scene::get_engine() const
{
	return _engine;
}

EntityRegistry* minty::Scene::get_entity_registry() const
{
	return _entities;
}

SystemRegistry* minty::Scene::get_system_registry() const
{
	return _systems;
}

void minty::Scene::load()
{
	_systems->load();
}

void minty::Scene::update()
{
	// update systems
	_systems->update();

	EntityRegistry const* er = _entities;

	// TODO: move to fixed_update
	// TODO: use group, only sort dirty components
	_entities->sort<TransformComponent>([er](Entity const left, Entity const right)
		{
			// get relationships
			RelationshipComponent const* leftRelationship = er->try_get<RelationshipComponent>(left);
			RelationshipComponent const* rightRelationship = er->try_get<RelationshipComponent>(right);

			if (leftRelationship)
			{
				if (rightRelationship)
				{
					// both exist
					return
						rightRelationship->parent == left || // put parents on left of children
						leftRelationship->next == right || // put siblings in order
						// put in order based on parent sibling index
						((leftRelationship->parent != right && rightRelationship->next != left) && (leftRelationship->parent < rightRelationship->parent || (leftRelationship->parent == rightRelationship->parent && left < right)));
				}
				else
				{
					// right dne
					return
						leftRelationship->next == right ||
						(leftRelationship->parent != right && leftRelationship->parent == NULL_ENTITY && left < right);
				}
			}
			else
			{
				if (rightRelationship)
				{
					// left dne
					return
						rightRelationship->parent == left ||
						(rightRelationship->next != left && (rightRelationship->parent != NULL_ENTITY || left < right));
				}
				else
				{
					// both dne
					// compare entity ID values
					return left < right;
				}
			}
		});

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
				transform.global = parentTransform->global * transform.local.get_matrix();

				continue;
			}

			// if no transform on parent, treat as if no parent
		}

		// no parent
		transform.global = transform.local.get_matrix();
	}
}

void minty::Scene::fixed_update()
{
	// update systems
	_systems->fixed_update();

	// update transforms
}

void minty::Scene::unload()
{
	_systems->unload();
}

void minty::Scene::finalize()
{
	// remove all dirty tags
	_entities->clear<DirtyComponent>();
}

void minty::Scene::serialize(Writer& writer) const
{
	writer.write("Systems", _systems);
	writer.write("Entities", _entities);
}

void minty::Scene::deserialize(Reader const& reader)
{
	reader.read_object("Systems", _systems);
	reader.read_object("Entities", _entities);
}

String minty::to_string(Scene const& value)
{
	return std::format("Scene({}, {})", to_string(*value._systems), to_string(*value._entities));
}
