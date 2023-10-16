#pragma once
#include "M_Object.h"

#include "M_Constants.h"
#include "M_Register.h"
#include "M_Color.h"
#include "M_Info.h"
#include "M_Window.h"
#include "M_Viewport.h"
#include "M_Vector3.h"
#include "M_Transform.h"

#include "M_EntityRegistry.h"
#include "M_CameraComponent.h"
#include "M_MeshComponent.h"

#include "M_Texture.h"
#include "M_Shader.h"
#include "M_Material.h"
#include "M_Mesh.h"

#include "M_Rendering_TextureBuilder.h"
#include "M_Rendering_ShaderBuilder.h"
#include "M_Rendering_MaterialBuilder.h"

#include "M_Error.h"

#include "glm.hpp"

#include <array>
#include <vector>
#include <set>
#include <optional>
#include <string>
#include <cstring>

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

	class Scene;

	namespace rendering
	{
		class RendererBuilder;
		class TextureBuilder;
	}

	/// <summary>
	/// Handles rendering for the game engine.
	/// </summary>
	class Renderer
		: public Object
	{
	private:
		rendering::RendererBuilder const* _builder;
		Window* _window;
		
		//std::vector<Texture> _textures;
		//std::vector<Material> _materials;
		//std::vector<Shader> _shaders;
		Register<Texture> _textures;
		Register<Material> _materials;
		Register<Shader> _shaders;

		Material* _boundMaterial;
		Shader* _boundShader;

		Viewport _view;
		Color _backgroundColor;
		bool _initialized;
	protected:
		Scene const* _scene = nullptr;
		EntityRegistry const* _registry = nullptr;
		Entity _mainCamera = NULL_ENTITY;

		// (Vulkan) rendering components
		VkDevice _device;
		VkInstance instance;
		VkPhysicalDevice _physicalDevice = nullptr;

		VkDebugUtilsMessengerEXT debugMessenger;
		VkQueue graphicsQueue;
		VkSurfaceKHR surface;
		VkQueue presentQueue;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;
		VkRenderPass _renderPass;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
		QueueFamilyIndices queueFamilyIndices;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		bool framebufferResized = false;
		uint32_t _frame = 0;

		std::vector<VkBuffer> materialBuffers;
		std::vector<VkDeviceMemory> materialBuffersMemory;
		std::vector<void*> materialBuffersMapped;

		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;
	public:
		Renderer(Window* const window);

		~Renderer();

		/// <summary>
		/// Draws a frame to the screen.
		/// </summary>
		void render_frame();

		/// <summary>
		/// Checks if the render engine is still running.
		/// </summary>
		/// <returns>True if the engine is still running.</returns>
		bool is_running() const;

		bool is_initialized() const;

#pragma region Get

		/// <summary>
		/// Gets the device that this Renderer is rendering to.
		/// </summary>
		/// <returns></returns>
		VkDevice get_device() const;

		/// <summary>
		/// Gets the physical graphics device that this Renderer is using to render.
		/// </summary>
		/// <returns></returns>
		VkPhysicalDevice get_physical_device() const;

		VkRenderPass get_render_pass() const;

		uint32_t get_shader_count() const;

		uint32_t get_texture_count() const;

		uint32_t get_material_count() const;

		uint32_t get_frame() const;

#pragma endregion

#pragma region Init

	public:
		/// <summary>
		/// Initializes the Renderer.
		/// </summary>
		void init(rendering::RendererBuilder const& builder);

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
		virtual void create_swap_chain();

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
		/// Creates all textures based on the builder plans.
		/// </summary>
		void build_textures();

		/// <summary>
		/// Creates all shaders based on the builder plans.
		/// </summary>
		void build_shaders();

		/// <summary>
		/// Creates all materials based on the builder plans.
		/// </summary>
		void build_materials();

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
		/// Updates the Renderer.
		/// </summary>
		void update();

		/// <summary>
		/// Updates the Camera uniform buffer with the Camera info.
		/// </summary>
		void update_camera(CameraComponent const& camera, Vector3 const& position, Vector3 const& rotation);

#pragma endregion

#pragma region Binding

		void bind_material(VkCommandBuffer const commandBuffer, Material& material);

		void unbind_material();

#pragma endregion

#pragma region Destroy

	public:
		/// <summary>
		/// Cleans up all of the render engine resources.
		/// </summary>
		void destroy();

#pragma endregion

#pragma region Data

	public:
		/// <summary>
		/// Sets the Camera that this Renderer is rendering from.
		/// </summary>
		/// <param name="entity">The entity to render from.</param>
		void set_main_camera(Entity const entity);

		/// <summary>
		/// Sets the Scene that this Renderer is rendering.
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

	private:
		/// <summary>
		/// Creates a Texture using the file at the given path.
		/// </summary>
		/// <param name="path">The path to the file on the disk.</param>
		/// <returns>The ID of the new Texture.</returns>
		ID create_texture(std::string const& path, rendering::TextureBuilder const& builder);

		ID create_shader(std::string const& vertexPath, std::string const& fragmentPath, rendering::ShaderBuilder const& builder);

		ID create_material(rendering::MaterialBuilder const& builder);

	public:
		Texture& get_texture(ID const id);

		Texture const& get_texture(ID const id) const;

		Shader& get_shader(ID const id);

		Shader const& get_shader(ID const id) const;

		Material& get_material(ID const id);

		Material const& get_material(ID const id) const;

#pragma endregion

#pragma region Drawing

	private:
		void draw_mesh(VkCommandBuffer commandBuffer, Transform const& transform, MeshComponent const& meshComponent);

#pragma endregion

#pragma region Helper

	private:
		/// <summary>
		/// Gets the Transform for the given Entity.
		/// </summary>
		/// <param name="entity"></param>
		/// <param name="transform"></param>
		void get_entity_transform(Entity const entity, Transform& transform) const;

	public:
		/// <summary>
		/// Creates a buffer.
		/// </summary>
		/// <param name="size">The size of the buffer.</param>
		/// <param name="usage">How the buffer will be used.</param>
		/// <param name="properties">The memory properties.</param>
		/// <param name="buffer">The buffer to create.</param>
		/// <param name="bufferMemory">The memory to be associated with the buffer.</param>
		void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

		/// <summary>
		/// Copies the buffer from source to destination.
		/// </summary>
		/// <param name="srcBuffer">The buffer to copy from.</param>
		/// <param name="dstBuffer">The buffer to copy to.</param>
		/// <param name="size">The size of the buffer to copy.</param>
		void copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

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
		/// Draws the objects within the Scene.
		/// </summary>
		void draw_scene(VkCommandBuffer commandBuffer);

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
		virtual void record_command_buffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	};
}