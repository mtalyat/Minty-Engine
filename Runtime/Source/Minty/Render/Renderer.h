#pragma once

#include "Minty/Core/Color.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/Types.h"
#include "Minty/Window/Window.h"
#include "Minty/Render/Camera.h"
#include "Minty/Render/Mesh.h"
#include "Minty/Render/Material.h"
#include "Minty/Render/MaterialTemplate.h"
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
		Ref<Window> window;
		Color clearColor = Color::black();
	};

	/// <summary>
	/// Handles rendering things to the screen.
	/// </summary>
	class Renderer
	{
	private:
		static Ref<Window> s_window;
		static Color s_color;

		static Ref<RenderTarget> s_renderTarget;

		static std::unordered_map<MeshType, Ref<Mesh>> s_defaultMeshes;
		static std::unordered_map <UInt, Ref<Material>> s_defaultMaterials;

		static Ref<Shader> s_boundShader;
		static Ref<Material> s_boundMaterial;
		static Ref<Mesh> s_boundMesh;

	private:
		Renderer() = default;
		~Renderer() = default;

	public:
		static void initialize(const RendererBuilder& builder);
		static void shutdown();

		static int start_frame(const Ref<RenderTarget> tempRenderTarget = nullptr);
		static void end_frame();

		// sets the currently active camera
		static void set_camera(Float3 const position, Quaternion const rotation, Camera const& camera);

		static void sync();

		static void draw_vertices(const UInt vertexCount);
		static void draw_instances(const UInt instanceCount, const UInt vertexCount = 0);
		static void draw_indices(const UInt indexCount);

		/// <summary>
		/// On a significant event that alters the rendering environment (Window resize, etc.), refresh all data in the Renderer.
		/// </summary>
		static void refresh();

#pragma region Get Set

	public:
		static Ref<Viewport> get_viewport();

		static Ref<Scissor> get_scissor();

		static Ref<Mesh> get_or_create_mesh(MeshType const type);

	private:
		static UInt get_default_material_id(AssetType const type, Space const space);

	public:
		static Ref<Material> get_or_create_default_material(Ref<Texture> const texture, AssetType const type, Space const space);

		static Owner<RenderTarget> create_render_target();

		static void set_render_target(const Ref<RenderTarget> renderTarget) { s_renderTarget = renderTarget; }

		static Ref<Window> get_window() { return s_window; }

		static Color get_color() { return s_color; }

		static void set_color(Color const color) { s_color = color; }

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