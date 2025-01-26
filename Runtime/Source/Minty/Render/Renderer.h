#pragma once

#include "Minty/Core/Color.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/Types.h"
#include "Minty/Window/Window.h"
#include "Minty/Render/Camera.h"
#include "Minty/Render/Mesh.h"
#include "Minty/Render/Material.h"
#include "Minty/Render/MaterialTemplate.h"
#include "Minty/Render/RenderPass.h"
#include "Minty/Render/RenderTarget.h"
#include "Minty/Render/Scissor.h"
#include "Minty/Render/Shader.h"
#include "Minty/Render/Space.h"
#include "Minty/Render/Sprite.h"
#include "Minty/Render/Texture.h"
#include "Minty/Render/Viewport.h"

namespace Minty
{
	struct RendererBuilder
	{
		/// <summary>
		/// The Window to render to.
		/// </summary>
		Ref<Window> window;

		/// <summary>
		/// The starting clear color for the Renderer.
		/// </summary>
		Color clearColor = Color::black();

		/// <summary>
		/// Used to determine the format of all surfaces. Set to undefined to use the default format (B8G8R8A8_SRGB).
		/// </summary>
		Format targetSurfaceFormat = Format::Undefined;
	};

	/// <summary>
	/// Handles rendering things to the screen.
	/// </summary>
	class Renderer
	{
	private:
		static Ref<Window> s_window;
		static Color s_color;

		static Owner<RenderPass> s_renderPass;
		static Ref<RenderTarget> s_renderTarget;

		static std::vector<Ref<RenderTarget>> s_screenTargets;

		static std::unordered_map<MeshType, Ref<Mesh>> s_defaultMeshes;
		static std::unordered_map <UInt, Ref<Material>> s_defaultMaterials;

		static Ref<Shader> s_boundShader;
		static Ref<Material> s_boundMaterial;
		static Ref<Mesh> s_boundMesh;

	private:
		Renderer() = default;
		~Renderer() = default;

	public:
		static void initialize(RendererBuilder const& builder);
		static void shutdown();

		static Int start_frame();
		static void end_frame();

		static void start_render_pass(Ref<RenderPass> const& renderPass, Ref<RenderTarget> const& renderTarget);
		static void end_render_pass();
		static void transition_between_render_passes();

		// skips the render pass and draws nothing
		static void skip_render_pass(Ref<RenderPass> const& renderPass, Ref<RenderTarget> const& renderTarget);
		// skips the scene render pass
		static void skip();

		// sets the currently active camera
		static void set_camera(Float3 const position, Quaternion const rotation, Camera const& camera);

		static void sync();

		static void draw_vertices(UInt const vertexCount);
		static void draw_instances(UInt const instanceCount, UInt const vertexCount = 0);
		static void draw_indices(UInt const indexCount);

		/// <summary>
		/// On a significant event that alters the rendering environment (Window resize, etc.), refresh all data in the Renderer.
		/// </summary>
		static void refresh();

#pragma region Create

	public:
		static Ref<Material> get_or_create_default_material(Ref<Texture> const texture, AssetType const type, Space const space);

		static Owner<RenderPass> create_render_pass(RenderPassBuilder const& builder);

		static Ref<RenderTarget> create_render_target(Ref<RenderPass> const& renderPass);

#pragma endregion

#pragma region Get Set

	public:
		static Ref<Viewport> get_viewport();

		static Ref<Scissor> get_scissor();

		static Ref<Mesh> get_or_create_mesh(MeshType const type);

	private:
		static UInt get_default_material_id(AssetType const type, Space const space);

	public:
		static Ref<RenderPass> get_render_pass() { return s_renderPass.create_ref(); }

		static Ref<RenderTarget> get_render_target() { return s_renderTarget; }

		static std::vector<Ref<RenderTarget>> const& get_screen_render_targets() { return s_screenTargets; }

		static Ref<Window> get_window() { return s_window; }

		static Color get_color() { return s_color; }

		static void set_color(Color const color) { s_color = color; }

		static Format get_color_format();

		static Format get_depth_format();

#pragma endregion

#pragma region Bind

	public:
		// binds the shader to be used for rendering, returns false if already bound
		static void bind_shader(Ref<Shader> const shader);

		// binds the material for rendering, returns false if already bound
		static void bind_material(Ref<Material> const material);

		// binds the given mesh
		static void bind_mesh(Ref<Mesh> const mesh);

		static void bind_vertex_buffer(Ref<Buffer> const buffer, UInt const binding = 0);

		static void bind_index_buffer(Ref<Buffer> const buffer);

#pragma endregion

#pragma region Draw

	public:
		// draws the mesh to the screen
		static void draw(Ref<Mesh> const mesh);

#pragma endregion
	};
}