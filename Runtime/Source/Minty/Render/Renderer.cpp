#include "pch.h"
#include "Renderer.h"

#include "Minty/Asset/AssetManager.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanRenderer.h"
#endif

using namespace Minty;

Ref<Window> Minty::Renderer::s_window = nullptr;
Color Minty::Renderer::s_color = Color::black();
std::vector<Owner<RenderPass>> Renderer::s_renderPasses;
std::unordered_map<MeshType, Ref<Mesh>> Minty::Renderer::s_defaultMeshes = {};
std::unordered_map <UInt, Ref<Material>> Minty::Renderer::s_defaultMaterials = {};
Ref<Shader> Minty::Renderer::s_boundShader = nullptr;
Ref<Material> Minty::Renderer::s_boundMaterial = nullptr;
Ref<Mesh> Minty::Renderer::s_boundMesh = nullptr;

void Minty::Renderer::initialize(RendererBuilder const& builder)
{
	MINTY_ASSERT(builder.window.get() != nullptr);

	s_window = builder.window;

	// initialize API
#if defined(MINTY_VULKAN)
	VulkanRenderer::initialize(builder);
#endif
}

void Minty::Renderer::shutdown()
{
	// destroy render passes
	s_renderPasses.clear();

	// shutdown API
#if defined(MINTY_VULKAN)
	VulkanRenderer::shutdown();
#endif

	s_window = nullptr;
}

Int Minty::Renderer::start_frame()
{
#if defined(MINTY_VULKAN)
	return VulkanRenderer::start_frame();
#endif
}

void Minty::Renderer::end_frame()
{
	// end frame
#if defined(MINTY_VULKAN)
	VulkanRenderer::end_frame();
#endif

	// unbind specific assets that need re-bound each frame
	s_boundMesh = nullptr;
	s_boundMaterial = nullptr;
	s_boundShader = nullptr;
}

void Minty::Renderer::set_camera(Float3 const position, Quaternion const rotation, Camera const& camera)
{
	set_color(camera.get_color());

	Matrix4 view = glm::lookAt(position, position + forward(rotation), Float3(0.0f, 1.0f, 0.0f));

	// TODO: don't use lookat
	// maybe invert global?

	// get projection
	Matrix4 proj;
	switch (camera.get_perspective())
	{
	case Perspective::Perspective:
		proj = glm::perspective(camera.get_fov(), camera.get_aspect_ratio(), camera.get_near(), camera.get_far());
		break;
	case Perspective::Orthographic:
	{
		float orthoHeight = camera.get_size();
		float orthoWidth = camera.get_size() * camera.get_aspect_ratio();

		float left = -orthoWidth * 0.5f;
		float right = orthoWidth * 0.5f;
		float bottom = -orthoHeight * 0.5f;
		float top = orthoHeight * 0.5f;

		proj = glm::ortho(left, right, bottom, top, camera.get_near(), camera.get_far());
		break;
	}
	default:
		MINTY_ERROR_FORMAT("Unrecognized Camera perspective: {}", static_cast<int>(camera.get_perspective()));
		return;
		break;
	}

	// multiply together
	Matrix4 transformMatrix = proj * view;

	// update all materials that have a camera
	for (auto const& material : AssetManager::get_by_type<Material>())
	{
		material->try_set_input("camera", &transformMatrix);
	}
}

void Minty::Renderer::sync()
{
#if defined(MINTY_VULKAN)
	VulkanRenderer::sync();
#endif
}

void Minty::Renderer::draw_vertices(UInt const vertexCount)
{
#if defined(MINTY_VULKAN)
	VulkanRenderer::draw_vertices(vertexCount);
#endif
}

void Minty::Renderer::draw_instances(UInt const instanceCount, UInt const vertexCount)
{
#if defined(MINTY_VULKAN)
	VulkanRenderer::draw_instances(instanceCount, vertexCount);
#endif
}

void Minty::Renderer::draw_indices(UInt const indexCount)
{
#if defined(MINTY_VULKAN)
	VulkanRenderer::draw_indices(indexCount);
#endif
}

void Minty::Renderer::refresh()
{
#if defined(MINTY_VULKAN)
	VulkanRenderer::refresh();
#endif
}

Ref<Viewport> Minty::Renderer::get_viewport()
{
#if defined(MINTY_VULKAN)
	return VulkanRenderer::get_viewport();
#endif
}

Ref<Scissor> Minty::Renderer::get_scissor()
{
#if defined(MINTY_VULKAN)
	return VulkanRenderer::get_scissor();
#endif
}

Ref<Mesh> Minty::Renderer::get_or_create_mesh(MeshType const type)
{
	// ignore custom
	if (type == MeshType::Custom)
	{
		return Ref<Mesh>();
	}

	// if mesh exists, return that
	auto found = s_defaultMeshes.find(type);

	if (found != s_defaultMeshes.end())
	{
		return found->second;
	}

	// create new
	MeshBuilder meshBuilder{};
	meshBuilder.id = UUID::create();
	meshBuilder.type = type;
	Ref<Mesh> mesh = AssetManager::create<Mesh>(meshBuilder);

	// add it to defaults
	s_defaultMeshes.emplace(type, mesh);

	return mesh;
}

UInt Minty::Renderer::get_default_material_id(AssetType const type, Space const space)
{
	return (static_cast<UShort>(type) << (sizeof(UShort) << 3)) | static_cast<UShort>(space);
}

Ref<Material> Minty::Renderer::get_or_create_default_material(Ref<Texture> const texture, AssetType const type, Space const space)
{
	MINTY_ASSERT_MESSAGE(texture != nullptr, "Cannot get or create a Material for a null Sprite Texture.");

	UInt id = get_default_material_id(type, space);

	auto found = s_defaultMaterials.find(id);

	if (found == s_defaultMaterials.end())
	{
		// new material
		MaterialBuilder builder{};
		builder.id = UUID::create();

		UUID templateId{};
		
		switch (type)
		{
		case AssetType::Sprite:
			switch (space)
			{
			case Space::D3:
				templateId = DEFAULT_SPRITE_MATERIAL_TEMPLATE;
				break;
			case Space::UI:
				templateId = DEFAULT_UI_MATERIAL_TEMPLATE;
				break;
			default:
				MINTY_ERROR_FORMAT("Invalid Space for AssetType Sprite: \"{}\".", to_string(space));
				break;
			}
			break;
		case AssetType::FontVariant:
			switch (space)
			{
			case Space::UI:
				templateId = DEFAULT_TEXT_MATERIAL_TEMPLATE;
				break;
			default:
				MINTY_ERROR_FORMAT("Invalid Space for AssetType FontVariant: \"{}\".", to_string(space));
				break;
			}
			break;
		default:
			MINTY_ABORT_FORMAT("Cannot create a default Mesh for the type \"{}\".", to_string(type));
			break;
		}

		builder.materialTemplate = AssetManager::get<MaterialTemplate>(templateId);

		// set texture
		Cargo cargo{};
		Texture const* spriteTexturePtr = texture.get();
		cargo.emplace("texture", Type::Asset, &spriteTexturePtr);
		builder.values.emplace("texture", std::move(cargo));

		Ref<Material> material = AssetManager::create<Material>(builder);

		// add to list
		s_defaultMaterials.emplace(id, material);

		return material;
	}

	// old material
	return found->second;
}

Ref<RenderPass> Minty::Renderer::create_render_pass(RenderPassBuilder const& builder)
{
	Owner<RenderPass> renderPass = RenderPass::create(builder);

	Ref<RenderPass> renderPassRef = renderPass.create_ref();

	s_renderPasses.push_back(std::move(renderPass));

	return renderPassRef;
}

void Minty::Renderer::destroy_render_pass(Ref<RenderPass> const renderPass)
{
	// find the render pass and remove it
	for (Size i = 0; i < s_renderPasses.size(); i++)
	{
		if (s_renderPasses.at(i).get() == renderPass.get())
		{
			s_renderPasses.erase(s_renderPasses.begin() + i);
			break;
		}
	}
}

Ref<RenderTarget> Minty::Renderer::create_render_target(Ref<RenderPass> const& renderPass)
{
#if defined(MINTY_VULKAN)
	return VulkanRenderer::create_render_target(renderPass);
#else
	return Ref<RenderTarget>();
#endif
}

Format Minty::Renderer::get_color_format()
{
#if defined(MINTY_VULKAN)
	return static_cast<Format>(VulkanRenderer::get_swapchain_surface_format().format);
#else
	return Format::Undefined;
#endif
}

Format Minty::Renderer::get_depth_format()
{
#if defined(MINTY_VULKAN)
	return static_cast<Format>(VulkanRenderer::find_depth_format());
#else
	return Format::Undefined;
#endif
}

void Minty::Renderer::bind_shader(Ref<Shader> const shader)
{
	s_boundShader = shader;
	if(s_boundShader != nullptr) s_boundShader->on_bind();
}

void Minty::Renderer::bind_material(Ref<Material> const material)
{
	MINTY_ASSERT_MESSAGE(material->get_template()->get_shader() == s_boundShader, "The Shader of the Material given to bind does not match the currently bound Shader.");

	s_boundMaterial = material;
	s_boundMaterial->on_bind();
}

void Minty::Renderer::bind_mesh(Ref<Mesh> const mesh)
{
	s_boundMesh = mesh;
	s_boundMesh->on_bind();
}

void Minty::Renderer::bind_vertex_buffer(Ref<Buffer> const buffer, UInt const binding)
{
#if defined(MINTY_VULKAN)
	VulkanRenderer::bind_vertex_buffer(VulkanRenderer::get_command_buffer(), static_cast<VkBuffer>(buffer->get_native()), binding);
#endif
}

void Minty::Renderer::bind_index_buffer(Ref<Buffer> const buffer)
{
#if defined(MINTY_VULKAN)
	VulkanRenderer::bind_index_buffer(VulkanRenderer::get_command_buffer(), static_cast<VkBuffer>(buffer->get_native()));
#endif
}

void Minty::Renderer::draw(Ref<Mesh> const mesh)
{
	MINTY_ASSERT_MESSAGE(mesh != nullptr, "Cannot draw a null Mesh.");

	draw_indices(mesh->get_index_count());
}
