#include "pch.h"
#include "RenderSystem.h"

#include "Minty/Asset/Asset.h"
#include "Minty/Asset/AssetManager.h"
#include "Minty/Component/AllComponents.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/Pack.h"
#include "Minty/Entity/EntityRegistry.h"
#include "Minty/Library/GLM.h"
#include "Minty/Render/Renderer.h"

using namespace Minty;

void Minty::RenderSystem::update(Time const& time)
{
	// do nothing if no camera
	if (m_camera == NULL_ENTITY)
	{
		MINTY_WARN("There is no Camera to render to.");
		return;
	}

	// get camera transform
	EntityRegistry& entityRegistry = get_entity_registry();

	if (!entityRegistry.all_of<CameraComponent>(m_camera))
	{
		MINTY_WARN("The Camera entity has no Camera component.");
		return;
	}

	// update camera in renderer if it is enabled
	if (entityRegistry.all_of<EnabledComponent>(m_camera))
	{
		CameraComponent const& camera = entityRegistry.get<CameraComponent>(m_camera);
		TransformComponent const* transformComponent = entityRegistry.try_get<TransformComponent>(m_camera);

		if (transformComponent)
		{
			update_camera(camera, *transformComponent);
		}
		else
		{
			// fake an empty transform component
			TransformComponent temp = TransformComponent::create_empty();
			update_camera(camera, temp);
		}
	}

	// draw all meshes
	TransformComponent const* transformComponent;
	Ref<Shader> shader;
	Ref<MaterialTemplate> materialTemplate;
	Ref<Material> material;
	Matrix4 transformation;

	// draw all meshes in the scene
	for (auto&& [entity, meshComp, renderable, enabled] : entityRegistry.view<MeshComponent, RenderableComponent const, EnabledComponent const>().each())
	{
		// skip empty meshes
		if (meshComp.type == MeshType::Empty) continue;

		// skip if no material
		if (meshComp.material == nullptr) continue;

		// get assets
		material = meshComp.material;
		materialTemplate = material->get_template();
		MINTY_ASSERT(materialTemplate != nullptr);
		shader = materialTemplate->get_shader();
		MINTY_ASSERT(shader != nullptr);

		// bind assets
		Renderer::bind_shader(shader);
		Renderer::bind_material(material);

		// get transform for entity
		transformComponent = entityRegistry.try_get<TransformComponent>(entity);

		if (transformComponent)
		{
			// render the entity's mesh at the position
			transformation = transformComponent->globalMatrix;
		}
		else
		{
			// if no transform, use empty matrix
			transformation = Matrix4(1.0f);
		}

		// set transform
		material->set_input("object", &transformation);

		// draw mesh
		Renderer::bind_mesh(meshComp.mesh);
		Renderer::draw(meshComp.mesh);
	}

	// draw all world sprites in the scene
	auto spriteView = entityRegistry.view<RenderableComponent const, TransformComponent const, SpriteComponent const, EnabledComponent const>();
	for (auto&& [entity, renderable, transform, sprite, enabled] : spriteView.each())
	{
		// set push data
		Float2 minCoords = sprite.sprite->get_offset();
		Float2 maxCoords = minCoords + sprite.sprite->get_size();

		Byte* data = pack_new(transform.globalMatrix, sprite.color, minCoords, maxCoords, sprite.sprite->get_pivot(), sprite.sprite->get_scale());

		// draw mesh
		Renderer::draw(sprite.sprite);
	}
}

void Minty::RenderSystem::finalize()
{
	//EntityRegistry& entityRegistry = get_entity_registry();

	//// update all text if dirty
	//for (auto&& [entity, text] : entityRegistry.view<TextComponent>().each())
	//{
	//	text.try_regenerate_mesh();
	//}
}

void Minty::RenderSystem::update_camera(CameraComponent const& camera, TransformComponent const& transform)
{
	Renderer::set_camera(transform.get_global_position(), transform.get_global_rotation(), camera.camera);
}
