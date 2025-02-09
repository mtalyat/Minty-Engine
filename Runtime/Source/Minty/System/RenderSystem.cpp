#include "pch.h"
#include "RenderSystem.h"

#include "Minty/Asset/Asset.h"
#include "Minty/Asset/AssetManager.h"
#include "Minty/Component/AllComponents.h"
#include "Minty/Core/Application.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/Pack.h"
#include "Minty/Data/BatchFactory.h"
#include "Minty/Entity/EntityRegistry.h"
#include "Minty/Library/GLM.h"
#include "Minty/Render/Renderer.h"

using namespace Minty;

void Minty::RenderSystem::finalize()
{
	EntityRegistry& entityRegistry = get_entity_registry();

	// update all dirty text
	for (auto&& [entity, text] : entityRegistry.view<TextComponent>().each())
	{
		text.try_regenerate_mesh();
	}
}

void Minty::RenderSystem::draw()
{
	// update global constants provided by the engine
	Int frameIndex = static_cast<Int>(Renderer::get_current_frame_index());
	for (Ref<Shader> const& shader : AssetManager::get_by_type<Shader>())
	{
		if (shader->has_input("index"))
		{
			shader->set_global_input("index", &frameIndex);
		}
	}

	// draw for each enabled camera
	if (Application::instance().get_mode() == ApplicationMode::Edit)
	{
		// draw from editor camera
		Renderer::start_render_pass(Renderer::get_render_pass(), Renderer::get_render_target());

		// assume camera data has already been set
		
		// draw scene
		draw_scene();

		Renderer::end_render_pass();
	}
	else
	{
		// draw from cameras
		EntityRegistry& entityRegistry = get_entity_registry();

		bool drawn = false;

		for (auto const& [entity, camera, enabled] : entityRegistry.view<CameraComponent, EnabledComponent const>().each())
		{
			Ref<RenderTarget> renderTarget = camera.camera.get_render_target();

			// if no render target, use default render target
			if (renderTarget == nullptr)
			{
				renderTarget = Renderer::get_render_target();
			}

			// start render pass
			Renderer::start_render_pass(Renderer::get_render_pass(), renderTarget);

			// set camera orientation data
			TransformComponent const* transformComponent = entityRegistry.try_get<TransformComponent>(entity);
			if (transformComponent)
			{
				// use transform component
				update_camera(camera, *transformComponent);
			}
			else
			{
				// fake an empty transform component at origin
				TransformComponent temp = TransformComponent::create_empty();
				update_camera(camera, temp);
			}

			// draw scene
			draw_scene();

			Renderer::end_render_pass();

			drawn = true;
		}

		// show warning if nothing drawn
		if (!drawn)
		{
			MINTY_WARN("There is no Camera to render to.");
			return;
		}
	}
}

void Minty::RenderSystem::update_camera(CameraComponent const& camera, TransformComponent const& transform)
{
	Renderer::set_camera(transform.get_global_position(), transform.get_global_rotation(), camera.camera);
	m_layerMask = camera.camera.get_layer_mask();
}

void Minty::RenderSystem::draw_3d_meshes()
{
	// get camera transform
	EntityRegistry& entityRegistry = get_entity_registry();

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
		if (meshComp.type == MeshType::Empty)
		{
			continue;
		}

		// skip if no material or mesh
		if (meshComp.material == nullptr || meshComp.mesh == nullptr)
		{
			continue;
		}

		// skip if not part of camera layer mask
		if (!entityRegistry.is_in_layer_mask(entity, m_layerMask))
		{
			continue;
		}

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
}

void Minty::RenderSystem::draw_3d_sprites()
{
	// get camera transform
	EntityRegistry& entityRegistry = get_entity_registry();

	// draw all world sprites in the scene
	auto spriteView = entityRegistry.view<RenderableComponent const, TransformComponent const, SpriteComponent const, EnabledComponent const>();

	Size count = 0;
	for (auto&& [entity, renderable, transform, sprite, enabled] : spriteView.each())
	{
		count += 1;
	}

	// ignore if no sprites to draw
	if (count)
	{
		// pack them into a instance array
		Size const dataSize = sizeof(Float4) + sizeof(Float2) * 3 + sizeof(Float) + sizeof(Matrix4);
		Size const maxDataSize = count * dataSize;

		Ref<Material> material;

		BatchFactory<1, Ref<Material>> batchFactory(maxDataSize);

		for (auto const& [entity, renderable, transform, spriteComp, enabled] : spriteView.each())
		{
			Ref<Sprite> sprite = spriteComp.sprite;

			// skip if no sprite to draw
			if (sprite == nullptr)
			{
				continue;
			}

			// skip if not part of camera layer mask
			if (!entityRegistry.is_in_layer_mask(entity, m_layerMask))
			{
				continue;
			}

			// get the batch, based on the material
			Ref<Texture> spriteTexture = sprite->get_texture();
			material = Renderer::get_or_create_default_material(spriteTexture, AssetType::Sprite, Space::D3);
			Batch<1, Ref<Material>>& batch = batchFactory.get_or_create_batch({ material });

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
			DynamicContainer& batchContainer = batch.get_data_container();
			batchContainer.append_object(instColor);
			batchContainer.append_object(instOffset);
			batchContainer.append_object(instSize);
			batchContainer.append_object(instPivot);
			batchContainer.append_object(instScale);
			batchContainer.append_object(instTransform0);
			batchContainer.append_object(instTransform1);
			batchContainer.append_object(instTransform2);
			batchContainer.append_object(instTransform3);

			// one more item stored inside the batch
			batch.increment();
		}

		// render the batches
		Size index = 0;
		for (auto const& batch : batchFactory)
		{
			// bind batch
			Ref<Material> material = batch.get_object<Ref<Material>>(0);
			Ref<MaterialTemplate> materialTemplate = material->get_template();
			Ref<Shader> shader = materialTemplate->get_shader();
			Renderer::bind_shader(shader);
			Renderer::bind_material(material);

			// update the instanced container with the data
			BufferContainer& container = m_instanceCargo.get_container(m_3dSpriteGroupId, index);
			container.set(batch.get_data(), batch.get_data_size());
			Renderer::bind_vertex_buffer(container.get_buffer());

			// draw the sprites
			Renderer::draw_instances(static_cast<UInt>(batch.get_count()), 6); // 6 vertices per sprite, generated in the shader

			index++;
		}
	}
}

void Minty::RenderSystem::draw_3d()
{
	draw_3d_meshes();
	draw_3d_sprites();
}

void Minty::RenderSystem::draw_ui_sprites()
{
	EntityRegistry& entityRegistry = get_entity_registry();

	Ref<Material> material = nullptr;
	Ref<Shader> shader = nullptr;
	BatchFactory<2, Ref<Material>, Entity> batchFactory(256);

	for (auto const& [entity, ui, renderable, enabled, spriteComp] : entityRegistry.view<UITransformComponent const, RenderableComponent const, EnabledComponent const, SpriteComponent const>().each())
	{
		// skip if not part of camera layer mask
		if (!entityRegistry.is_in_layer_mask(entity, m_layerMask))
		{
			continue;
		}

		material = Renderer::get_or_create_default_material(spriteComp.sprite->get_texture(), AssetType::Sprite, Space::UI);

		MINTY_ASSERT(material != nullptr);
		shader = material->get_template()->get_shader();
		MINTY_ASSERT(shader != nullptr);

		auto& batch = batchFactory.get_or_create_batch({ material, ui.canvas });
		DynamicContainer& batchContainer = batch.get_data_container();

		// update canvas data in new shader
		if (batch.empty())
		{
			// update Canvas global constant
			CanvasComponent* canvas = entityRegistry.try_get<CanvasComponent>(ui.canvas);
			DynamicContainer canvasContainer(sizeof(Int) * 2);
			if (canvas)
			{
				canvasContainer.append_object(canvas->referenceResolution.x);
				canvasContainer.append_object(canvas->referenceResolution.y);
			}
			else
			{
				canvasContainer.append_object(0);
				canvasContainer.append_object(0);
			}
			shader->set_global_input("canvas", canvasContainer.data());
		}

		batchContainer.append_object(ui.globalRect.rect);
		batchContainer.append_object(spriteComp.sprite->get_uv());
		batchContainer.append_object(spriteComp.color.toFloat4());

		batch.increment();
	}

	// render each batch
	Size index = 0;
	for (auto const& batch : batchFactory)
	{
		// bind batch
		Ref<Material> material = batch.get_object<Ref<Material>>(0);
		Ref<MaterialTemplate> materialTemplate = material->get_template();
		Ref<Shader> shader = materialTemplate->get_shader();
		Renderer::bind_shader(shader);
		Renderer::bind_material(material);

		// update the instanced container with the data
		BufferContainer& container = m_instanceCargo.get_container(m_uiSpriteGroupId, index);
		container.set(batch.get_data(), batch.get_data_size());
		Renderer::bind_vertex_buffer(container.get_buffer());

		// draw the sprites
		Renderer::draw_instances(static_cast<UInt>(batch.get_count()), 6); // 6 vertices per sprite, generated in the shader

		index++;
	}
}

void Minty::RenderSystem::draw_ui_text()
{
	EntityRegistry& entityRegistry = get_entity_registry();

	Ref<Material> material = nullptr;
	Ref<Shader> shader = nullptr;

	BatchFactory<2, Ref<Material>, Entity> batchFactory(256);

	Byte pushData[sizeof(Float4) * 2];
	Size pushDataOffset;

	for (auto&& [entity, ui, renderable, enabled, textComp] : entityRegistry.view<UITransformComponent const, RenderableComponent const, EnabledComponent const, TextComponent const>().each())
	{
		// skip if null font or no mesh
		if (textComp.font == nullptr || textComp.fontVariant == nullptr || textComp.mesh == nullptr)
		{
			continue;
		}

		// skip if not part of camera layer mask
		if (!entityRegistry.is_in_layer_mask(entity, m_layerMask))
		{
			continue;
		}

		material = textComp.fontVariant->get_material();

		MINTY_ASSERT(material != nullptr);
		shader = material->get_template()->get_shader();
		MINTY_ASSERT(shader != nullptr);

		// update push constant info
		pushDataOffset = 0;
		pack_into<Float4>(pushData, pushDataOffset, ui.globalRect.rect);
		pack_into<Float4>(pushData, pushDataOffset, textComp.color.toFloat4());
		material->set_input("push", pushData);

		// bind shader and material
		Renderer::bind_shader(shader);
		Renderer::bind_material(material);

		// bind mesh
		Renderer::bind_mesh(textComp.mesh);

		// draw the mesh
		Renderer::draw(textComp.mesh);
	}
}

void Minty::RenderSystem::draw_ui()
{
	EntityRegistry& entityRegistry = get_entity_registry();

	// sort ui transforms so they render in the correct order, since z alone determines depth
	entityRegistry.sort<UITransformComponent>([](UITransformComponent const& left, UITransformComponent const& right)
		{
			return left.z < right.z;
		});

	draw_ui_sprites();
	draw_ui_text();
}

void Minty::RenderSystem::draw_scene()
{
	draw_3d();
	// draw_2d(); goes here
	draw_ui();
}
