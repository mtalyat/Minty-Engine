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

struct SpriteBatch
{
	Ref<Material> material = nullptr;
	Byte* data = nullptr;
	Size count = 0;
};

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
	// pack them into a instance array
	Size const instanceDataSize = sizeof(Float4) + sizeof(Float2) * 3 + sizeof(Float) + sizeof(Matrix4);
#if defined(MINTY_DEBUG)
	Size count = 0;
	for (auto&& [entity, renderable, transform, sprite, enabled] : spriteView.each())
	{
		count += 1;
	}

	if (count != spriteView.size_hint())
	{
		MINTY_ERROR_FORMAT("Estimated sprite count ({}) is not the same as the actual sprite count ({}).", spriteView.size_hint(), count);
	}
#else
	Size count = spriteView.size_hint();
#endif // MINTY_DEBUG

	// ignore if no sprites to draw
	if (count)
	{
		Size offset = 0;
		Size dataSize = count * instanceDataSize;

		std::vector<SpriteBatch> batches;
		batches.reserve(10);
		SpriteBatch batch{};

		for (auto const& [entity, renderable, transform, spriteComp, enabled] : spriteView.each())
		{
			Ref<Sprite> sprite = spriteComp.sprite;

			// skip if no sprite to draw
			if (sprite == nullptr)
			{
				continue;
			}

			material = sprite->get_material();

			// if a new material, queue up batch and reset
			if (batch.material != material)
			{
				if (batch.material != nullptr)
				{
					batches.push_back(batch);
				}
				batch.material = sprite->get_material();
				batch.data = new Byte[dataSize];
				offset = 0;
			}

			Float4 instColor = spriteComp.color.toFloat4();
			Float2 instOffset = sprite->get_offset();
			Float2 instSize = sprite->get_size();
			Float2 instPivot = sprite->get_pivot();
			Float instScale = sprite->get_scale();
			Float4 instTransform0 = transform.globalMatrix[0];
			Float4 instTransform1 = transform.globalMatrix[1];
			Float4 instTransform2 = transform.globalMatrix[2];
			Float4 instTransform3 = transform.globalMatrix[3];

			// pack all of the data into the instance array
			pack_into(batch.data, offset, instColor);
			pack_into(batch.data, offset, instOffset);
			pack_into(batch.data, offset, instSize);
			pack_into(batch.data, offset, instPivot);
			pack_into(batch.data, offset, instScale);
			pack_into(batch.data, offset, instTransform0);
			pack_into(batch.data, offset, instTransform1);
			pack_into(batch.data, offset, instTransform2);
			pack_into(batch.data, offset, instTransform3);

			batch.count += 1;
		}

		// pack last batch into queue as long as it has data
		if (batch.material != nullptr)
		{
			batches.push_back(batch);
		}

		// render the batches
		for (auto const& batch : batches)
		{
			// bind batch
			Ref<MaterialTemplate> materialTemplate = batch.material->get_template();
			Ref<Shader> shader = materialTemplate->get_shader();
			Renderer::bind_shader(shader);
			Renderer::bind_material(batch.material);

			// update the instanced container with the data
			m_instanceContainer.set(batch.data, batch.count * instanceDataSize);
			Renderer::bind_vertex_buffer(m_instanceContainer.get_buffer());

			// draw the sprites
			Renderer::draw_instances(static_cast<UInt>(batch.count), 6); // 6 vertices per sprite, generated in the shader

			// delete data
			delete[] batch.data;
		}
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
