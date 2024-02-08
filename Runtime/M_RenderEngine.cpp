#include "pch.h"
#include "M_RenderEngine.h"

#include "M_RendererBuilder.h"
#include "M_DrawCallObjectInfo.h"
#include "M_SpritePushData.h"

#include "M_Console.h"
#include "M_Engine.h"
#include "M_File.h"
#include "M_Scene.h"
#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"
#include "M_Encoding.h"

#include "M_RenderSystem.h"

#include "M_TransformComponent.h"
#include "M_UITransformComponent.h"
#include "M_SpriteComponent.h"
#include "M_CameraComponent.h"
#include "M_MeshComponent.h"
#include "M_RenderableComponent.h"
#include "M_RelationshipComponent.h"

#include "M_Asset.h"
#include "M_Parse.h"
#include "M_Text.h"

#include "M_Vector.h"
#include "M_Matrix.h"
#include "M_Builtin.h"

//#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp
#include <fstream> // Necessary for loading data from files
#include <chrono>
#include <filesystem>
#include <format>
#include <sstream>
#include <unordered_map>

using namespace minty;
using namespace minty;
using namespace minty::vk;
using namespace minty::Builtin;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
bool const enableValidationLayers = false;
#else
bool const enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

RenderEngine::RenderEngine(Window* const window)
	: _builder(nullptr)
	, _window(window)
	, _boundIds()
	, _view()
	, _backgroundColor({ 250, 220, 192, 255 }) // light tan color
	, _initialized()
	, _frame(0)
{
	for (size_t i = 0; i < _boundIds.size(); i++)
	{
		_boundIds[i] = ERROR_ID;
	}
}

RenderEngine::~RenderEngine()
{
	destroy();
}

void minty::RenderEngine::init(RenderEngineBuilder const& builder)
{
	// keep track of builder
	_builder = &builder;

	create_instance();
	setup_debug_messenger();
	create_surface();
	pick_physical_device();
	create_logical_device();
	create_swap_chain();
	create_image_views();
	create_render_pass();
	_queueFamilyIndices = find_queue_families(_physicalDevice);
	create_command_pool(_commandPool);
	create_depth_resources();
	create_framebuffers();
	create_command_buffers();
	create_sync_objects();

	_initialized = true;

	// builder no longer needed
	_builder = nullptr;
}

void RenderEngine::render_frame()
{
	// wait until previous draw has completed
	vkWaitForFences(_device, 1, &_inFlightFences[_frame], VK_TRUE, UINT64_MAX);

	// recreate swap chain check
	uint32_t imageIndex;
	VkResult r = vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, _imageAvailableSemaphores[_frame], VK_NULL_HANDLE, &imageIndex);

	if (r == VK_ERROR_OUT_OF_DATE_KHR) {
		recreate_swap_chain();
		return;
	}
	else if (r != VK_SUCCESS && r != VK_SUBOPTIMAL_KHR) {
		Error::abort("Failed to acquire swap chain image.");
	}

	// reset the fence states so they can be used to wait again
	vkResetFences(_device, 1, &_inFlightFences[_frame]);

	// reset command buffer
	vkResetCommandBuffer(_commandBuffers[_frame], 0);

	// record the command buffer so we know what to do to render stuff to the screen
	record_command_buffer(_commandBuffers[_frame], imageIndex);

	// update uniform information
	//update_uniform_buffer();

	// submit the command buffer
	VkSubmitInfo submitInfo
	{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO
	};

	// specify how to wait, in this case, with the semaphores
	VkSemaphore waitSemaphores[] = { _imageAvailableSemaphores[_frame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	// set number of command buffers
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffers[_frame];

	// specift what semaphores to signal when complete
	VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[_frame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	// submit the buffer
	VK_ASSERT(vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFences[_frame]), "Failed to submit draw command buffer.");

	// submit to swap chain so it will show up on the screen
	VkPresentInfoKHR presentInfo
	{
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = signalSemaphores
	};

	// specify which swap chain to use (only 1 normally)
	VkSwapchainKHR swapChains[] = { _swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	// present the visual to the screen
	r = vkQueuePresentKHR(_presentQueue, &presentInfo);

	if (r == VK_ERROR_OUT_OF_DATE_KHR || r == VK_SUBOPTIMAL_KHR || _framebufferResized) {
		_framebufferResized = false;
		recreate_swap_chain();
	}
	else if (r != VK_SUCCESS) {
		Error::abort("Failed to present swap chain image.");
	}

	// move to next frame
	_frame++;
	if (_frame == MAX_FRAMES_IN_FLIGHT) _frame = 0;
}

bool RenderEngine::is_running() const
{
	return _window->is_open();
}

bool minty::RenderEngine::is_initialized() const
{
	return _initialized;
}

void RenderEngine::create_instance()
{
	// check if we can use validation layers
	if (enableValidationLayers && !check_validation_layer_support())
	{
		Error::abort("Validation layers requested, but not available.");
	}

	// get glfw extensions
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	VkApplicationInfo appInfo;

	Info const* const info = _builder->info;

	if (info)
	{
		// custom info
		appInfo = info->get_application_info();
	}
	else
	{
		// default
		appInfo =
		{
			.pApplicationName = "Minty Application",
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName = MINTY_NAME,
			.engineVersion = MINTY_VERSION,
			.apiVersion = MINTY_API_VERSION,
		};
	}

	// create info, telling vulkan which global extensions and validation layers we want to use
	VkInstanceCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
	};

	auto extensions = get_required_extensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populate_debug_messenger_create_info(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		Error::abort("failed to create instance!");
	}
}

VkFormat RenderEngine::find_depth_format() {
	return find_supported_format(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

bool RenderEngine::has_stencil_component(VkFormat format) {
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkFormat RenderEngine::find_supported_format(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	Error::abort("Failed to find_animation supported format.");

	return VkFormat();
}

void RenderEngine::create_depth_resources()
{
	VkFormat depthFormat = find_depth_format();

	create_image(_swapChainExtent.width, _swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _depthImage, _depthImageMemory);
	_depthImageView = create_image_view(_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void RenderEngine::create_image(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		Error::abort("Failed to create image.");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(_device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = find_memory_type(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		Error::abort("Failed to allocate image memory.");
	}

	vkBindImageMemory(_device, image, imageMemory, 0);
}

VkDevice minty::RenderEngine::get_device() const
{
	return _device;
}

VkPhysicalDevice minty::RenderEngine::get_physical_device() const
{
	return _physicalDevice;
}

VkRenderPass minty::RenderEngine::get_render_pass() const
{
	return _renderPass;
}

uint32_t minty::RenderEngine::get_frame() const
{
	return _frame;
}

float minty::RenderEngine::get_aspect_ratio() const
{
	return static_cast<float>(_swapChainExtent.width) / static_cast<float>(_swapChainExtent.height);
}

VkImageView RenderEngine::create_image_view(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		Error::abort("Failed to create texture image view.");
	}

	return imageView;
}

VkShaderModule minty::RenderEngine::load_shader_module(String const& path) const
{
	return create_shader_module(Asset::load_chars(path));
}

VkShaderModule minty::RenderEngine::create_shader_module(std::vector<char> const& code) const
{
	VkShaderModuleCreateInfo createInfo
	{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size(),
		.pCode = reinterpret_cast<const uint32_t*>(code.data())
	};

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		Error::abort("Failed to create shader module.");
	}

	return shaderModule;
}

VkCommandBuffer RenderEngine::begin_single_time_commands(VkCommandPool commandPool) {
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void RenderEngine::end_single_time_commands(VkCommandBuffer commandBuffer, VkCommandPool commandPool) {
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(_graphicsQueue);

	vkFreeCommandBuffers(_device, commandPool, 1, &commandBuffer);
}

void RenderEngine::change_image_layout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
	VkCommandBuffer commandBuffer = begin_single_time_commands(_commandPool);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;

	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	barrier.srcAccessMask = 0; // TODO
	barrier.dstAccessMask = 0; // TODO

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	end_single_time_commands(commandBuffer, _commandPool);
}

void RenderEngine::copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
	VkCommandBuffer commandBuffer = begin_single_time_commands(_commandPool);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(
		commandBuffer,
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	end_single_time_commands(commandBuffer, _commandPool);
}

void RenderEngine::create_image_views()
{
	// resize to fit all image views
	_swapChainImageViews.resize(_swapChainImages.size());

	for (uint32_t i = 0; i < _swapChainImages.size(); i++) {
		_swapChainImageViews[i] = create_image_view(_swapChainImages[i], _swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}

void RenderEngine::create_surface()
{
	// create window surface that vulkan can use to draw
	if (glfwCreateWindowSurface(instance, _window->get_raw(), nullptr, &_surface) != VK_SUCCESS) {
		Error::abort("Failed to create window surface.");
	}
}

void RenderEngine::pick_physical_device()
{
	// find number of hardware GPUs
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	// if zero, error
	if (deviceCount == 0)
	{
		Error::abort("Failed to find_animation GPU's with Vulkan support.");
	}

	// get devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	// find a suitable device, pick the first one
	for (const auto& device : devices) {
		if (is_device_suitable(device)) {
			_physicalDevice = device;
			break;
		}
	}

	// none found
	if (_physicalDevice == VK_NULL_HANDLE) {
		Error::abort("Failed to find_animation a suitable GPU.");
	}
}

bool RenderEngine::is_device_suitable(VkPhysicalDevice device)
{
	// define requirements for the GPU we want

	//VkPhysicalDeviceProperties deviceProperties;
	//vkGetPhysicalDeviceProperties(device, &deviceProperties);
	//VkPhysicalDeviceFeatures deviceFeatures;
	//vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	// for ANY GPU, return true

	// find GPU that supports queue families
	QueueFamilyIndices indices = find_queue_families(device);

	bool extensionsSupported = check_device_extension_support(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = query_swap_chain_support(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy && supportedFeatures.shaderSampledImageArrayDynamicIndexing;
}

void RenderEngine::create_swap_chain()
{
	SwapChainSupportDetails swapChainSupport = query_swap_chain_support(_physicalDevice);

	// get settings
	VkSurfaceFormatKHR surfaceFormat = choose_swap_surface_format(swapChainSupport.formats);
	VkPresentModeKHR presentMode = choose_swap_present_mode(swapChainSupport.presentModes);
	VkExtent2D extent = choose_swap_extent(swapChainSupport.capabilities);

	// how many images in swap chain
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1; // recommended to have min + 1

	// do not exceed max image count
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo
	{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = _surface,
		.minImageCount = imageCount,
		.imageFormat = surfaceFormat.format,
		.imageColorSpace = surfaceFormat.colorSpace,
		.imageExtent = extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	};

	QueueFamilyIndices indices = find_queue_families(_physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	// set to no transform rotations or anything
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

	// ignore alpha
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	// ignore pixels that we can't see
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	// reference old swap chain if made a new one from resizing, here
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	// create swap chain
	if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
		Error::abort("Failed to create swap chain.");
	}

	vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, nullptr);
	_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, _swapChainImages.data());

	_swapChainImageFormat = surfaceFormat.format;
	_swapChainExtent = extent;

	// update viewport and scissor
	_view.set_extent(extent.width, extent.height);
}

void RenderEngine::cleanup_swap_chain()
{
	vkDestroyImageView(_device, _depthImageView, nullptr);
	vkDestroyImage(_device, _depthImage, nullptr);
	vkFreeMemory(_device, _depthImageMemory, nullptr);

	for (size_t i = 0; i < _swapChainFramebuffers.size(); i++) {
		vkDestroyFramebuffer(_device, _swapChainFramebuffers[i], nullptr);
	}

	for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
		vkDestroyImageView(_device, _swapChainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(_device, _swapChain, nullptr);
}

void RenderEngine::recreate_swap_chain()
{
	// on window minimize, pause program until un-minimized
	_window->refresh();
	while (_window->get_frame_width() == 0 || _window->get_frame_height() == 0) {
		_window->refresh();
		glfwWaitEvents();
	}

	// wait for device to be done rendering
	vkDeviceWaitIdle(_device);

	cleanup_swap_chain();

	// recreate the swap chain
	create_swap_chain();
	create_image_views();
	create_depth_resources();
	create_framebuffers();
}

VkExtent2D RenderEngine::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		_window->refresh();

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(_window->get_frame_width()),
			static_cast<uint32_t>(_window->get_frame_height())
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

VkPresentModeKHR RenderEngine::choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	// essentially, check if v-sync exists, then use it
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR; // v-sync
}

VkSurfaceFormatKHR RenderEngine::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	for (const auto& availableFormat : availableFormats) {
		// VK_FORMAT_R8G8B8A8_UINT?
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

SwapChainSupportDetails RenderEngine::query_swap_chain_support(VkPhysicalDevice device) {
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

bool RenderEngine::check_device_extension_support(VkPhysicalDevice device)
{
	// check if all required extensions are there
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<String> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

QueueFamilyIndices RenderEngine::find_queue_families(VkPhysicalDevice device)
{
	// find graphics familes for queueing up commands
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

// pick the software GPU to use (driver)
void RenderEngine::create_logical_device()
{
	QueueFamilyIndices indices = find_queue_families(_physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceDescriptorIndexingFeaturesEXT deviceIndexingFeatures
	{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT,
		.shaderUniformBufferArrayNonUniformIndexing = VK_TRUE,
		.shaderSampledImageArrayNonUniformIndexing = VK_TRUE,
		.shaderStorageBufferArrayNonUniformIndexing = VK_TRUE,
		.descriptorBindingPartiallyBound = VK_TRUE,
		.descriptorBindingVariableDescriptorCount = VK_TRUE,
		.runtimeDescriptorArray = VK_TRUE,
	};

	VkPhysicalDeviceFeatures const deviceFeatures
	{
		.samplerAnisotropy = VK_TRUE,
		.shaderSampledImageArrayDynamicIndexing = VK_TRUE
	};

	VkPhysicalDeviceFeatures2 const deviceFeatures2
	{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext = &deviceIndexingFeatures,
		.features = deviceFeatures
	};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	//createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.pEnabledFeatures = nullptr; // must be null if using VkPhysicalDeviceFeatures2
	createInfo.pNext = &deviceFeatures2;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS) {
		Error::abort("Failed to create logical device.");
	}

	vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
	vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);
}

void minty::RenderEngine::draw_scene(VkCommandBuffer commandBuffer)
{
	// sort sprites so they render in the correct order, since Z does not matter
	_registry->sort<SpriteComponent>([](SpriteComponent const& left, SpriteComponent const& right)
		{
			return left.order < right.order;
		});

	TransformComponent const* transformComponent;

	// draw all meshes in the scene
	for (auto&& [entity, renderable, mesh] : _registry->view<RenderableComponent const, MeshComponent>().each())
	{
		// get transform for entity
		transformComponent = _registry->try_get<TransformComponent>(entity);

		if (transformComponent)
		{
			// render the entity's mesh at the position
			draw_mesh(commandBuffer, transformComponent->globalMatrix, mesh);
		}
		else
		{
			// if no transform, use empty matrix
			draw_mesh(commandBuffer, Matrix4(1.0f), mesh);
		}
	}

	// draw all world sprites in the scene
	for (auto&& [entity, renderable, transform, sprite] : _registry->view<RenderableComponent const, TransformComponent const, SpriteComponent const>().each())
	{
		draw_sprite(commandBuffer, transform, sprite);
	}

	// sort UITransforms so that it matches order of sprites for rendering
	_registry->sort<UITransformComponent, SpriteComponent>();

	// draw all UI in scene
	for (auto&& [entity, renderable, ui, sprite] : _registry->view<RenderableComponent const, UITransformComponent const, SpriteComponent const>().each())
	{
		draw_ui(commandBuffer, ui, sprite);
	}

	// unbind any shaders used
	bind(commandBuffer, ERROR_ID);
	//bind_shader(commandBuffer, nullptr);
}

void minty::RenderEngine::draw_mesh(VkCommandBuffer commandBuffer, Matrix4 const& transformationMatrix, MeshComponent const& meshComponent)
{
	// do nothing if null mesh
	if (meshComponent.meshId == ERROR_ID)
	{
		return;
	}

	Mesh& mesh = _renderSystem->get_mesh(meshComponent.meshId);

	// do nothing if empty mesh
	if (mesh.empty())
	{
		return;
	}

	// bind the material, which will bind the shader and update its values
	//bind_shader(commandBuffer, &shader);
	bind(commandBuffer, meshComponent.materialId);

	// bind vertex data
	VkBuffer vertexBuffers[] = { get_buffer(mesh.get_vertex_buffer_id()) };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, get_buffer(mesh.get_index_buffer_id()), 0, mesh.get_index_type());

	// send push constants so we know where to draw
	Matrix4 tmatrix = transformationMatrix;
	DrawCallObject3D info
	{
		.transform = tmatrix,
	};
	Shader& shader = _renderSystem->get_shader_from_material_id(meshComponent.materialId);
	shader.update_push_constant(commandBuffer, &info, sizeof(DrawCallObject3D));

	// draw
	vkCmdDrawIndexed(commandBuffer, mesh.get_index_count(), 1, 0, 0, 0);
}

void minty::RenderEngine::draw_sprite(VkCommandBuffer commandBuffer, TransformComponent const& transformComponent, SpriteComponent const& spriteComponent)
{
	// if no sprite, skip and draw nothing
	if(spriteComponent.spriteId == ERROR_ID)
	{
		return;
	}

	// get the sprite
	Sprite const& sprite = _renderSystem->get_sprite(spriteComponent.spriteId);

	// bind the material the sprite is using
	bind(commandBuffer, sprite.get_material_id());

	// get the data
	SpritePushData pushData
	{
		.transform = transformComponent.globalMatrix,
		.minCoords = sprite.get_min_coords(),
		.maxCoords = sprite.get_max_coords(),
		.pivot = sprite.get_pivot(),
		.size = spriteComponent.size,
	};

	// push data to shader
	Shader& shader = _renderSystem->get_shader_from_material_id(sprite.get_material_id());
	shader.update_push_constant(commandBuffer, &pushData, sizeof(SpritePushData));

	// draw
	vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}

void minty::RenderEngine::draw_ui(VkCommandBuffer commandBuffer, UITransformComponent const& uiComponent, SpriteComponent const& spriteComponent)
{
	Console::todo("fix draw_ui");
	//// get the sprite
	//Sprite const& sprite = get_sprite(spriteComponent.spriteId);

	//// bind the material the sprite is using
	//bind(commandBuffer, sprite.get_material_id());

	//// adjust info based on anchor and pivot
	//float width = static_cast<float>(_window->get_width());
	//float height = static_cast<float>(_window->get_height());
	//float left, top, right, bottom;

	//int anchor = static_cast<int>(uiComponent.anchorMode);

	//// do x, then y
	//if (anchor & static_cast<int>(AnchorMode::Left))
	//{
	//	left = uiComponent.x / width;
	//	right = (uiComponent.x + uiComponent.width) / width;
	//}
	//else if (anchor & static_cast<int>(AnchorMode::Center))
	//{
	//	left = uiComponent.x / width + 0.5f;
	//	right = (uiComponent.x + uiComponent.width) / width + 0.5f;
	//}
	//else if (anchor & static_cast<int>(AnchorMode::Right))
	//{
	//	left = uiComponent.x / width + 1.0f;
	//	right = (uiComponent.x + uiComponent.width) / width + 1.0f;
	//}
	//else
	//{
	//	left = uiComponent.left;
	//	right = uiComponent.right;
	//}

	//if (anchor & static_cast<int>(AnchorMode::Top))
	//{
	//	top = uiComponent.y / height;
	//	bottom = (uiComponent.y + uiComponent.height) / height;
	//}
	//else if (anchor & static_cast<int>(AnchorMode::Middle))
	//{
	//	top = uiComponent.y / height + 0.5f;
	//	bottom = (uiComponent.y + uiComponent.height) / height + 0.5f;
	//}
	//else if (anchor & static_cast<int>(AnchorMode::Bottom))
	//{
	//	top = uiComponent.y / height + 1.0f;
	//	bottom = (uiComponent.y + uiComponent.height) / height + 1.0f;
	//}
	//else
	//{
	//	top = uiComponent.top;
	//	bottom = uiComponent.bottom;
	//}

	//// set draw call info
	//DrawCallObjectUI info
	//{
	//	.materialId = sprite.get_material_id(),
	//	.layer = spriteComponent.layer,
	//	.coords = Vector4(sprite.get_min_coords(), sprite.get_max_coords()),
	//	.pos = Vector4(left, top, right, bottom),
	//};

	//Shader& shader = get_shader_from_material_id(sprite.get_material_id());
	//shader.update_push_constant(commandBuffer, &info, sizeof(DrawCallObjectUI));

	//// draw
	//vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}

bool RenderEngine::check_validation_layer_support()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> RenderEngine::get_required_extensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void RenderEngine::populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = debug_callback
	};
}

void RenderEngine::setup_debug_messenger() {
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populate_debug_messenger_create_info(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS) {
		Error::abort("Failed to set up debug messenger.");
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL RenderEngine::debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	// change colors based on severity
	if (messageSeverity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		Console::error(pCallbackData->pMessage);
	}
	else if (messageSeverity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		Console::warn(pCallbackData->pMessage);
	}
	else if (messageSeverity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
	{
		Console::info(pCallbackData->pMessage);
	}
	else
	{
		Console::info(pCallbackData->pMessage);
		//Console::log(pCallbackData->pMessage);
	}

	return VK_FALSE;
}

void RenderEngine::create_render_pass()
{
	// need to tell vulkan how much of what to use for rendering

	VkAttachmentDescription colorAttachment
	{
		.format = _swapChainImageFormat,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
	};

	VkAttachmentReference colorAttachmentRef
	{
		.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = find_depth_format();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency
	{
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.srcAccessMask = 0,
		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
	};
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(_device, &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS) {
		Error::abort("Failed to create render pass.");
	}
}

void RenderEngine::create_framebuffers()
{
	// resize to hold all framebuffers
	_swapChainFramebuffers.resize(_swapChainImageViews.size());

	for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
		std::array<VkImageView, 2> attachments = {
			_swapChainImageViews[i],
			_depthImageView
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = _swapChainExtent.width;
		framebufferInfo.height = _swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS) {
			Error::abort("Failed to create framebuffer.");
		}
	}
}

ID minty::RenderEngine::create_buffer(VkDeviceSize const size, VkBufferUsageFlags const usage, VkMemoryPropertyFlags const properties)
{
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VK_ASSERT(vkCreateBuffer(_device, &bufferInfo, nullptr, &buffer), "Failed to create buffer.");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(_device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = find_memory_type(memRequirements.memoryTypeBits, properties);

	VK_ASSERT(vkAllocateMemory(_device, &allocInfo, nullptr, &bufferMemory), "Failed to allocate buffer memory.");

	VK_ASSERT(vkBindBufferMemory(_device, buffer, bufferMemory, 0), "Failed to bind buffer memory.");

	return _buffers.emplace(Buffer(buffer, bufferMemory, size));
}

ID minty::RenderEngine::create_buffer_uniform(VkDeviceSize const size)
{
	return create_buffer(size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void minty::RenderEngine::destroy_buffer(ID const id)
{
	if (!_buffers.contains(id))
	{
		return;
	}

	// destroy the buffer with the id
	destroy_buffer(_buffers.at(id));

	// remove it from the list
	_buffers.erase(id);
}

void* minty::RenderEngine::map_buffer(ID const id) const
{
	Buffer const& buffer = _buffers.at(id);

	void* data = nullptr;

	VK_ASSERT(vkMapMemory(_device, buffer.memory, 0, buffer.size, 0, &data), "Failed to map memory for new buffer.");

	return data;
}

void minty::RenderEngine::unmap_buffer(ID const id) const
{
	Buffer const& buffer = _buffers.at(id);

	vkUnmapMemory(_device, buffer.memory);
}

void minty::RenderEngine::set_buffer(ID const id, void const* const data)
{
	Buffer const& buffer = _buffers.at(id);

	// set whole buffer
	set_buffer(id, data, 0, buffer.size);
}

void minty::RenderEngine::set_buffer(ID const id, void const* const data, VkDeviceSize const size, VkDeviceSize const offset)
{
	Buffer const& buffer = _buffers.at(id);

	// map data
	void* mappedData = map_buffer(id);

	// offset if needed
	if (offset)
	{
		Byte* temp = static_cast<Byte*>(mappedData);

		temp += offset;

		mappedData = static_cast<void*>(temp);
	}

	// set data
	memcpy(mappedData, data, size);

	// unmap data
	unmap_buffer(id);
}

VkBuffer minty::RenderEngine::get_buffer(ID const id) const
{
	return _buffers.at(id).buffer;
}

void minty::RenderEngine::get_buffer_data(ID const id, void* const out) const
{
	// get buffer
	Buffer const& buffer = _buffers.at(id);

	// map data
	void* data = map_buffer(id);

	// copy out data
	memcpy(out, data, buffer.size);

	// unmap
	unmap_buffer(id);
}

VkDeviceSize minty::RenderEngine::get_buffer_size(ID const id) const
{
	return _buffers.at(id).size;
}

void minty::RenderEngine::copy_buffer(ID const srcId, ID const dstId, VkDeviceSize const size)
{
	Buffer const& srcBuffer = _buffers.at(srcId);
	Buffer const& dstBuffer = _buffers.at(dstId);

	VkCommandBuffer commandBuffer = begin_single_time_commands(_commandPool);

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer.buffer, dstBuffer.buffer, 1, &copyRegion);

	end_single_time_commands(commandBuffer, _commandPool);
}

void minty::RenderEngine::destroy_buffer(Buffer const& buffer)
{
	vkDestroyBuffer(_device, buffer.buffer, nullptr);
	vkFreeMemory(_device, buffer.memory, nullptr);
}

uint32_t RenderEngine::find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	Error::abort("Failed to find_animation suitable memory type.");
	return 0;
}

void RenderEngine::create_command_pool(VkCommandPool& commandPool)
{
	// create info for command pool
	VkCommandPoolCreateInfo poolInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = _queueFamilyIndices.graphicsFamily.value(),
	};

	if (vkCreateCommandPool(_device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		Error::abort("Failed to create command pool.");
	}
}

void RenderEngine::create_command_buffers()
{
	// create space for all the buffers
	_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	// allocate a command buffer
	VkCommandBufferAllocateInfo allocInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = _commandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = (uint32_t)_commandBuffers.size(),
	};

	if (vkAllocateCommandBuffers(_device, &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
		Error::abort("Failed to allocate command buffers.");
	}
}

void minty::RenderEngine::set_scene(Scene const* const scene, Entity const camera)
{
	// update scene that is being rendered
	_scene = scene;

	// if scene is null, so are other things
	if (!_scene)
	{
		_registry = nullptr;
		return;
	}

	// set registry to scene registry
	_registry = &_scene->get_entity_registry();
	_renderSystem = _scene->get_system_registry().find<RenderSystem>();
}

void RenderEngine::record_command_buffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = 0, // Optional
		.pInheritanceInfo = nullptr, // Optional
	};

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		Error::abort("Failed to begin recording command buffer.");
	}

	// begin the render pass
	VkRenderPassBeginInfo renderPassInfo
	{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = _renderPass,
		.framebuffer = _swapChainFramebuffers[imageIndex],
	};

	// match size of attachments for best performance
	// define render area
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = _swapChainExtent;

	// clear the background
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = {
		{
			_backgroundColor.rf(),
			_backgroundColor.gf(),
			_backgroundColor.bf(),
			_backgroundColor.af()
		}
	};
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	// begin pass
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// set viewport
	_view.bind(commandBuffer);

	// render meshes
	draw_scene(commandBuffer);

	// done rendering
	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		Error::abort("Failed to record command buffer.");
	}
}

void RenderEngine::create_sync_objects()
{
	// resize to number of frames in flight
	_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	// semaphore = lock
	// fence = lock, but it waits instead of blocks?

	// create semaphore
	VkSemaphoreCreateInfo semaphoreInfo
	{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};

	// create fence
	VkFenceCreateInfo fenceInfo
	{
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT,
	};

	// create semaphores and fences
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(_device, &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS) {

			Error::abort("Failed to create synchronization objects for a frame.");
		}
	}
}

void RenderEngine::destroy()
{
	if (!_initialized)
	{
		return;
	}

	sync();

	// clean up vulkan
	cleanup_swap_chain();

	for (auto& buffer : _buffers)
	{
		destroy_buffer(buffer.second);
	}
	_buffers.clear();

	// destroy renderer data
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(_device, _renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(_device, _imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(_device, _inFlightFences[i], nullptr);
	}
	vkDestroyCommandPool(_device, _commandPool, nullptr);
	vkDestroyRenderPass(_device, _renderPass, nullptr);
	vkDestroyDevice(_device, nullptr);
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, _debugMessenger, nullptr);
	}
	vkDestroySurfaceKHR(instance, _surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	_initialized = false;
}

void minty::RenderEngine::bind(VkCommandBuffer const commandBuffer)
{
	// bind using the bound ids
	if (_boundIds.at(0) == ERROR_ID)
	{
		return;
	}

	// get references to grab descriptor sets from
	Shader const& shader = _renderSystem->get_shader(_boundIds.at(BIND_SHADER));
	ShaderPass const& shaderPass = _renderSystem->get_shader_pass(_boundIds.at(BIND_SHADER_PASS));
	Material const& material = _renderSystem->get_material(_boundIds.at(BIND_MATERIAL));

	// set descriptor sets we actually care about
	std::array<VkDescriptorSet, DESCRIPTOR_SET_COUNT> descriptorSets;
	descriptorSets[DESCRIPTOR_SET_SHADER] = shader.get_global_descriptor_set().at(_frame);
	descriptorSets[DESCRIPTOR_SET_SHADER_PASS] = shaderPass.get_descriptor_set().at(_frame);
	descriptorSets[DESCRIPTOR_SET_MATERIAL] = material.get_descriptor_set(0).at(_frame);

	// bind pipeline and descriptor sets
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPass.get_pipeline());
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader.get_pipeline_layout(), 0, DESCRIPTOR_SET_COUNT, descriptorSets.data(), 0, nullptr);
}

void minty::RenderEngine::bind(VkCommandBuffer const commandBuffer, ID const materialId, uint32_t const pass)
{
	// if ERROR_ID material, unbind all
	if (materialId == ERROR_ID)
	{
		for (size_t i = 0; i < _boundIds.size(); i++)
		{
			_boundIds[i] = ERROR_ID;
		}
		return;
	}

	// if the same, do nothing
	if (_boundIds.at(BIND_MATERIAL) == materialId)
	{
		return;
	}

	// new material, so update bound ids and bind descriptor sets as needed

	_boundIds[BIND_MATERIAL] = materialId;

	Material const& material = _renderSystem->get_material(materialId);
	ID const materialTemplateId = material.get_template_id();

	if (_boundIds.at(BIND_MATERIAL_TEMPLATE) == materialTemplateId)
	{
		// only update material
		bind(commandBuffer);
		return;
	}

	_boundIds[BIND_MATERIAL_TEMPLATE] = materialTemplateId;
	MaterialTemplate const& materialTemplate = _renderSystem->get_material_template(materialTemplateId);
	ID const shaderPassId = materialTemplate.get_shader_pass_ids().front();

	if (_boundIds.at(BIND_SHADER_PASS) == shaderPassId)
	{
		// only update material and material template
		bind(commandBuffer);
		return;
	}

	_boundIds[BIND_SHADER_PASS] = shaderPassId;
	ShaderPass const& shaderPass = _renderSystem->get_shader_pass(shaderPassId);
	ID const shaderId = shaderPass.get_shader_id();

	if (_boundIds.at(BIND_SHADER) == shaderId)
	{
		// only update material, material template, and shader pass
		bind(commandBuffer);
		return;
	}

	// update everything
	_boundIds[BIND_SHADER] = shaderId;
	bind(commandBuffer);
}

void minty::RenderEngine::sync()
{
	// wait for logical device to finish rendering before closing program
	vkDeviceWaitIdle(_device);
}

String minty::to_string(RenderEngine const& value)
{
	return std::format("RenderEngine()");
}
