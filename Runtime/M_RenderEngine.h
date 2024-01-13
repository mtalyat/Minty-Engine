#pragma once
#include "M_Object.h"

#include "M_Constants.h"
#include "M_Register.h"
#include "M_Color.h"
#include "M_Info.h"
#include "M_Window.h"
#include "M_Viewport.h"

#include "M_EntityRegistry.h"
#include "M_CameraComponent.h"
#include "M_MeshComponent.h"
#include "M_TransformComponent.h"
#include "M_UITransformComponent.h"
#include "M_SpriteComponent.h"

#include "M_Texture.h"
#include "M_Sprite.h"
#include "M_Shader.h"
#include "M_ShaderPass.h"
#include "M_MaterialTemplate.h"
#include "M_Material.h"
#include "M_Mesh.h"

#include "M_Rendering_Buffer.h"
#include "M_Rendering_TextureBuilder.h"
#include "M_Rendering_SpriteBuilder.h"
#include "M_Rendering_ShaderBuilder.h"
#include "M_Rendering_ShaderPassBuilder.h"
#include "M_Rendering_MaterialTemplateBuilder.h"
#include "M_Rendering_MaterialBuilder.h"
#include "M_Rendering_DescriptorSet.h"

#include "M_Error.h"

#include "glm.hpp"

#include <array>
#include <vector>
#include <set>
#include <optional>
#include <string>
#include <cstring>
#include <filesystem>

namespace minty
{
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	constexpr uint32_t const BIND_COUNT = 4;
	constexpr uint32_t const BIND_SHADER = 0;
	constexpr uint32_t const BIND_SHADER_PASS = 1;
	constexpr uint32_t const BIND_MATERIAL_TEMPLATE = 2;
	constexpr uint32_t const BIND_MATERIAL = 3;

	class Scene;

	namespace rendering
	{
		struct RenderEngineBuilder;
		struct TextureBuilder;
	}

	/// <summary>
	/// Handles rendering for the game engine.
	/// </summary>
	class RenderEngine
		: public Object
	{
	private:
		rendering::RenderEngineBuilder const* _builder;
		Window* _window;

		// assets

		Register<Texture> _textures;
		Register<Sprite> _sprites;
		Register<Material> _materials;
		Register<MaterialTemplate> _materialTemplates;
		Register<ShaderPass> _shaderPasses;
		Register<Shader> _shaders;
		Register<rendering::Buffer> _buffers;
		Register<Mesh> _meshes;

		std::array<ID, BIND_COUNT> _boundIds;

		Viewport _view;
		Color _backgroundColor;
		bool _initialized;

		Scene const* _scene = nullptr;
		EntityRegistry* _registry = nullptr;
		Entity _mainCamera = NULL_ENTITY;

		// (Vulkan) rendering components
		VkDevice _device;
		VkInstance instance;
		VkPhysicalDevice _physicalDevice = nullptr;

		VkDebugUtilsMessengerEXT _debugMessenger;
		VkQueue _graphicsQueue;
		VkSurfaceKHR _surface;
		VkQueue _presentQueue;
		VkSwapchainKHR _swapChain;
		std::vector<VkImage> _swapChainImages;
		VkFormat _swapChainImageFormat;
		VkExtent2D _swapChainExtent;
		std::vector<VkImageView> _swapChainImageViews;
		std::vector<VkFramebuffer> _swapChainFramebuffers;
		VkRenderPass _renderPass;
		VkCommandPool _commandPool;
		std::vector<VkCommandBuffer> _commandBuffers;
		QueueFamilyIndices _queueFamilyIndices;
		std::vector<VkSemaphore> _imageAvailableSemaphores;
		std::vector<VkSemaphore> _renderFinishedSemaphores;
		std::vector<VkFence> _inFlightFences;
		bool _framebufferResized = false;
		uint32_t _frame;

		VkImage _depthImage;
		VkDeviceMemory _depthImageMemory;
		VkImageView _depthImageView;
	public:
		/// <summary>
		/// Creates a new RenderEngine that will render to the given Window.
		/// </summary>
		RenderEngine(Window* const window);

		~RenderEngine();

		/// <summary>
		/// Draws a frame to the screen.
		/// </summary>
		void render_frame();

		/// <summary>
		/// Checks if the render engine is still running.
		/// </summary>
		/// <returns>True if the engine is still running.</returns>
		bool is_running() const;

		/// <summary>
		/// Checks if this RenderEngine has been initialized.
		/// </summary>
		/// <returns>True if the engine is initialized.</returns>
		bool is_initialized() const;

#pragma region Get

		/// <summary>
		/// Gets the device that this RenderEngine is rendering to.
		/// </summary>
		/// <returns></returns>
		VkDevice get_device() const;

		/// <summary>
		/// Gets the physical graphics device that this RenderEngine is using to render.
		/// </summary>
		/// <returns></returns>
		VkPhysicalDevice get_physical_device() const;

		/// <summary>
		/// Gets the render pass that this RenderEngine is using.
		/// </summary>
		/// <returns></returns>
		VkRenderPass get_render_pass() const;

		/// <summary>
		/// Gets the current frame index.
		/// </summary>
		/// <returns>The current frame index.</returns>
		uint32_t get_frame() const;

#pragma endregion

#pragma region Init

	public:
		/// <summary>
		/// Initializes the RenderEngine.
		/// </summary>
		void init(rendering::RenderEngineBuilder const& builder);

	private:
		/// <summary>
		/// Creates a Vulkan instance.
		/// </summary>
		void create_instance();

		/// <summary>
		/// Initializes the debug messenger.
		/// </summary>
		void setup_debug_messenger();

		/// <summary>
		/// Creates the rendering surface.
		/// </summary>
		void create_surface();

		/// <summary>
		/// Picks the hardware (GPU) to use.
		/// </summary>
		void pick_physical_device();

		/// <summary>
		/// Pick the software side of the GPU to use (driver).
		/// </summary>
		void create_logical_device();

		/// <summary>
		/// Creates the swap chain.
		/// </summary>
		void create_swap_chain();

		/// <summary>
		/// Creates the image views in the swap chain.
		/// </summary>
		void create_image_views();

		/// <summary>
		/// Creates the render pass.
		/// </summary>
		void create_render_pass();

		/// <summary>
		/// Creates a command pool.
		/// </summary>
		void create_command_pool(VkCommandPool& commandPool);

		/// <summary>
		/// Creates the depth buffer resources.
		/// </summary>
		void create_depth_resources();

		/// <summary>
		/// Creates the frame buffers.
		/// </summary>
		void create_framebuffers();

		/// <summary>
		/// Creates the command buffers.
		/// </summary>
		void create_command_buffers();

		/// <summary>
		/// Create the objects used to sync the CPU and the GPU.
		/// </summary>
		void create_sync_objects();

#pragma endregion

#pragma region Update

	public:
		/// <summary>
		/// Updates the Renderable components within the Scene.
		/// </summary>
		void update();

		/// <summary>
		/// Updates the Camera uniform buffer with the Camera info.
		/// </summary>
		void update_camera(CameraComponent const& camera, TransformComponent const& transform);

#pragma endregion

#pragma region Binding

	private:
		/// <summary>
		/// Rebinds the currently bound shader and material IDs.
		/// </summary>
		/// <param name="commandBuffer"></param>
		void bind(VkCommandBuffer const commandBuffer);

		/// <summary>
		/// Binds the Material using its given material ID.
		/// </summary>
		/// <param name="commandBuffer">The buffer to use to bind.</param>
		/// <param name="materialId">The ID of the Material to bind.</param>
		/// <param name="pass">The ShaderPass index.</param>
		void bind(VkCommandBuffer const commandBuffer, ID const materialId, uint32_t const pass = 0); // TODO: easier way to select shader pass?

#pragma endregion

#pragma region Destroy

	public:
		/// <summary>
		/// Waits for the device to be idle.
		/// </summary>
		void sync();

		/// <summary>
		/// Cleans up all of the render engine resources.
		/// </summary>
		void destroy();

#pragma endregion

#pragma region Data

	public:
		/// <summary>
		/// Sets the Camera that this RenderEngine is rendering from.
		/// </summary>
		/// <param name="entity">The entity to render from.</param>
		void set_main_camera(Entity const entity);

		/// <summary>
		/// Sets the Scene that this RenderEngine is rendering.
		/// </summary>
		/// <param name="scene">The Scene to render.</param>
		/// <param name="camera">The entity to render from.</param>
		void set_scene(Scene const* const scene, Entity const camera = NULL_ENTITY);

#pragma endregion

#pragma region Debug

	private:
		/// <summary>
		/// Fills in the create info for the debug messenger.
		/// </summary>
		/// <param name="createInfo">The create info to populate.</param>
		void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

#pragma endregion

#pragma region Assets

	public:
		/// <summary>
		/// Creates a new Texture using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_texture(rendering::TextureBuilder const& builder);

		/// <summary>
		/// Creates a new Sprite using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_sprite(rendering::SpriteBuilder const& builder);

		/// <summary>
		/// Creates a new Shader using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_shader(rendering::ShaderBuilder const& builder);

		/// <summary>
		/// Creates a new ShaderPass using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_shader_pass(rendering::ShaderPassBuilder const& builder);

		/// <summary>
		/// Creates a new MaterialTemplate using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_material_template(rendering::MaterialTemplateBuilder const& builder);

		/// <summary>
		/// Creates a new Material using the given builder, then returns its ID.
		/// </summary>
		/// <param name="builder">The rendering builder to use.</param>
		/// <returns>The ID of the newly created asset.</returns>
		ID create_material(rendering::MaterialBuilder const& builder);

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

		/// <summary>
		/// Finds the ID of the loaded Texture with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_texture(String const& name) const;

		/// <summary>
		/// Finds the ID of the loaded Sprite with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_sprite(String const& name) const;

		/// <summary>
		/// Finds the ID of the loaded Shader with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_shader(String const& name) const;

		/// <summary>
		/// Finds the ID of the loaded ShaderPass with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_shader_pass(String const& name) const;

		/// <summary>
		/// Finds the ID of the loaded MaterialTemplate with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_material_template(String const& name) const;

		/// <summary>
		/// Finds the ID of the loaded Material with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_material(String const& name) const;

		/// <summary>
		/// Finds the ID of the loaded Mesh with the given name.
		/// </summary>
		/// <param name="name">The name of the asset to find.</param>
		/// <returns>The ID of the asset, or ERROR_ID if none was found with the given name.</returns>
		ID find_mesh(String const& name) const;

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

		/// <summary>
		/// Loads the Texture at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_texture(Path const& path);

		/// <summary>
		/// Loads the Sprite at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_sprite(Path const& path);

		/// <summary>
		/// Loads the Shader at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_shader(Path const& path);

		/// <summary>
		/// Loads the ShaderPass at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_shader_pass(Path const& path);

		/// <summary>
		/// Loads the MaterialTemplate at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_material_template(Path const& path);

		/// <summary>
		/// Loads the Material at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_material(Path const& path);

		/// <summary>
		/// Loads the Mesh at the given asset path into this RenderEngine.
		/// </summary>
		/// <param name="path">The path relative to the Assets folder.</param>
		/// <returns>The ID of the newly loaded asset.</returns>
		ID load_mesh(Path const& path);

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
	private:
		void load_descriptor_values(std::unordered_map<String, Dynamic>& values, Node const& node, std::vector<rendering::UniformConstantInfo> const& infos) const;

		/// <summary>
		/// Checks if the asset at the given path can be loaded.
		/// </summary>
		/// <param name="path">The path to the asset.</param>
		/// <param name="requiresMeta">If true, the meta file will also be checked.</param>
		/// <returns>0 if the asset can be loaded, 1 if there was a problem with the main file, or 2 if there was a problem with the meta file.</returns>
		int check_asset(Path const& path, bool const requiresMeta) const;

		// loads a .obj file
		void load_mesh_obj(Path const& path, ID const meshId);

	public:
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

#pragma region Buffers

	public:
		/// <summary>
		/// Creates a buffer.
		/// </summary>
		/// <param name="size">The size of the buffer.</param>
		/// <param name="usage">How the buffer will be used.</param>
		/// <param name="properties">The memory properties.</param>
		/// <returns>The ID of the new buffer.</returns>
		ID create_buffer(VkDeviceSize const size, VkBufferUsageFlags const usage, VkMemoryPropertyFlags const properties);

		/// <summary>
		/// Creates a uniform buffer.
		/// </summary>
		/// <param name="size">The size of the buffer.</param>
		/// <returns>The ID of the new buffer.</returns>
		ID create_buffer_uniform(VkDeviceSize const size);

		/// <summary>
		/// Destroys the buffer with the given ID.
		/// </summary>
		/// <param name="id"></param>
		void destroy_buffer(ID const id);

		/// <summary>
		/// Maps the buffer data to a pointer in memory.
		/// </summary>
		/// <param name="id">The ID of the buffer to map.</param>
		/// <returns>A pointer to the buffer data in memory.</returns>
		void* map_buffer(ID const id) const;

		/// <summary>
		/// Unmaps the buffer data from memory. The invalidates the pointer given from map_buffer for the same ID.
		/// </summary>
		/// <param name="id">The ID of the buffer to unmap.</param>
		void unmap_buffer(ID const id) const;

		/// <summary>
		/// Sets the data for the buffer with the given ID.
		/// </summary>
		/// <param name="id">The ID of the buffer to modify.</param>
		/// <param name="data">The data to set to the buffer.</param>
		void set_buffer(ID const id, void const* const data);

		/// <summary>
		/// Sets the data for the buffer with the given ID.
		/// </summary>
		/// <param name="id">The ID of the buffer to modify.</param>
		/// <param name="data">The data to set to the buffer.</param>
		/// <param name="size">The size of the data in bytes.</param>
		/// <param name="offset">The offset of the data within the buffer in bytes.</param>
		void set_buffer(ID const id, void const* const data, VkDeviceSize const size, VkDeviceSize const offset = 0);

		/// <summary>
		/// Gets the buffer with the given ID.
		/// </summary>
		/// <param name="id">The ID of the buffer.</param>
		/// <returns>The buffer with the givne ID.</returns>
		VkBuffer get_buffer(ID const id) const;

		/// <summary>
		/// Gets the data of the buffer with the given ID and stores it within the out pointer.
		/// </summary>
		/// <param name="id">The ID of the buffer to get the data from.</param>
		/// <param name="out">The pointer to the location of the data to be set.</param>
		void get_buffer_data(ID const id, void* const out) const; // TODO: test method

		/// <summary>
		/// Gets the size of the buffer with the given ID.
		/// </summary>
		/// <param name="id">The ID of the buffer to get the size of.</param>
		/// <returns>The size of the buffer in bytes.</returns>
		VkDeviceSize get_buffer_size(ID const id) const;

		/// <summary>
		/// Copies the buffer with the srcId to the buffer with the dstId.
		/// </summary>
		/// <param name="srcId">The ID of the source buffer.</param>
		/// <param name="dstId">The ID of the destination buffer.</param>
		/// <param name="size">The number of bytes to copy.</param>
		void copy_buffer(ID const srcId, ID const dstId, VkDeviceSize const size);

	private:
		// destroys the buffer data directly
		void destroy_buffer(rendering::Buffer const& buffer);

#pragma endregion

#pragma region Drawing

	private:
		void draw_scene(VkCommandBuffer commandBuffer);

		void draw_mesh(VkCommandBuffer commandBuffer, Matrix4 const& transformationMatrix, MeshComponent const& meshComponent);

		void draw_sprite(VkCommandBuffer commandBuffer, TransformComponent const& transformComponent, SpriteComponent const& spriteComponent);

		void draw_ui(VkCommandBuffer commandBuffer, UITransformComponent const& uiComponent, SpriteComponent const& spriteComponent);

#pragma endregion

#pragma region Helper

	public:

		/// <summary>
		/// Creates an image.
		/// </summary>
		/// <param name="width">The width of the image.</param>
		/// <param name="height">The height of the image.</param>
		/// <param name="format">The format of the image.</param>
		/// <param name="tiling">The tiling mode of the image.</param>
		/// <param name="usage">How the image will be used.</param>
		/// <param name="properties">The properties of the image.</param>
		/// <param name="image">The image object to update/create values in.</param>
		/// <param name="imageMemory">The memory location of where the image is stored.</param>
		void create_image(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

		/// <summary>
		/// Changes the image layout.
		/// </summary>
		/// <param name="image">The image to change.</param>
		/// <param name="format">The format of the image.</param>
		/// <param name="oldLayout">The old layout of the image.</param>
		/// <param name="newLayout">The new layout of the image.</param>
		void change_image_layout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		/// <summary>
		/// Coppies the buffer to the image data.
		/// </summary>
		/// <param name="buffer">The buffer to copy the data from.</param>
		/// <param name="image">The image to copy the data to.</param>
		/// <param name="width">The width of the image.</param>
		/// <param name="height">The height of the image.</param>
		void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		/// <summary>
		/// Creates an image view, so the image can be seen.
		/// </summary>
		/// <param name="image">The image to view.</param>
		/// <param name="format">The format of the image.</param>
		/// <param name="aspectFlags">The aspect flags of the image.</param>
		/// <returns>The created image view.</returns>
		VkImageView create_image_view(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

		/// <summary>
		/// Loads the shader module from the asset path.
		/// </summary>
		/// <param name="path">The path to the shader code (.spv).</param>
		/// <returns>The shader module.</returns>
		VkShaderModule load_shader_module(String const& path) const;

		/// <summary>
		/// Creates a shader module from the given code.
		/// </summary>
		/// <param name="code">The code loaded from a .spv file.</param>
		/// <returns>The shader module.</returns>
		VkShaderModule create_shader_module(std::vector<char> const& code) const;

#pragma endregion

	private:
		/// <summary>
		/// Finds the depth rendering format that can be used.
		/// </summary>
		/// <returns>The depth format.</returns>
		VkFormat find_depth_format();

		/// <summary>
		/// Checks if the given format has a stencil component.
		/// </summary>
		/// <param name="format">The format to check.</param>
		/// <returns>True if the format has a stencil component.</returns>
		bool has_stencil_component(VkFormat format);

		/// <summary>
		/// Finds the supported format that allows for the given image tiling and format feature flags.
		/// </summary>
		/// <param name="candidates">The potential formats to choose from.</param>
		/// <param name="tiling">The image tiling data.</param>
		/// <param name="features">The feature flags.</param>
		/// <returns>The found format.</returns>
		VkFormat find_supported_format(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		/// <summary>
		/// Start the command buffer.
		/// </summary>
		/// <returns>The command buffer that has began, and is ready for use.</returns>
		VkCommandBuffer begin_single_time_commands(VkCommandPool commandPool);

		/// <summary>
		/// Ends the command buffer.
		/// </summary>
		/// <param name="commandBuffer">The command buffer to end, and will be no longer used.</param>
		void end_single_time_commands(VkCommandBuffer commandBuffer, VkCommandPool commandPool);

		/// <summary>
		/// Checks if the given device is able to do the operations we want.
		/// </summary>
		/// <param name="device">The device to check.</param>
		/// <returns>True if it is suitable.</returns>
		bool is_device_suitable(VkPhysicalDevice device);

		/// <summary>
		/// Cleans up any resources in the current swap chain.
		/// </summary>
		void cleanup_swap_chain();

		/// <summary>
		/// Recreates the swap chain. Important for window changing events, such as a resize.
		/// </summary>
		virtual void recreate_swap_chain();

		/// <summary>
		/// Chooses the swap extent, given the capabilities of the surface.
		/// </summary>
		/// <param name="capabilities">The surface capabilities.</param>
		/// <returns>The chosen extent.</returns>
		VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);

		/// <summary>
		/// Chooses the swap present mode, given the available modes.
		/// </summary>
		/// <param name="availablePresentModes">The available present modes.</param>
		/// <returns>The chosen mode.</returns>
		VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		/// <summary>
		/// Chooses the swap surface format, givent the available formats.
		/// </summary>
		/// <param name="availableFormats">The available surface formats.</param>
		/// <returns>The chosen format.</returns>
		VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		/// <summary>
		/// Checks if the given device supports a swap chain.
		/// </summary>
		/// <param name="device">The device to check.</param>
		/// <returns>Details on swap chain support.</returns>
		SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device);

		/// <summary>
		/// Checks if the given device has extension support.
		/// </summary>
		/// <param name="device">The device to check.</param>
		/// <returns>True if the device has extension support.</returns>
		bool check_device_extension_support(VkPhysicalDevice device);

		/// <summary>
		/// Finds the queue families.
		/// </summary>
		/// <param name="device">The device to grab the queue families from.</param>
		/// <returns>The indices of the queue families.</returns>
		QueueFamilyIndices find_queue_families(VkPhysicalDevice device);

		/// <summary>
		/// Check if all validation layers are available for debugging.
		/// </summary>
		/// <returns>True if all validation layers are available for debugging.</returns>
		bool check_validation_layer_support();

		/// <summary>
		/// Gets the names of the required extensions.
		/// </summary>
		/// <returns>A vector of all of the extensions.</returns>
		std::vector<const char*> get_required_extensions();

		/// <summary>
		/// Called when a debug message has been "printed" from Vulkan.
		/// </summary>
		/// <param name="messageSeverity">The severtity of the message.</param>
		/// <param name="messageType">The type of the message.</param>
		/// <param name="pCallbackData">The extra callback data.</param>
		/// <param name="pUserData">The user data.</param>
		/// <returns>???</returns>
		static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

		/// <summary>
		/// Finds the memory type.
		/// </summary>
		/// <param name="typeFilter">The filter for what types are acceptable.</param>
		/// <param name="properties">The memory properties.</param>
		/// <returns>The found type. ???</returns>
		uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		/// <summary>
		/// Records the command buffer.
		/// </summary>
		/// <param name="commandBuffer">The command buffer to record.</param>
		/// <param name="imageIndex">The image to send the commands to.</param>
		void record_command_buffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	public:

		friend String to_string(RenderEngine const& value);
	};
}