#pragma once

// Graphics pipeline: https://vulkan-tutorial.com/en/Drawing_a_triangle/Graphics_pipeline_basics/Introduction

//#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <vector>
#include <set>
#include <optional>
#include <string>
#include <cstring>

namespace minty
{
	// https://vulkan-tutorial.com/en/Vertex_buffers/Vertex_input_description
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			return attributeDescriptions;
		}
	};

	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

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

	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	/// <summary>
	/// Handles rendering for the game engine.
	/// </summary>
	class RenderEngine
	{
	public:
		RenderEngine();

		~RenderEngine();

		void run();
	private:
		GLFWwindow* window;

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
		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;
		VkRenderPass renderPass;
		VkPipeline graphicsPipeline;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		bool framebufferResized = false;
		uint32_t currentFrame = 0;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;
		VkSampler textureSampler;
		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;

		/// <summary>
		/// Initializes the GLFW window on the screen.
		/// </summary>
		void initWindow();

		/// <summary>
		/// Initializes the Vulkan framework and all necessary components to render things to the window.
		/// </summary>
		void initVulkan();

		/// <summary>
		/// Called when the framebuffer has been resizes, eg. when the window is resized, minimizes, maximized, etc.
		/// </summary>
		/// <param name="window">The GLFW window that was resized.</param>
		/// <param name="width">The new width.</param>
		/// <param name="height">The new height.</param>
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		/// <summary>
		/// Creates a Vulkan instance.
		/// </summary>
		void createInstance();

		/// <summary>
		/// Finds the depth rendering format that can be used.
		/// </summary>
		/// <returns>The depth format.</returns>
		VkFormat findDepthFormat();

		/// <summary>
		/// Checks if the given format has a stencil component.
		/// </summary>
		/// <param name="format">The format to check.</param>
		/// <returns>True if the format has a stencil component.</returns>
		bool hasStencilComponent(VkFormat format);

		/// <summary>
		/// Finds the supported format that allows for the given image tiling and format feature flags.
		/// </summary>
		/// <param name="candidates">The potential formats to choose from.</param>
		/// <param name="tiling">The image tiling data.</param>
		/// <param name="features">The feature flags.</param>
		/// <returns>The found format.</returns>
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		/// <summary>
		/// Creates the depth buffer resources.
		/// </summary>
		void createDepthResources();

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
		void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

		/// <summary>
		/// Creates the texture image that will be rendered to the screen.
		/// </summary>
		void createTextureImage();

		/// <summary>
		/// Creates an image view, so the image can be seen.
		/// </summary>
		/// <param name="image">The image to view.</param>
		/// <param name="format">The format of the image.</param>
		/// <param name="aspectFlags">The aspect flags of the image.</param>
		/// <returns>The created image view.</returns>
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

		/// <summary>
		/// Creates the texture image view.
		/// </summary>
		void createTextureImageView();

		/// <summary>
		/// Creates the texture sampler for the GPU.
		/// </summary>
		void createTextureSampler();

		/// <summary>
		/// Start the command buffer.
		/// </summary>
		/// <returns>The command buffer that has began, and is ready for use.</returns>
		VkCommandBuffer beginSingleTimeCommands();

		/// <summary>
		/// Ends the command buffer.
		/// </summary>
		/// <param name="commandBuffer">The command buffer to end, and will be no longer used.</param>
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);

		/// <summary>
		/// Changes the image layout.
		/// </summary>
		/// <param name="image">The image to change.</param>
		/// <param name="format">The format of the image.</param>
		/// <param name="oldLayout">The old layout of the image.</param>
		/// <param name="newLayout">The new layout of the image.</param>
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		/// <summary>
		/// Coppies the buffer to the image data.
		/// </summary>
		/// <param name="buffer">The buffer to copy the data from.</param>
		/// <param name="image">The image to copy the data to.</param>
		/// <param name="width">The width of the image.</param>
		/// <param name="height">The height of the image.</param>
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		/// <summary>
		/// Creates the descriptor set layout for uniform objects.
		/// </summary>
		void createDescriptorSetLayout();

		/// <summary>
		/// Creates the image views in the swap chain.
		/// </summary>
		void createImageViews();

		/// <summary>
		/// Creates the rendering surface.
		/// </summary>
		void createSurface();

		/// <summary>
		/// Picks the hardware (GPU) to use.
		/// </summary>
		void pickPhysicalDevice();

		/// <summary>
		/// Checks if the given device is able to do the operations we want.
		/// </summary>
		/// <param name="device">The device to check.</param>
		/// <returns>True if it is suitable.</returns>
		bool isDeviceSuitable(VkPhysicalDevice device);

		/// <summary>
		/// Creates the swap chain.
		/// </summary>
		void createSwapChain();

		/// <summary>
		/// Cleans up any resources in the current swap chain.
		/// </summary>
		void cleanupSwapChain();

		/// <summary>
		/// Recreates the swap chain. Important for window changing events, such as a resize.
		/// </summary>
		void recreateSwapChain();

		/// <summary>
		/// Chooses the swap extent, given the capabilities of the surface.
		/// </summary>
		/// <param name="capabilities">The surface capabilities.</param>
		/// <returns>The chosen extent.</returns>
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		/// <summary>
		/// Chooses the swap present mode, given the available modes.
		/// </summary>
		/// <param name="availablePresentModes">The available present modes.</param>
		/// <returns>The chosen mode.</returns>
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		/// <summary>
		/// Chooses the swap surface format, givent the available formats.
		/// </summary>
		/// <param name="availableFormats">The available surface formats.</param>
		/// <returns>The chosen format.</returns>
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		/// <summary>
		/// Checks if the given device supports a swap chain.
		/// </summary>
		/// <param name="device">The device to check.</param>
		/// <returns>Details on swap chain support.</returns>
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		/// <summary>
		/// Checks if the given device has extension support.
		/// </summary>
		/// <param name="device">The device to check.</param>
		/// <returns>True if the device has extension support.</returns>
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		/// <summary>
		/// Finds the queue families.
		/// </summary>
		/// <param name="device">The device to grab the queue families from.</param>
		/// <returns>The indices of the queue families.</returns>
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		/// <summary>
		/// Pick the software side of the GPU to use (driver).
		/// </summary>
		void createLogicalDevice();

		/// <summary>
		/// The main rendering loop.
		/// </summary>
		void mainLoop();

		/// <summary>
		/// Draw a single frame to the screen.
		/// </summary>
		void drawFrame();

		/// <summary>
		/// Check if all validation layers are available for debugging.
		/// </summary>
		/// <returns>True if all validation layers are available for debugging.</returns>
		bool checkValidationLayerSupport();

		/// <summary>
		/// Gets the names of the required extensions.
		/// </summary>
		/// <returns>A vector of all of the extensions.</returns>
		std::vector<const char*> getRequiredExtensions();

		/// <summary>
		/// Fills in the create info for the debug messenger.
		/// </summary>
		/// <param name="createInfo">The create info to populate.</param>
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		/// <summary>
		/// Initializes the debug messenger.
		/// </summary>
		void setupDebugMessenger();

		/// <summary>
		/// Called when a debug message has been "printed" from Vulkan.
		/// </summary>
		/// <param name="messageSeverity">The severtity of the message.</param>
		/// <param name="messageType">The type of the message.</param>
		/// <param name="pCallbackData">The extra callback data.</param>
		/// <param name="pUserData">The user data.</param>
		/// <returns>???</returns>
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

		/// <summary>
		/// Reads the file into a char vector.
		/// </summary>
		/// <param name="filename">The path fo the file.</param>
		/// <returns>The text from the file.</returns>
		static std::vector<char> readFile(const std::string& filename);

		/// <summary>
		/// Creates the shader module, given the shader code.
		/// </summary>
		/// <param name="code">The shader code.</param>
		/// <returns>The created shader module.</returns>
		VkShaderModule createShaderModule(const std::vector<char>& code);

		/// <summary>
		/// Creates the render pass.
		/// </summary>
		void createRenderPass();

		/// <summary>
		/// Creates the graphics pipeline.
		/// </summary>
		void createGraphicsPipeline();

		/// <summary>
		/// Creates the frame buffers.
		/// </summary>
		void createFramebuffers();

		/// <summary>
		/// Creates the vertex buffer.
		/// </summary>
		void createVertexBuffer();

		/// <summary>
		/// Creates the index buffer.
		/// </summary>
		void createIndexBuffer();

		/// <summary>
		/// Creates the uniform buffers.
		/// </summary>
		void createUniformBuffers();

		/// <summary>
		/// Updates the uniform buffer with new rotation values.
		/// </summary>
		/// <param name="currentImage">The image in which to update the buffer for.</param>
		void updateUniformBuffer(uint32_t currentImage);

		/// <summary>
		/// Creates the descriptor pool.
		/// </summary>
		void createDescriptorPool();

		/// <summary>
		/// Creates the descriptor sets.
		/// </summary>
		void createDescriptorSets();

		/// <summary>
		/// Creates a buffer.
		/// </summary>
		/// <param name="size">The size of the buffer.</param>
		/// <param name="usage">How the buffer will be used.</param>
		/// <param name="properties">The memory properties.</param>
		/// <param name="buffer">The buffer to create.</param>
		/// <param name="bufferMemory">The memory to be associated with the buffer.</param>
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

		/// <summary>
		/// Copies the buffer from source to destination.
		/// </summary>
		/// <param name="srcBuffer">The buffer to copy from.</param>
		/// <param name="dstBuffer">The buffer to copy to.</param>
		/// <param name="size">The size of the buffer to copy.</param>
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		/// <summary>
		/// Finds the memory type.
		/// </summary>
		/// <param name="typeFilter">The filter for what types are acceptable.</param>
		/// <param name="properties">The memory properties.</param>
		/// <returns>The found type. ???</returns>
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		/// <summary>
		/// Creates the command pool.
		/// </summary>
		void createCommandPool();

		/// <summary>
		/// Creates the command buffers.
		/// </summary>
		void createCommandBuffers();

		/// <summary>
		/// Records the command buffer.
		/// </summary>
		/// <param name="commandBuffer">The command buffer to record.</param>
		/// <param name="imageIndex">The image to send the commands to.</param>
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

		/// <summary>
		/// Create the sync objects.
		/// </summary>
		void createSyncObjects();

		/// <summary>
		/// Cleans up all of the render engine resources.
		/// </summary>
		void cleanup();
	};
}