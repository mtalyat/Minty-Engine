#include "pch.h"
#include "Scene.h"

#include "Minty/Asset/AssetManager.h"
#include "Minty/Component/AllComponents.h"
#include "Minty/Core/Application.h"
#include "Minty/Window/Window.h"
#include "Minty/Asset/AssetManager.h"
#include "Minty/Entity/EntityRegistry.h"
#include "Minty/System/AllSystems.h"
#include "Minty/System/SystemRegistry.h"

using namespace Minty;

Minty::Scene::Scene(SceneBuilder const& builder)
	: Asset(builder.id)
	, mp_entityRegistry(new EntityRegistry(*this))
	, mp_systemRegistry(new SystemRegistry(*this))
	, m_loaded()
	, m_registeredAssets()
	, m_assets()
	, m_loadedAssets()
{
	// register all given asset paths
	for (auto const& path : builder.assets)
	{
		register_asset(path);
	}
}

Minty::Scene::~Scene()
{
	delete mp_entityRegistry;
	delete mp_systemRegistry;
}

Minty::Scene::Scene(Scene&& other) noexcept
	: Asset(std::move(other))
	, mp_entityRegistry(std::move(other.mp_entityRegistry))
	, mp_systemRegistry(std::move(other.mp_systemRegistry))
	, m_loaded(std::move(other.m_loaded))
	, m_registeredAssets(std::move(other.m_registeredAssets))
	, m_assets(std::move(other.m_assets))
	, m_loadedAssets(std::move(other.m_loadedAssets))
{}

Scene& Minty::Scene::operator=(Scene&& other) noexcept
{
	if (this != &other)
	{
		mp_entityRegistry = std::move(other.mp_entityRegistry);
		mp_systemRegistry = std::move(other.mp_systemRegistry);
		m_loaded = std::move(other.m_loaded);
		m_registeredAssets = std::move(other.m_registeredAssets);
		m_assets = std::move(other.m_assets);
		m_loadedAssets = std::move(other.m_loadedAssets);
	}

	return *this;
}

void Minty::Scene::initialize()
{
	load_registered_assets();
}

void Minty::Scene::load()
{
	if (m_loaded) return;

	// mark as loaded
	m_loaded = true;

	mp_systemRegistry->load();

	// sort quick after loading in case entities are out of order, if the file was manually edited
	sort();

	// select camera if needed
	if (RenderSystem* renderSystem = mp_systemRegistry->find<RenderSystem>())
	{
		renderSystem->set_main_camera(mp_entityRegistry->find_by_type<CameraComponent>());
	}
}

void Minty::Scene::update(Time const& time)
{
	if (Application::instance().get_mode() == ApplicationMode::Normal)
	{
		// update systems
		mp_systemRegistry->update(time);
	}
	else
	{
		// only update render system, ignore the rest
		RenderSystem* renderSystem = mp_systemRegistry->find<RenderSystem>();

		if (renderSystem)
		{
			// if found, render
			renderSystem->update(time);
		}
	}
}

void Minty::Scene::sort()
{
	EntityRegistry const* er = mp_entityRegistry;

	// compute ancestors
	std::unordered_map<Entity, std::pair<std::unordered_set<Entity>, std::vector<Entity>>> ancestors;
	for (auto const& [entity, relationship] : mp_entityRegistry->view<RelationshipComponent const>().each())
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
			ancestor = mp_entityRegistry->get_parent(ancestor);
		}
	}

	// sort relationships
	mp_entityRegistry->sort<RelationshipComponent>([er, &ancestors](Entity const left, Entity const right)
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

void Minty::Scene::fixed_update(Time const& time)
{
	if (Application::instance().get_mode() == ApplicationMode::Normal)
	{
		// update systems
		mp_systemRegistry->fixed_update(time);

		// TODO: update transforms...
	}
}

void Minty::Scene::unload()
{
	if (!m_loaded) return;

	m_loaded = false;

	mp_systemRegistry->unload();
	mp_entityRegistry->clear();
}

void Minty::Scene::shutdown()
{
	unload_registered_assets();
}

void Minty::Scene::finalize()
{
	// update all the dirty tags

	// sort the hierarchy
	sort();

	// update dirty transform group with relationships
	auto view = mp_entityRegistry->view<DirtyComponent const, TransformComponent, RelationshipComponent const>();
	view.use<RelationshipComponent const>();
	for (auto [entity, dirty, transform, relationship] : view.each())
	{
		// if parent, apply local to parent global for this global
		// if no parent, set global to local
		if (relationship.parent != NULL_ENTITY)
		{
			// parent

			// get parent Transform, if any
			if (TransformComponent* parentTransform = mp_entityRegistry->try_get<TransformComponent>(relationship.parent))
			{
				transform.globalMatrix = parentTransform->globalMatrix * transform.get_local_matrix();

				continue;
			}
		}

		// no parent
		transform.globalMatrix = transform.get_local_matrix();
	}

	// update dirty transform group with no relationships
	for (auto [entity, dirty, transform] : mp_entityRegistry->view<DirtyComponent const, TransformComponent>(entt::exclude<RelationshipComponent>).each())
	{
		// no parent, set to self matrix
		transform.globalMatrix = transform.get_local_matrix();
	}

	// update dirty UI transforms
	auto uiView = mp_entityRegistry->view<DirtyComponent const, UITransformComponent, RelationshipComponent const>();
	uiView.use<RelationshipComponent const>();
	for (auto [entity, dirty, transform, relationship] : uiView.each())
	{
		// if parent, apply local to parent global for this global
		// if no parent, set global to local
		UITransformComponent* parentTransform = mp_entityRegistry->try_get<UITransformComponent>(relationship.parent);
		if (parentTransform)
		{
			// parent exists, use its global rect
			transform.update_global_rect(parentTransform->globalRect);
		}
		else if (CanvasComponent* canvas = mp_entityRegistry->try_get<CanvasComponent>(entity))
		{
			// entity is the canvas, use that
			transform.update_global_rect(canvas->to_rect());
		}
		else
		{
			// no parent, not a canvas
			// set to not draw
			transform.globalRect = Rect();
		}
	}

	Ref<Window> window = WindowManager::get_main();
	Rect windowRect(0.0f, 0.0f, static_cast<Float>(window->get_frame_width()), static_cast<Float>(window->get_frame_height()));

	// update dirty UI transform group with no relationships
	for (auto [entity, dirty, transform] : mp_entityRegistry->view<DirtyComponent const, UITransformComponent>(entt::exclude<RelationshipComponent>).each())
	{
		// no parent, set to self matrix
		transform.update_global_rect(windowRect);
	}

	// remove all dirty tags
	mp_entityRegistry->clear<DirtyComponent>();

	// finalize systems
	mp_systemRegistry->finalize();

	// destroy all entities tagged with the destroy tag
	mp_entityRegistry->destroy_queued();
}

void Minty::Scene::draw()
{
	mp_systemRegistry->draw();
}

Bool Minty::Scene::register_asset(Path const& path)
{
	MINTY_ASSERT_FORMAT(!m_registeredAssets.contains(path), "Object already registered: \"{}\".", path.generic_string());

	AssetData data
	{
		.index = m_assets.size(),
		.id = INVALID_UUID,
	};

	// load asset if needed
	if (m_loaded)
	{
		Ref<Asset> asset = AssetManager::load_asset(path);

		if (asset == nullptr)
		{
			// did not load the asset: do not register
			return false;
		}

		m_loadedAssets.emplace(asset->id());
		data.id = asset->id();
	}

	m_assets.push_back(path);

	m_registeredAssets.emplace(path, data);

	return true;
}

void Minty::Scene::unregister_asset(Path const& path)
{
	MINTY_ASSERT(m_registeredAssets.contains(path));

	AssetData& data = m_registeredAssets.at(path);

	// unload asset if needed
	if (m_loaded)
	{
		if (data.id.valid())
		{
			AssetManager::unload(data.id);
			m_loadedAssets.erase(data.id);
		}
	}

	m_assets.erase(m_assets.begin() + data.index);
	m_registeredAssets.erase(path);

	// update indices since an asset was removed in the middle
	update_registered_indices();
}

Bool Minty::Scene::is_registered(Path const& assetPath)
{
	return m_registeredAssets.contains(assetPath);
}

void Minty::Scene::load_registered_assets()
{
	// load each asset into the engine and save its ID so it can be unloaded later
	for (auto const& path : m_assets)
	{
		Ref<Asset> asset = AssetManager::load_asset(path);
		if (asset.get())
		{
			AssetData& data = m_registeredAssets.at(path);

			m_loadedAssets.emplace(asset->id());
			data.id = asset->id();
		}
		else
		{
			MINTY_ERROR_FORMAT("Failed to load Object registered with Scene at \"{}\".", path.generic_string());
		}
	}
}

void Minty::Scene::unload_registered_assets()
{
	// unload each asset from the engine
	for (auto const id : m_loadedAssets)
	{
		AssetManager::unload(id);
	}

	m_loadedAssets.clear();

	for (auto& [path, data] : m_registeredAssets)
	{
		data.id = INVALID_UUID;
	}
}

void Minty::Scene::update_registered_indices()
{
	for (Size i = 0; i < m_assets.size(); i++)
	{
		m_registeredAssets.at(m_assets.at(i)).index = i;
	}
}

void Minty::Scene::serialize(Writer& writer) const
{
	// write assts
	writer.write("Assets", m_assets);

	// write systems
	writer.write("Systems", *mp_systemRegistry);
	
	// write entities
	writer.write("Entities", *mp_entityRegistry);
}

void Minty::Scene::deserialize(Reader& reader)
{
	MINTY_ABORT("Scene deserialize not implemented. Use AssetManager::load_scene().");
}

Owner<Scene> Minty::Scene::create(SceneBuilder const& builder)
{
	return Owner<Scene>(builder);
}
