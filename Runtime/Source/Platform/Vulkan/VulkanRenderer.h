#pragma once
#include "Minty/Render/Renderer.h"

#include "Minty/Core/ApplicationInfo.h"
#include "Minty/Library/Vulkan.h"
#include "Minty/Render/Shader.h"

#include "Platform/Vulkan/VulkanImage.h"
#include "Platform/Vulkan/VulkanRenderTarget.h"
#include "Platform/Vulkan/VulkanScissor.h"
#include "Platform/Vulkan/VulkanViewport.h"

#include <array>
#include <unordered_set>
#include <vector>

namespace Minty
{
	class VulkanRenderer
	{
#pragma region Structures

	public:
		/// <summary>
		/// Holds the data for one frame in the flight.
		/// </summary>
		struct Frame
		{
			VkCommandBuffer commandBuffer;

			VkSemaphore imageAvailableSemaphore;
			VkSemaphore renderFinishedSemaphore;
			VkFence inFlightFence;
		};

		/// <summary>
		/// Holds the indices to the queue families to be used.
		/// </summary>
		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			/// <summary>
			/// Checks if this QueueFamilyIndices has a valid index for each family.
			/// </summary>
			/// <returns>True when all indices are valid.</returns>
			Bool is_complete() const
			{
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

#pragma endregion

	private:
		static VkInstance s_instance;
		static VkPhysicalDevice s_physicalDevice;
		static QueueFamilyIndices s_queueFamilyIndices;
		static VkDevice s_device;
		static VkQueue s_graphicsQueue; // queue for graphics operations
		static VkQueue s_presentQueue; // queue for rendering to the screen
		static VkSurfaceKHR s_surface;
		static VkSurfaceFormatKHR s_swapchainSurfaceFormat;
		static VkExtent2D s_swapchainExtent;
		static VkSwapchainKHR s_swapchain;
		static uint32_t s_imageIndex;

		static std::vector<Owner<VulkanImage>> s_swapchainImages;
		static VkRenderPass s_renderPass;
		static std::unordered_set <VulkanRenderTarget*> s_renderTargets;
		static Owner<VulkanImage> s_depthImage;
		static Ref<VulkanViewport> s_viewport;
		static Ref<VulkanScissor> s_scissor;

		static VkCommandPool s_commandPool;
		
		static std::array<Frame, MAX_FRAMES_IN_FLIGHT> s_frames;
		static Size s_currentFrame;

#pragma region Init

	public:
		static void initialize(const RendererBuilder& builder);

		static void shutdown();

	private:
		static void initialize_frame(Frame& frame);

		static void destroy_frame(Frame& frame);

		static void recreate_swapchain();

#pragma endregion

#pragma region Rendering

	public:
		static int start_frame(const Ref<RenderTarget> renderTarget);

		static void end_frame();

		static void draw_vertices(const UInt vertexCount);

		static void draw_instances(const UInt instanceCount, const UInt vertexCount = 0);

		static void draw_indices(const UInt indexCount);

		static void sync();

		static void refresh();

		static void register_target(VulkanRenderTarget* const renderTarget);

		static void unregister_target(VulkanRenderTarget* const renderTarget);

#pragma endregion

#pragma region Get Set

	public:
		static Ref<Viewport> get_viewport() { return s_viewport; }

		static Ref<Scissor> get_scissor() { return s_scissor; }

		static VkInstance get_instance() { return s_instance; }

		static VkPhysicalDevice get_physical_device() { return s_physicalDevice; }

		static VkDevice get_device() { return s_device; }

		static VkSurfaceFormatKHR get_swapchain_surface_format() { return s_swapchainSurfaceFormat; }

		static VkExtent2D get_swapchain_extent() { return s_swapchainExtent; }

		static VkRenderPass get_render_pass() { return s_renderPass; }

		static VkQueue get_graphics_queue() { return s_graphicsQueue; }

		static VkCommandPool get_command_pool() { return s_commandPool; }

		static VkCommandBuffer get_command_buffer() { return s_frames.at(s_currentFrame).commandBuffer; }

		static Size get_current_frame_index() { return s_currentFrame; }

	private:
		static Frame& get_current_frame() { return s_frames.at(s_currentFrame); }

#pragma endregion

#pragma region Create

	public:
		static Owner<RenderTarget> create_render_target();

#pragma endregion

#pragma region Utility

#pragma region Validation Layers

	private:
		static void setup_debug_messenger();

		static void cleanup_debug_messenger();

#pragma endregion

#pragma region Instance

	private:
		static void create_instance(const Minty::ApplicationInfo& applicationInfo);

		static void destroy_instance();

		static std::vector<VkExtensionProperties> get_instance_extensions();

#pragma endregion

#pragma region Surface

	private:
		static VkSurfaceKHR create_surface(const Ref<Window> window);

		static void destroy_surface(const VkSurfaceKHR surface);

#pragma endregion

#pragma region Physical Device

	private:
		static void select_physical_device();

		static int rate_device_suitability(const VkPhysicalDevice physicalDevice);

		static SwapChainSupportDetails query_swap_chain_support(const VkPhysicalDevice physicalDevice);

		static VkSurfaceFormatKHR select_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats, const VkFormat format = VK_FORMAT_B8G8R8A8_SRGB, const VkColorSpaceKHR colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);

		static VkExtent2D select_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);

		static VkPresentModeKHR select_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes, const VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR);

#pragma endregion

#pragma region Queue Families

	private:
		static QueueFamilyIndices find_queue_families(const VkPhysicalDevice physicalDevice);

		static VkQueue get_device_queue(const uint32_t index);

#pragma endregion

#pragma region Logical Device

	private:
		static void create_device();

		static void destroy_device();

		static void wait_until_device_idle();

#pragma endregion

#pragma region Swapchain

	private:
		static void create_swapchain(const VkSurfaceFormatKHR surfaceFormat, const VkExtent2D extent, const VkPresentModeKHR presentMode);

		static void destroy_swapchain();

		static std::vector<VkImage> get_swapchain_images();

		static VkResult get_next_swapchain_image(const VkSemaphore waitSemaphore, uint32_t& index);

#pragma endregion

#pragma region Format

	private:
		static VkFormat find_supported_format(const std::vector<VkFormat>& candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features);

#pragma endregion

#pragma region Image

	public:
		static VkImage create_image(const uint32_t width, const uint32_t height, const VkImageType type, const VkFormat format, const VkImageTiling tiling, const VkImageUsageFlags usage);

		static void create_image_and_memory(const uint32_t width, const uint32_t height, const VkImageType type, const VkFormat format, const VkImageTiling tiling, const VkImageUsageFlags usage, const VkMemoryPropertyFlags memoryProperties, VkImage& image, VkDeviceMemory& memory);

		static void destroy_image(const VkImage image);

		static void bind_image_memory(const VkImage image, const VkDeviceMemory memory);

		static void transition_image_layout(const VkImage image, const VkFormat format, const VkImageLayout oldLayout, const VkImageLayout newLayout);

#pragma endregion

#pragma region Image View

	public:
		static VkImageView create_image_view(const VkImage image, const VkFormat format, const VkImageAspectFlags aspectFlags, const VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D);

		static std::vector<VkImageView> create_image_views(const std::vector<VkImage>& images, const VkFormat format, const VkImageAspectFlags aspectFlags, const VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D);

		static void destroy_image_view(const VkImageView imageView);

#pragma endregion

#pragma region Sampler

	public:
		static VkSampler create_sampler(const VkFilter magFilter = VK_FILTER_LINEAR, const VkFilter minFilter = VK_FILTER_LINEAR, const VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT, const VkBorderColor borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK, const Bool normalizedCoordinates = true);

		static void destroy_sampler(const VkSampler sampler);

#pragma endregion

#pragma region Depth

	private:
		static VkFormat find_depth_format();

		static Bool has_stencil_component(const VkFormat format);

#pragma endregion

#pragma region Shader Module

	public:
		static VkShaderModule create_shader_module(const std::vector<Minty::Char>& code);

		static VkShaderModule create_shader_module(const Path& path);

		static void destroy_shader_module(const VkShaderModule shaderModule);

#pragma endregion

#pragma region Render Pass

	public:
		static void create_render_pass();

		static void destroy_render_pass();

#pragma endregion

#pragma region Frame Buffer

	public:
		static VkFramebuffer create_framebuffer(const VkRenderPass renderPass, const VkImageView attachment, const VkExtent2D extent);

		static void destroy_framebuffer(const VkFramebuffer framebuffer);

#pragma endregion

#pragma region Command PoolData

	private:
		static void create_command_pool(const uint32_t queueFamilyIndex);

		static void destroy_command_pool();

#pragma endregion

#pragma region Command Buffer

	public:
		static VkCommandBuffer create_command_buffer();

		static void destroy_command_buffer(const VkCommandBuffer commandBuffer);

		static void begin_command_buffer(const VkCommandBuffer commandBuffer);

		static void begin_command_buffer_temp(const VkCommandBuffer commandBuffer);

		static void end_command_buffer(const VkCommandBuffer commandBuffer);

		static VkCommandBuffer begin_command_buffer_single();

		static void end_command_buffer_single(VkCommandBuffer& commandBuffer, const VkQueue queue);

		static void reset_command_buffer(const VkCommandBuffer commandBuffer);

		// submit and presentation
		static void submit_command_buffer(const VkCommandBuffer commandBuffer, const VkQueue queue, const VkSemaphore waitSemaphore, const VkSemaphore signalSemaphore, const VkFence inFlightFence);

		static void submit_command_buffer(const Frame& frame, const VkQueue queue);

		static void submit_command_buffer(const VkCommandBuffer commandBuffer, const VkQueue queue);

#pragma region Commands

	public:
		static void begin_render_pass(const VkCommandBuffer commandBuffer, const VkRenderPass renderPass, const VkFramebuffer framebuffer, const VkRect2D renderArea, const VkClearColorValue clearColor);

		static void end_render_pass(const VkCommandBuffer commandBuffer);

		static void bind_pipeline(const VkCommandBuffer commandBuffer, const VkPipeline graphicsPipeline, const VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);

		static void bind_descriptor_set(const VkCommandBuffer commandBuffer, const VkPipelineLayout graphicsPipelineLayout, const VkDescriptorSet descriptorSet);

		static void bind_viewport(const VkCommandBuffer commandBuffer, const VkViewport& viewport);

		static void bind_scissor(const VkCommandBuffer commandBuffer, const VkRect2D& scissor);

		static void bind_vertex_buffer(const VkCommandBuffer commandBuffer, const VkBuffer buffer, UInt const binding = 0);

		static void bind_index_buffer(const VkCommandBuffer commandBuffer, const VkBuffer buffer);

		static void draw_vertices(const VkCommandBuffer commandBuffer, const uint32_t count);

		static void draw_instances(const VkCommandBuffer commandBuffer, const uint32_t count, const uint32_t vertexCount);

		static void draw_indices(const VkCommandBuffer commandBuffer, const uint32_t count);

#pragma endregion

#pragma endregion

#pragma region Multithreading

	private:
		static VkSemaphore create_semaphore();

		static void destroy_semaphore(const VkSemaphore semaphore);

		static VkFence create_fence();

		static void destroy_fence(const VkFence fence);

		static void wait_for_fence(VkFence& fence);

		static void reset_fence(VkFence& fence);

#pragma endregion

#pragma region Presentation

	private:
		static VkResult present_frame(uint32_t& imageIndex, const VkSemaphore signalSemaphore);

#pragma endregion

#pragma region Buffer

	public:
		static VkBuffer create_buffer(const VkDeviceSize size, const VkBufferUsageFlags usage);

		static void create_buffer_and_memory(const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& memory);

		static void destroy_buffer(const VkBuffer buffer);

		static void destroy_buffer_and_memory(const VkBuffer buffer, const VkDeviceMemory memory);

		static void get_buffer_memory_requirements(const VkBuffer buffer, VkMemoryRequirements& requirements);

		static void bind_buffer_memory(const VkBuffer buffer, const VkDeviceMemory memory);

		static void copy_buffer_to_buffer(const VkQueue queue, const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize size);

		static void copy_buffer_to_image(const VkQueue queue, const VkBuffer srcBuffer, const VkImage dstImage, const uint32_t width, const uint32_t height);

#pragma endregion

#pragma region Memory

	public:
		static uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		static VkDeviceMemory allocate_memory(const VkDeviceSize size, const uint32_t memoryTypeIndex);

		static void free_memory(const VkDeviceMemory memory);

		static void* map_memory(const VkDeviceMemory memory, const VkDeviceSize offset, const VkDeviceSize size);

		static void unmap_memory(const VkDeviceMemory memory);

		static void set_memory(const VkDeviceMemory memory, const void* const data, const VkDeviceSize offset, const VkDeviceSize size);

#pragma endregion

#pragma region Descriptor Set



#pragma endregion

#pragma region Conversions

	public:
		static VkFormat format_to_vulkan(const Minty::Format format);
		
		static VkBufferUsageFlags buffer_usage_to_vulkan(const Minty::BufferUsage bufferUsage);

		static VkShaderStageFlags shader_stage_to_vulkan(const Minty::ShaderStage shaderStage);

		static VkDescriptorType descriptor_type_to_vulkan(const Minty::ShaderInputType type);

		static VkFormat type_to_vulkan(const Minty::Type type);

		static VkImageType image_type_to_vulkan(const Minty::ImageType type);

		static VkImageTiling image_tiling_to_vulkan(const Minty::ImageTiling tiling);

		static VkSamplerAddressMode address_mode_to_vulkan(const Minty::ImageAddressMode addressMode);

		static VkImageAspectFlags image_aspect_to_vulkan(const Minty::ImageAspect aspect);

		static VkImageUsageFlags image_usage_to_vulkan(const Minty::ImageUsage usage);

		static VkPrimitiveTopology primitive_topology_to_vulkan(const Minty::ShaderPrimitiveTopology topology);

		static VkCullModeFlags cull_mode_to_vulkan(const Minty::ShaderCullMode mode);

		static VkFrontFace front_face_to_vulkan(const Minty::ShaderFrontFace frontFace);

		static VkPolygonMode polygon_mode_to_vulkan(const Minty::ShaderPolygonMode mode);

		static VkVertexInputRate input_rate_to_vulkan(const Minty::ShaderInputRate rate);

#pragma endregion

#pragma endregion
};
}