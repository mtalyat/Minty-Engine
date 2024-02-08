#pragma once

#include "M_System.h"

#include "M_Register.h"
#include "M_Texture.h"
#include "M_TextureAtlas.h"
#include "M_Sprite.h"
#include "M_Material.h"
#include "M_MaterialTemplate.h"
#include "M_ShaderPass.h"
#include "M_Shader.h"
#include "M_Mesh.h"

#include "M_Entity.h"

#include <unordered_map>
#include <vector>

namespace minty
{
	struct CameraComponent;
	struct TransformComponent;

	class RenderSystem
		: public System
	{
	private:
		Entity _mainCamera = NULL_ENTITY;

		Register<Texture> _textures;
		Register<Sprite> _sprites;
		Register<Material> _materials;
		Register<MaterialTemplate> _materialTemplates;
		Register<ShaderPass> _shaderPasses;
		Register<Shader> _shaders;
		Register<Mesh> _meshes;

	public:
		RenderSystem(Engine& engine, ID const sceneId);

		~RenderSystem();

		void reset() override;

		void update() override;

		void unload() override;

		/// <summary>
		/// Updates the Camera uniform buffer with the Camera info.
		/// </summary>
		void update_camera(CameraComponent const& camera, TransformComponent const& transform);

#pragma region Create

		/// <summary>
		/// Creates a new Texture using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_texture(TextureBuilder const& builder);

		/// <summary>
		/// Creates a new Sprite using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_sprite(SpriteBuilder const& builder);

		/// <summary>
		/// Creates a new Shader using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_shader(ShaderBuilder const& builder);

		/// <summary>
		/// Creates a new ShaderPass using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_shader_pass(ShaderPassBuilder const& builder);

		/// <summary>
		/// Creates a new MaterialTemplate using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_material_template(MaterialTemplateBuilder const& builder);

		/// <summary>
		/// Creates a new Material using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_material(MaterialBuilder const& builder);

		/// <summary>
		/// Creates a new Mesh using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_mesh();

		/// <summary>
		/// Finds a loaded mesh with the given name, or creates an empty Mesh with the given name if no Mesh exists.
		/// </summary>
		/// <param name="name">The name of the Mesh to get or create.</param>
		/// <returns>The ID of the existing Mesh, or the ID of the new Mesh.</returns>
		ID get_or_create_mesh(String const& name);

		/// <summary>
		/// Finds a loaded mesh with the given type, or creates a mesh of that type if no Mesh exists.
		/// </summary>
		/// <param name="name">The type of the Mesh to get or create.</param>
		/// <returns>The ID of the existing Mesh, or the ID of the new Mesh.</returns>
		ID get_or_create_mesh(MeshType const type);

#pragma endregion

#pragma region Find

		/// <summary>
		/// Finds the ID of the loaded Texture with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_texture(String const& name, ID const defaultId = ERROR_ID) const;

		/// <summary>
		/// Finds the ID of the loaded Sprite with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_sprite(String const& name, ID const defaultId = ERROR_ID) const;

		/// <summary>
		/// Finds the ID of the loaded Shader with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_shader(String const& name, ID const defaultId = ERROR_ID) const;

		/// <summary>
		/// Finds the ID of the loaded ShaderPass with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_shader_pass(String const& name, ID const defaultId = ERROR_ID) const;

		/// <summary>
		/// Finds the ID of the loaded MaterialTemplate with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_material_template(String const& name, ID const defaultId = ERROR_ID) const;

		/// <summary>
		/// Finds the ID of the loaded Material with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_material(String const& name, ID const defaultId = ERROR_ID) const;

		/// <summary>
		/// Finds the ID of the loaded Mesh with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_mesh(String const& name, ID const defaultId = ERROR_ID) const;

#pragma endregion

#pragma region Name

		/// <summary>
		/// Finds the name of the loaded Texture with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to get the name of.</param>
		/// <returns>The name of the given asset, or "" if no asset was found with the given ID.</returns>
		String get_texture_name(ID const id) const;

		/// <summary>
		/// Finds the name of the loaded Sprite with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to get the name of.</param>
		/// <returns>The name of the given asset, or "" if no asset was found with the given ID.</returns>
		String get_sprite_name(ID const id) const;

		/// <summary>
		/// Finds the name of the loaded Shader with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to get the name of.</param>
		/// <returns>The name of the given asset, or "" if no asset was found with the given ID.</returns>
		String get_shader_name(ID const id) const;

		/// <summary>
		/// Finds the name of the loaded ShaderPass with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to get the name of.</param>
		/// <returns>The name of the given asset, or "" if no asset was found with the given ID.</returns>
		String get_shader_pass_name(ID const id) const;

		/// <summary>
		/// Finds the name of the loaded MaterialTemplate with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to get the name of.</param>
		/// <returns>The name of the given asset, or "" if no asset was found with the given ID.</returns>
		String get_material_template_name(ID const id) const;

		/// <summary>
		/// Finds the name of the loaded Material with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to get the name of.</param>
		/// <returns>The name of the given asset, or "" if no asset was found with the given ID.</returns>
		String get_material_name(ID const id) const;

		/// <summary>
		/// Finds the name of the loaded Mesh with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to get the name of.</param>
		/// <returns>The name of the given asset, or "" if no asset was found with the given ID.</returns>
		String get_mesh_name(ID const id) const;

#pragma endregion

#pragma region Load

		/// <summary>
		/// Loads the Texture at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_texture(Path const& path);

		/// <summary>
		/// Loads the Texture at the given asset path with the given name into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <param name="name">The name of the newly loaded asset.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_texture(Path const& path, String const& name);

		/// <summary>
		/// Loads the Sprite at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_sprite(Path const& path);

		/// <summary>
		/// Loads the Sprite at the given asset path with the given name into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <param name="name">The name of the newly loaded asset.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_sprite(Path const& path, String const& name);

		/// <summary>
		/// Loads the Shader at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_shader(Path const& path);

		/// <summary>
		/// Loads the Shader at the given asset path with the given name into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <param name="name">The name of the newly loaded asset.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_shader(Path const& path, String const& name);

		/// <summary>
		/// Loads the ShaderPass at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_shader_pass(Path const& path);

		/// <summary>
		/// Loads the ShaderPass at the given asset path with the given name into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <param name="name">The name of the newly loaded asset.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_shader_pass(Path const& path, String const& name);

		/// <summary>
		/// Loads the MaterialTemplate at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_material_template(Path const& path);

		/// <summary>
		/// Loads the MaterialTemplate at the given asset path with the given name into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <param name="name">The name of the newly loaded asset.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_material_template(Path const& path, String const& name);

		/// <summary>
		/// Loads the Material at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_material(Path const& path);

		/// <summary>
		/// Loads the Material at the given asset path with the given name into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <param name="name">The name of the newly loaded asset.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_material(Path const& path, String const& name);

		/// <summary>
		/// Loads the Mesh at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_mesh(Path const& path);

		/// <summary>
		/// Loads the Mesh at the given asset path with the given name into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <param name="name">The name of the newly loaded asset.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_mesh(Path const& path, String const& name);

#pragma endregion

#pragma region Destroy

		/// <summary>
		/// Destroys the Texture with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to destroy.</param>
		void destroy_texture(ID const id);

		/// <summary>
		/// Destroys the Sprite with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to destroy.</param>
		void destroy_sprite(ID const id);

		/// <summary>
		/// Destroys the Shader with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to destroy.</param>
		void destroy_shader(ID const id);

		/// <summary>
		/// Destroys the ShaderPass with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to destroy.</param>
		void destroy_shader_pass(ID const id);

		/// <summary>
		/// Destroys the MaterialTemplate with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to destroy.</param>
		void destroy_material_template(ID const id);

		/// <summary>
		/// Destroys the Material with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to destroy.</param>
		void destroy_material(ID const id);

		/// <summary>
		/// Destroys the Mesh with the given ID.
		/// </summary>
		/// <param name="id">The ID of the asset to destroy.</param>
		void destroy_mesh(ID const id);

		/// <summary>
		/// Destroys the all of the assets.
		/// </summary>
		void destroy_assets();

#pragma endregion

#pragma region Get

		/// <summary>
		/// Gets the Texture with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Texture& get_texture(ID const id);

		/// <summary>
		/// Gets the Texture with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Texture const& get_texture(ID const id) const;

		/// <summary>
		/// Gets the Sprite with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Sprite& get_sprite(ID const id);

		/// <summary>
		/// Gets the Sprite with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Sprite const& get_sprite(ID const id) const;

		/// <summary>
		/// Gets the Shader with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Shader& get_shader(ID const id);

		/// <summary>
		/// Gets the Shader with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Shader const& get_shader(ID const id) const;

		/// <summary>
		/// Gets the Shader with the given Material ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Shader& get_shader_from_material_id(ID const id);

		/// <summary>
		/// Gets the Shader with the given Material ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Shader const& get_shader_from_material_id(ID const id) const;

		/// <summary>
		/// Gets the ShaderPass with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		ShaderPass& get_shader_pass(ID const id);

		/// <summary>
		/// Gets the ShaderPass with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		ShaderPass const& get_shader_pass(ID const id) const;

		/// <summary>
		/// Gets the ShaderPass with the given Material ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		ShaderPass& get_shader_pass_from_material_id(ID const id);

		/// <summary>
		/// Gets the ShaderPass with the given Material ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		ShaderPass const& get_shader_pass_from_material_id(ID const id) const;

		/// <summary>
		/// Gets the MaterialTemplate with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		MaterialTemplate& get_material_template(ID const id);

		/// <summary>
		/// Gets the MaterialTemplate with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		MaterialTemplate const& get_material_template(ID const id) const;

		/// <summary>
		/// Gets the MaterialTemplate with the given Material ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		MaterialTemplate& get_material_template_from_material_id(ID const id);

		/// <summary>
		/// Gets the MaterialTemplate with the given Material ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		MaterialTemplate const& get_material_template_from_material_id(ID const id) const;

		/// <summary>
		/// Gets the Material with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Material& get_material(ID const id);

		/// <summary>
		/// Gets the Material with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Material const& get_material(ID const id) const;

		/// <summary>
		/// Gets the Mesh with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Mesh& get_mesh(ID const id);

		/// <summary>
		/// Gets the Mesh with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Mesh const& get_mesh(ID const id) const;

#pragma endregion

#pragma region Set

	public:
		/// <summary>
		/// Sets the Camera that this RenderEngine is rendering from.
		/// </summary>
		/// <param name="entity">The entity to render from.</param>
		void set_main_camera(Entity const entity);

#pragma endregion


	private:
		void load_descriptor_values(std::unordered_map<String, Dynamic>& values, Node const& node, std::vector<UniformConstantInfo> const& infos) const;

		// loads a .obj file
		void load_mesh_obj(Path const& path, ID const meshId);

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}