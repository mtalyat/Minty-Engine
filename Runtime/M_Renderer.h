#pragma once

#include "M_Object.h"
#include "M_Info.h"
#include "M_Window.h"
#include "M_Texture.h"
#include "M_Mesh.h"
#include "M_Viewport.h"
#include "M_Color.h"
#include "M_Shader.h"
#include "M_Material.h"
#include "M_CameraComponent.h"
#include "M_MeshComponent.h"
#include "M_Vector3.h"
#include "M_Transform.h"
#include "M_EntityRegistry.h"

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

	constexpr int MAX_FRAMES_IN_FLIGHT = 2;
	constexpr int MAX_SETS_PER_FRAME = 1;
	constexpr int MAX_SETS = MAX_FRAMES_IN_FLIGHT * MAX_SETS_PER_FRAME;

	// 100 is arbitrary
	constexpr int MAX_TEXTURES = 4;
	constexpr int MAX_MATERIALS = 4;
	constexpr int MAX_SHADERS = 1;

	struct UniformBufferObject;
	struct MaterialInfo;
	struct MeshInfo;

	class Scene;

	/// <summary>
	/// Handles rendering for the game engine.
	/// </summary>
	class Renderer
		: public Object
	{
	public: // TODO: set to private
		Window* const _window;
		
		std::vector<Texture> _textures;
		std::vector<Material> _materials;
		std::vector<Shader> _shaders;

		Viewport view;
		Color _backgroundColor;

		Scene const* _scene = nullptr;
		EntityRegistry const* _registry = nullptr;
		Entity _mainCamera = NULL_ENTITY;
	public:
		Renderer(Window* const window, Info* const appInfo);

		~Renderer();

		/// <summary>
		/// Draws a frame to the screen.
		/// </summary>
		void renderFrame();

		/// <summary>
		/// Checks if the render engine is still running.
		/// </summary>
		/// <returns>True if the engine is still running.</returns>
		bool running();
	//private:
	public: // TODO: TEMPORARILY ALL PUBLIC FOR TESTING/REFACTORING PURPOSES
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		VkQueue graphicsQueue;
		VkSurfaceKHR surface;
		VkQueue presentQueue;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
		VkRenderPass renderPass;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
		QueueFamilyIndices queueFamilyIndices;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		bool framebufferResized = false;
		uint32_t currentFrame = 0;

		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;
		
		std::vector<VkBuffer> materialBuffers;
		std::vector<VkDeviceMemory> materialBuffersMemory;
		std::vector<void*> materialBuffersMapped;

		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;

		/// <summary>
		/// Initializes the Renderer.
		/// </summary>
		virtual void init(Info* const appInfo);

		/// <summary>
		/// Starts the Renderer.
		/// </summary>
		virtual void start();

		/// <summary>
		/// Updates the Renderer.
		/// </summary>
		void update();

		void get_entity_transform(Entity const entity, Transform& transform) const;

#pragma region Components

		ID create_texture(std::string const& path);

		Texture& get_texture(ID const id);

		/// <summary>
		/// Loads a shader from the disk.
		/// </summary>
		/// <param name="path">The path to the .spv shader file. .spv files come from a compiled GLSL file.</param>
		/// <returns>The shader module.</returns>
		VkShaderModule load_shader_module(std::string const& path);

		ID create_shader(std::string const& vertexPath, std::string const& fragmentPath);

		Shader& get_shader(ID const id);

		ID create_material(ID const shaderId, ID const textureID, Color const color);

		Material& get_material(ID const id);

		void update_material(ID const id);

#pragma endregion

#pragma region Drawing

		void render_mesh(VkCommandBuffer commandBuffer, Transform const& transform, MeshComponent const& meshComponent);

#pragma endregion

		void set_main_camera(Entity const entity);

		void set_scene(Scene const* const scene, Entity const camera = NULL_ENTITY);

		void create_material_buffers();

		/// <summary>
		/// Creates a Vulkan instance.
		/// </summary>
		void create_instance(Info* const appInfo);

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
		/// Creates the depth buffer resources.
		/// </summary>
		void create_depth_resources();

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
		/// Creates an image view, so the image can be seen.
		/// </summary>
		/// <param name="image">The image to view.</param>
		/// <param name="format">The format of the image.</param>
		/// <param name="aspectFlags">The aspect flags of the image.</param>
		/// <returns>The created image view.</returns>
		VkImageView create_image_view(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

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
		/// Changes the image layout.
		/// </summary>
		/// <param name="image">The image to change.</param>
		/// <param name="format">The format of the image.</param>
		/// <param name="oldLayout">The old layout of the image.</param>
		/// <param name="newLayout">The new layout of the image.</param>
		void transition_image_layout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		/// <summary>
		/// Coppies the buffer to the image data.
		/// </summary>
		/// <param name="buffer">The buffer to copy the data from.</param>
		/// <param name="image">The image to copy the data to.</param>
		/// <param name="width">The width of the image.</param>
		/// <param name="height">The height of the image.</param>
		void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		/// <summary>
		/// Creates the image views in the swap chain.
		/// </summary>
		void create_image_views();

		/// <summary>
		/// Creates the rendering surface.
		/// </summary>
		void create_surface();

		/// <summary>
		/// Picks the hardware (GPU) to use.
		/// </summary>
		void pick_physical_device();

		/// <summary>
		/// Checks if the given device is able to do the operations we want.
		/// </summary>
		/// <param name="device">The device to check.</param>
		/// <returns>True if it is suitable.</returns>
		bool is_device_suitable(VkPhysicalDevice device);

		/// <summary>
		/// Creates the swap chain.
		/// </summary>
		virtual void create_swap_chain();

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
		/// Pick the software side of the GPU to use (driver).
		/// </summary>
		void create_logical_device();

		/// <summary>
		/// Draw a single frame to the screen.
		/// </summary>
		virtual void draw_frame();

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
		/// Fills in the create info for the debug messenger.
		/// </summary>
		/// <param name="createInfo">The create info to populate.</param>
		void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		/// <summary>
		/// Initializes the debug messenger.
		/// </summary>
		void setup_debug_messenger();

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
		/// Reads the file into a char vector.
		/// </summary>
		/// <param name="filename">The path fo the file.</param>
		/// <returns>The text from the file.</returns>
		static std::vector<char> read_file(const std::string& filename);

		/// <summary>
		/// Creates the render pass.
		/// </summary>
		void create_render_pass();

		/// <summary>
		/// Creates the frame buffers.
		/// </summary>
		void create_framebuffers();

		/// <summary>
		/// Creates the uniform buffers.
		/// </summary>
		void create_uniform_buffers();

		/// <summary>
		/// Updates the Camera uniform buffer with the Camera info.
		/// </summary>
		void update_camera(CameraComponent const& camera, Vector3 const& position, Vector3 const& rotation);

		/// <summary>
		/// Creates the descriptor set layout for uniform objects.
		/// </summary>
		void create_descriptor_set_layouts();

		/// <summary>
		/// Creates the descriptor pool.
		/// </summary>
		void create_descriptor_pool();

		void create_descriptor_sets();

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
		/// Finds the memory type.
		/// </summary>
		/// <param name="typeFilter">The filter for what types are acceptable.</param>
		/// <param name="properties">The memory properties.</param>
		/// <returns>The found type. ???</returns>
		uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		/// <summary>
		/// Creates the command pool.
		/// </summary>
		void create_command_pool(VkCommandPool& commandPool);

		/// <summary>
		/// Creates the command buffers.
		/// </summary>
		void create_command_buffers();

		/// <summary>
		/// Records the command buffer.
		/// </summary>
		/// <param name="commandBuffer">The command buffer to record.</param>
		/// <param name="imageIndex">The image to send the commands to.</param>
		virtual void record_command_buffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

		/// <summary>
		/// Create the sync objects.
		/// </summary>
		void create_sync_objects();

		/// <summary>
		/// Cleans up all of the render engine resources.
		/// </summary>
		void cleanup();
	};
}