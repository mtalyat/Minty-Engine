#include "pch.h"
#include "Renderer.h"

#include "Minty/Asset/AssetManager.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanRenderer.h"
#endif

using namespace Minty;

Ref<Window> Minty::Renderer::s_window = nullptr;
Color Minty::Renderer::s_color = Color::black();
Ref<RenderTarget> Minty::Renderer::s_renderTarget = nullptr;
std::unordered_map<MeshType, Ref<Mesh>> Minty::Renderer::s_defaultMeshes = {};
std::unordered_map <UUID, Ref<Material>> Minty::Renderer::s_spriteMaterials = {};
Ref<Shader> Minty::Renderer::s_boundShader = nullptr;
Ref<Material> Minty::Renderer::s_boundMaterial = nullptr;
Ref<Mesh> Minty::Renderer::s_boundMesh = nullptr;

void Minty::Renderer::initialize(const RendererBuilder& builder)
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
	// shutdown API
#if defined(MINTY_VULKAN)
	VulkanRenderer::shutdown();
#endif

	s_window = nullptr;
}

int Minty::Renderer::start_frame(const Ref<RenderTarget> tempRenderTarget)
{
	MINTY_ASSERT_MESSAGE(s_renderTarget.get() != nullptr || tempRenderTarget.get() != nullptr, "Cannot start frame without a RenderTarget.");

	Ref<RenderTarget> target = s_renderTarget.get() != nullptr ? s_renderTarget : tempRenderTarget;

	// start frame
#if defined(MINTY_VULKAN)
	return VulkanRenderer::start_frame(s_renderTarget);
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

	// update all materials
	for (auto const& material : AssetManager::get_by_type<Material>(AssetType::Material))
	{
		material->set_input("camera", &transformMatrix);
	}
}

void Minty::Renderer::sync()
{
#if defined(MINTY_VULKAN)
	VulkanRenderer::sync();
#endif
}

void Minty::Renderer::draw_vertices(const UInt vertexCount)
{
#if defined(MINTY_VULKAN)
	VulkanRenderer::draw_vertices(vertexCount);
#endif
}

void Minty::Renderer::draw_indices(const UInt indexCount)
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

Ref<Material> Minty::Renderer::get_or_create_sprite_material(Ref<Texture> const spriteTexture)
{
	MINTY_ASSERT_MESSAGE(spriteTexture != nullptr, "Cannot get or create a Material for a null Sprite Texture.");

	auto found = s_spriteMaterials.find(spriteTexture->id());

	if (found == s_spriteMaterials.end())
	{
		// create new
		MaterialBuilder materialBuilder{};
		materialBuilder.id = UUID::create();
		materialBuilder.materialTemplate = AssetManager::get<MaterialTemplate>(DEFAULT_SPRITE_MATERIAL_TEMPLATE);

		Cargo cargo{};
		cargo.emplace("texture", Type::Asset, spriteTexture.get());
		materialBuilder.values.emplace("texture", std::move(cargo));

		Ref<Material> material = AssetManager::create<Material>(materialBuilder);

		// add to list
		s_spriteMaterials.emplace(spriteTexture->id(), material);

		return material;
	}

	// use existing
	return found->second;
}

Owner<RenderTarget> Minty::Renderer::create_render_target()
{
#if defined(MINTY_VULKAN)
	return VulkanRenderer::create_render_target();
#endif
}

Bool Minty::Renderer::bind_shader(Ref<Shader> const shader)
{
	// return if already bound
	if (s_boundShader == shader) return false;

	s_boundShader = shader;
	if(s_boundShader != nullptr) s_boundShader->on_bind();

	return true;
}

Bool Minty::Renderer::bind_material(Ref<Material> const material)
{
	if (s_boundMaterial == material) return false;

	MINTY_ASSERT_MESSAGE(material->get_template()->get_shader() == s_boundShader, "The Shader of the Material given to bind does not match the currently bound Shader.");

	s_boundMaterial = material;
	s_boundMaterial->on_bind();

	return true;
}

Bool Minty::Renderer::bind_mesh(Ref<Mesh> const mesh)
{
	if (s_boundMesh == mesh) return false;

	s_boundMesh = mesh;
	s_boundMesh->on_bind();

	return true;
}

void Minty::Renderer::draw(Ref<Mesh> const mesh)
{
	MINTY_ASSERT_MESSAGE(mesh != nullptr, "Cannot draw a null Mesh.");

	draw_indices(mesh->get_index_count());
}

void Minty::Renderer::draw(Ref<Sprite> const sprite)
{
	MINTY_ASSERT_MESSAGE(sprite != nullptr, "Cannot draw a null Sprite.");

	draw_vertices(6);
}
