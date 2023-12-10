#include "pch.h"
#include "M_Renderer.h"

#include "M_Rendering_RendererBuilder.h"
#include "M_Rendering_DrawCallObjectInfo.h"

#include "M_Console.h"
#include "M_Engine.h"
#include "M_File.h"
#include "M_Scene.h"
#include "M_EntityRegistry.h"
#include "M_Encoding.h"

#include "M_TransformComponent.h"
#include "M_CameraComponent.h"
#include "M_MeshComponent.h"
#include "M_RenderableTag.h"
#include "M_RelationshipComponent.h"

#include "M_Assets.h"
#include "M_Parse.h"

#include "M_Vector.h"
#include "M_Matrix.h"
#include "M_Basic_Vertex.h"

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
using namespace minty::rendering;
using namespace minty::vk;
using namespace minty::basic;

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

Renderer::Renderer(Window* const window)
	: _builder(nullptr)
	, _window(window)
	, _textures()
	, _materials()
	, _materialTemplates()
	, _shaderPasses()
	, _shaders()
	, _buffers()
	, _meshes()
	, _boundIds()
	, _view()
	, _backgroundColor({ 250, 220, 192, 255 }) // light tan color
	, _frame(1)
{
	for (size_t i = 0; i < _boundIds.size(); i++)
	{
		_boundIds[i] = ERROR_ID;
	}
}

Renderer::~Renderer()
{}

void minty::Renderer::init(rendering::RendererBuilder const& builder)
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
	queueFamilyIndices = find_queue_families(_physicalDevice);
	create_command_pool(commandPool);
	create_depth_resources();
	create_framebuffers();

	create_assets();

	create_command_buffers();
	create_sync_objects();

	_initialized = true;

	// builder no longer needed
	_builder = nullptr;
}

void Renderer::render_frame()
{
	// wait until previous draw has completed
	vkWaitForFences(_device, 1, &inFlightFences[_frame], VK_TRUE, UINT64_MAX);

	// recreate swap chain check
	uint32_t imageIndex;
	VkResult r = vkAcquireNextImageKHR(_device, swapChain, UINT64_MAX, imageAvailableSemaphores[_frame], VK_NULL_HANDLE, &imageIndex);

	if (r == VK_ERROR_OUT_OF_DATE_KHR) {
		recreate_swap_chain();
		return;
	}
	else if (r != VK_SUCCESS && r != VK_SUBOPTIMAL_KHR) {
		error::abort("Failed to acquire swap chain image.");
	}

	// reset the fence states so they can be used to wait again
	vkResetFences(_device, 1, &inFlightFences[_frame]);

	// reset command buffer
	vkResetCommandBuffer(commandBuffers[_frame], 0);

	// record the command buffer so we know what to do to render stuff to the screen
	record_command_buffer(commandBuffers[_frame], imageIndex);

	// update uniform information
	//update_uniform_buffer();

	// submit the command buffer
	VkSubmitInfo submitInfo
	{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO
	};

	// specify how to wait, in this case, with the semaphores
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[_frame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	// set number of command buffers
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[_frame];

	// specift what semaphores to signal when complete
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[_frame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	// submit the buffer
	VK_ASSERT(vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[_frame]), "Failed to submit draw command buffer.");

	// submit to swap chain so it will show up on the screen
	VkPresentInfoKHR presentInfo
	{
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = signalSemaphores
	};

	// specify which swap chain to use (only 1 normally)
	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	// present the visual to the screen
	r = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (r == VK_ERROR_OUT_OF_DATE_KHR || r == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		recreate_swap_chain();
	}
	else if (r != VK_SUCCESS) {
		error::abort("Failed to present swap chain image.");
	}

	// move to next frame
	_frame++;
	if (_frame == MAX_FRAMES_IN_FLIGHT) _frame = 0;
}

bool Renderer::is_running() const
{
	return _window->is_open();
}

bool minty::Renderer::is_initialized() const
{
	return _initialized;
}

void Renderer::create_instance()
{
	// check if we can use validation layers
	if (enableValidationLayers && !check_validation_layer_support())
	{
		error::abort("Validation layers requested, but not available.");
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
		error::abort("failed to create instance!");
	}
}

VkFormat Renderer::find_depth_format() {
	return find_supported_format(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

bool Renderer::has_stencil_component(VkFormat format) {
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkFormat Renderer::find_supported_format(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
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

	error::abort("Failed to find supported format.");

	return VkFormat();
}

void Renderer::create_depth_resources()
{
	VkFormat depthFormat = find_depth_format();

	create_image(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depthImageView = create_image_view(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void Renderer::create_image(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
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
		error::abort("Failed to create image.");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(_device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = find_memory_type(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		error::abort("Failed to allocate image memory.");
	}

	vkBindImageMemory(_device, image, imageMemory, 0);
}

void minty::Renderer::update()
{
	// do nothing if no scene set
	if (_scene == nullptr)
	{
		console::error("There is no Scene to render!");
		return;
	}

	// do nothing if no camera
	if (_mainCamera == NULL_ENTITY)
	{
		console::warn("There is no Camera to render to!");
		return;
	}

	// get camera transform
	CameraComponent const& camera = _registry->get<CameraComponent>(_mainCamera);
	TransformComponent const& transformComponent = _registry->get<TransformComponent>(_mainCamera);

	// update camera in renderer
	update_camera(camera, transformComponent);
}

VkDevice minty::Renderer::get_device() const
{
	return _device;
}

VkPhysicalDevice minty::Renderer::get_physical_device() const
{
	return _physicalDevice;
}

VkRenderPass minty::Renderer::get_render_pass() const
{
	return _renderPass;
}

uint32_t minty::Renderer::get_shader_count() const
{
	return static_cast<uint32_t>(_shaders.size());
}

uint32_t minty::Renderer::get_texture_count() const
{
	return static_cast<uint32_t>(_textures.size());
}

uint32_t minty::Renderer::get_sprite_count() const
{
	return static_cast<uint32_t>(_sprites.size());
}

uint32_t minty::Renderer::get_material_count() const
{
	return static_cast<uint32_t>(_materialTemplates.size());
}

uint32_t minty::Renderer::get_frame() const
{
	return _frame;
}

Texture& minty::Renderer::get_texture(ID const id)
{
	return _textures.at(id);
}

Texture const& minty::Renderer::get_texture(ID const id) const
{
	return _textures.at(id);
}

Sprite& minty::Renderer::get_sprite(ID const id)
{
	return _sprites.at(id);
}

Sprite const& minty::Renderer::get_sprite(ID const id) const
{
	return _sprites.at(id);
}

Shader& minty::Renderer::get_shader(ID const id)
{
	return _shaders.at(id);
}

Shader const& minty::Renderer::get_shader(ID const id) const
{
	return _shaders.at(id);
}

Shader& minty::Renderer::get_shader_from_material_id(ID const id)
{
	return get_shader(get_shader_pass(get_material_template(get_material(id).get_template_id()).get_shader_pass_ids().front()).get_shader_id());
}

Shader const& minty::Renderer::get_shader_from_material_id(ID const id) const
{
	return get_shader(get_shader_pass(get_material_template(get_material(id).get_template_id()).get_shader_pass_ids().front()).get_shader_id());
}

ShaderPass& minty::Renderer::get_shader_pass(ID const id)
{
	return _shaderPasses.at(id);
}

ShaderPass const& minty::Renderer::get_shader_pass(ID const id) const
{
	return _shaderPasses.at(id);
}

ShaderPass& minty::Renderer::get_shader_pass_from_material_id(ID const id)
{
	return get_shader_pass(get_material_template(get_material(id).get_template_id()).get_shader_pass_ids().front());
}

ShaderPass const& minty::Renderer::get_shader_pass_from_material_id(ID const id) const
{
	return get_shader_pass(get_material_template(get_material(id).get_template_id()).get_shader_pass_ids().front());
}

MaterialTemplate& minty::Renderer::get_material_template(ID const id)
{
	return _materialTemplates.at(id);
}

MaterialTemplate const& minty::Renderer::get_material_template(ID const id) const
{
	return _materialTemplates.at(id);
}

MaterialTemplate& minty::Renderer::get_material_template_from_material_id(ID const id)
{
	return get_material_template(get_material(id).get_template_id());
}

MaterialTemplate const& minty::Renderer::get_material_template_from_material_id(ID const id) const
{
	return get_material_template(get_material(id).get_template_id());
}

Material& minty::Renderer::get_material(ID const id)
{
	return _materials.at(id);
}

Material const& minty::Renderer::get_material(ID const id) const
{
	return _materials.at(id);
}

Mesh& minty::Renderer::get_mesh(ID const id)
{
	return _meshes.at(id);
}

Mesh const& minty::Renderer::get_mesh(ID const id) const
{
	return _meshes.at(id);
}

VkImageView Renderer::create_image_view(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
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
		error::abort("Failed to create texture image view.");
	}

	return imageView;
}

VkShaderModule minty::Renderer::load_shader_module(std::string const& path) const
{
	return create_shader_module(assets::load_chars(path));
}

VkShaderModule minty::Renderer::create_shader_module(std::vector<char> const& code) const
{
	VkShaderModuleCreateInfo createInfo
	{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size(),
		.pCode = reinterpret_cast<const uint32_t*>(code.data())
	};

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		error::abort("Failed to create shader module.");
	}

	return shaderModule;
}

VkCommandBuffer Renderer::begin_single_time_commands(VkCommandPool commandPool) {
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

void Renderer::end_single_time_commands(VkCommandBuffer commandBuffer, VkCommandPool commandPool) {
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(_device, commandPool, 1, &commandBuffer);
}

void Renderer::change_image_layout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
	VkCommandBuffer commandBuffer = begin_single_time_commands(commandPool);

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

	end_single_time_commands(commandBuffer, commandPool);
}

void Renderer::copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
	VkCommandBuffer commandBuffer = begin_single_time_commands(commandPool);

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

	end_single_time_commands(commandBuffer, commandPool);
}

void Renderer::create_image_views()
{
	// resize to fit all image views
	swapChainImageViews.resize(swapChainImages.size());

	for (uint32_t i = 0; i < swapChainImages.size(); i++) {
		swapChainImageViews[i] = create_image_view(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}

void Renderer::create_surface()
{
	// create window surface that vulkan can use to draw
	if (glfwCreateWindowSurface(instance, _window->get_raw(), nullptr, &surface) != VK_SUCCESS) {
		error::abort("Failed to create window surface.");
	}
}

void Renderer::pick_physical_device()
{
	// find number of hardware GPUs
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	// if zero, error
	if (deviceCount == 0)
	{
		error::abort("Failed to find GPU's with Vulkan support.");
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
		error::abort("Failed to find a suitable GPU.");
	}
}

bool Renderer::is_device_suitable(VkPhysicalDevice device)
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

void Renderer::create_swap_chain()
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
		.surface = surface,
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
	if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		error::abort("Failed to create swap chain.");
	}

	vkGetSwapchainImagesKHR(_device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(_device, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

	// update viewport and scissor
	_view.set_extent(extent.width, extent.height);
}

void Renderer::cleanup_swap_chain()
{
	vkDestroyImageView(_device, depthImageView, nullptr);
	vkDestroyImage(_device, depthImage, nullptr);
	vkFreeMemory(_device, depthImageMemory, nullptr);

	for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
		vkDestroyFramebuffer(_device, swapChainFramebuffers[i], nullptr);
	}

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		vkDestroyImageView(_device, swapChainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(_device, swapChain, nullptr);
}

void Renderer::recreate_swap_chain()
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

VkExtent2D Renderer::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities) {
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

VkPresentModeKHR Renderer::choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	// essentially, check if v-sync exists, then use it
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR; // v-sync
}

VkSurfaceFormatKHR Renderer::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	for (const auto& availableFormat : availableFormats) {
		// VK_FORMAT_R8G8B8A8_UINT?
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

SwapChainSupportDetails Renderer::query_swap_chain_support(VkPhysicalDevice device) {
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

bool Renderer::check_device_extension_support(VkPhysicalDevice device)
{
	// check if all required extensions are there
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

QueueFamilyIndices Renderer::find_queue_families(VkPhysicalDevice device)
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
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

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
void Renderer::create_logical_device()
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
		error::abort("Failed to create logical device.");
	}

	vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &presentQueue);
}

void minty::Renderer::draw_scene(VkCommandBuffer commandBuffer)
{
	// sort sprites so they render in the correct order, since Z does not matter
	_registry->sort<SpriteComponent>([](SpriteComponent const& left, SpriteComponent const& right)
		{
			return left.layer < right.layer;
		});

	TransformComponent const* transformComponent;

	// draw all meshes in the scene
	for (auto&& [entity, renderable, mesh] : _registry->view<RenderableTag const, MeshComponent>().each())
	{
		// get transform for entity
		transformComponent = _registry->try_get<TransformComponent>(entity);

		if (transformComponent)
		{
			// render the entity's mesh at the position
			draw_mesh(commandBuffer, transformComponent->global, mesh);
		}
		else
		{
			// if no transform, use empty matrix
			draw_mesh(commandBuffer, Matrix4(1.0f), mesh);
		}
	}

	// sort UITransforms so that it matches order of sprites for rendering
	_registry->sort<UITransformComponent, SpriteComponent>();

	// draw all UI in scene
	for (auto&& [entity, renderable, ui, sprite] : _registry->view<RenderableTag const, UITransformComponent const, SpriteComponent const>().each())
	{
		draw_ui(commandBuffer, ui, sprite);
	}

	// unbind any shaders used
	bind(commandBuffer, ERROR_ID);
	//bind_shader(commandBuffer, nullptr);
}

void minty::Renderer::draw_ui(VkCommandBuffer commandBuffer, UITransformComponent const& uiComponent, SpriteComponent const& spriteComponent)
{
	// get the sprite
	Sprite& sprite = get_sprite(spriteComponent.id);

	// bind the material the sprite is using
	bind(commandBuffer, sprite.get_material_id());

	// adjust info based on anchor and pivot
	float width = static_cast<float>(_window->get_width());
	float height = static_cast<float>(_window->get_height());
	float left, top, right, bottom;

	int anchor = static_cast<int>(uiComponent.anchorMode);

	// do x, then y
	if (anchor & static_cast<int>(AnchorMode::Left))
	{
		left = uiComponent.x / width;
		right = (uiComponent.x + uiComponent.width) / width;
	}
	else if (anchor & static_cast<int>(AnchorMode::Center))
	{
		left = uiComponent.x / width + 0.5f;
		right = (uiComponent.x + uiComponent.width) / width + 0.5f;
	}
	else if (anchor & static_cast<int>(AnchorMode::Right))
	{
		left = uiComponent.x / width + 1.0f;
		right = (uiComponent.x + uiComponent.width) / width + 1.0f;
	}
	else
	{
		left = uiComponent.left;
		right = uiComponent.right;
	}

	if (anchor & static_cast<int>(AnchorMode::Top))
	{
		top = uiComponent.y / height;
		bottom = (uiComponent.y + uiComponent.height) / height;
	}
	else if (anchor & static_cast<int>(AnchorMode::Middle))
	{
		top = uiComponent.y / height + 0.5f;
		bottom = (uiComponent.y + uiComponent.height) / height + 0.5f;
	}
	else if (anchor & static_cast<int>(AnchorMode::Bottom))
	{
		top = uiComponent.y / height + 1.0f;
		bottom = (uiComponent.y + uiComponent.height) / height + 1.0f;
	}
	else
	{
		top = uiComponent.top;
		bottom = uiComponent.bottom;
	}

	// set draw call info
	DrawCallObjectUI info
	{
		.materialId = sprite.get_material_id(),
		.layer = spriteComponent.layer,
		.coords = Vector4(sprite.get_min_coords(), sprite.get_max_coords()),
		.pos = Vector4(left, top, right, bottom),
	};
	Shader& shader = get_shader_from_material_id(sprite.get_material_id());
	shader.update_push_constant(commandBuffer, &info, sizeof(DrawCallObjectUI));

	// draw
	vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}

bool Renderer::check_validation_layer_support()
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

std::vector<const char*> Renderer::get_required_extensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void Renderer::populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = debug_callback
	};
}

ID minty::Renderer::create_texture(rendering::TextureBuilder const& builder)
{
	return _textures.emplace(builder.name, Texture(builder, *this));
}

ID minty::Renderer::create_shader(rendering::ShaderBuilder const& builder)
{
	return _shaders.emplace(builder.name, Shader(builder, *this));
}

ID minty::Renderer::create_shader_pass(rendering::ShaderPassBuilder const& builder)
{
	return _shaderPasses.emplace(builder.name, ShaderPass(builder, *this));
}

ID minty::Renderer::create_material_template(rendering::MaterialTemplateBuilder const& builder)
{
	return _materialTemplates.emplace(builder.name, MaterialTemplate(builder, *this));
}

ID minty::Renderer::create_material(rendering::MaterialBuilder const& builder)
{
	return _materials.emplace(builder.name, Material(builder, *this));
}

ID minty::Renderer::create_mesh()
{
	// just create a brand new mesh
	return _meshes.emplace(Mesh(*this));
}

ID minty::Renderer::get_or_create_mesh(std::string const& name)
{
	// if contains name, return it
	if (_meshes.contains(name))
	{
		return _meshes.get_id(name);
	}

	// create new
	return _meshes.emplace(name, Mesh(*this));
}

ID minty::Renderer::get_or_create_mesh(MeshType const type)
{
	std::string name = to_string(type);

	// if contains type, return that
	if (_meshes.contains(name))
	{
		return _meshes.get_id(name);
	}

	// create new
	ID id = _meshes.emplace(name, Mesh(*this));
	Mesh& mesh = _meshes.at(id);

	switch (type)
	{
	case MeshType::Custom:
		// do nothing for now
		console::todo("MeshType::Custom");
		break;
	case MeshType::Quad:
		Mesh::create_primitive_quad(mesh);
		break;
	case MeshType::Cube:
		Mesh::create_primitive_cube(mesh);
		break;
	case MeshType::Pyramid:
		Mesh::create_primitive_pyramid(mesh);
		break;
	case MeshType::Sphere:
		Mesh::create_primitive_sphere(mesh);
		break;
	case MeshType::Cylinder:
		Mesh::create_primitive_cylinder(mesh);
		break;
	default:
		// empty mesh
		mesh.clear();
		break;
	}

	return id;
}

ID minty::Renderer::find_texture(std::string const& name)
{
	if (!_textures.contains(name))
	{
		return ERROR_ID;
	}

	return _textures.get_id(name);
}

ID minty::Renderer::find_shader(std::string const& name)
{
	if (!_shaders.contains(name))
	{
		return ERROR_ID;
	}

	return _shaders.get_id(name);
}

ID minty::Renderer::find_shader_pass(std::string const& name)
{
	if (!_shaderPasses.contains(name))
	{
		return ERROR_ID;
	}

	return _shaderPasses.get_id(name);
}

ID minty::Renderer::find_material_template(std::string const& name)
{
	if (!_materialTemplates.contains(name))
	{
		return ERROR_ID;
	}

	return _materialTemplates.get_id(name);
}

ID minty::Renderer::find_material(std::string const& name)
{
	if (!_materials.contains(name))
	{
		return ERROR_ID;
	}

	return _materials.get_id(name);
}

ID minty::Renderer::find_mesh(std::string const& name)
{
	if (!_meshes.contains(name))
	{
		return ERROR_ID;
	}

	return _meshes.get_id(name);
}

ID minty::Renderer::load_texture(std::string const& path)
{
	if (check_asset(path, true))
	{
		return ERROR_ID;
	}

	Node meta = assets::load_meta(path);

	// create texture builder from path and meta file
	TextureBuilder builder(file::name(path), path);

	builder.filter = from_string_vk_filter(meta.get_string("filter"));
	builder.format = from_string_vk_format(meta.get_string("format"));
	builder.addressMode = from_string_vk_sampler_address_mode(meta.get_string("samplerAddressMode"));
	builder.mipmapMode = from_string_vk_sampler_mipmap_mode(meta.get_string("samplerMipmapMode"));
	builder.pixelFormat = from_string_texture_builder_pixel_format(meta.get_string("pixelFormat", "RGBA"));

	return create_texture(builder);
}

ID minty::Renderer::load_shader(std::string const& path)
{
	if (check_asset(path, false))
	{
		return ERROR_ID;
	}

	Node meta = assets::load_node(path);

	ShaderBuilder builder;
	builder.name = file::name(path);

	std::vector<Node> const* nodes;
	if (nodes = meta.find_all("push"))
	{
		for (Node const& child : *nodes)
		{
			PushConstantInfo pushConstantInfo;

			pushConstantInfo.name = child.get_string("name", child.to_string());
			pushConstantInfo.stageFlags = from_string_vk_shader_stage_flag_bits(child.get_string("stageFlags"));
			pushConstantInfo.offset = child.get_uint("offset");
			pushConstantInfo.size = child.get_uint("size");

			builder.pushConstantInfos.emplace(pushConstantInfo.name, pushConstantInfo);
		}
	}

	if (nodes = meta.find_all("uniform"))
	{
		for (Node const& child : *nodes)
		{
			UniformConstantInfo uniformConstantInfo;

			uniformConstantInfo.name = child.get_string("name", child.to_string());
			uniformConstantInfo.type = from_string_vk_descriptor_type(child.get_string("type"));
			uniformConstantInfo.set = child.get_uint("set");
			uniformConstantInfo.binding = child.get_uint("binding");
			uniformConstantInfo.count = child.get_uint("count", 1u);
			uniformConstantInfo.size = static_cast<VkDeviceSize>(child.get_size_t("size"));
			uniformConstantInfo.stageFlags = from_string_vk_shader_stage_flag_bits(child.get_string("stageFlags"));
			if (std::vector<Node> const* ids = child.find_all("id"))
			{
				for (auto const& id : *ids)
				{
					uniformConstantInfo.ids.push_back(id.to_id());
				}
			}

			builder.uniformConstantInfos.emplace(uniformConstantInfo.name, uniformConstantInfo);
		}
	}

	return create_shader(builder);
}

ID minty::Renderer::load_shader_pass(std::string const& path)
{
	if (check_asset(path, false))
	{
		return ERROR_ID;
	}

	Node meta = assets::load_node(path);

	ShaderPassBuilder builder;
	builder.name = file::name(path);
	builder.shaderId = find_shader(meta.get_string("shader", meta.to_string()));
	builder.topology = from_string_vk_primitive_topology(meta.get_string("primitiveTopology"));
	builder.polygonMode = from_string_vk_polygon_mode(meta.get_string("polygonMode"));
	builder.cullMode = from_string_vk_cull_mode_flag_bits(meta.get_string("cullMode"));
	builder.frontFace = from_string_vk_front_face(meta.get_string("frontFace"));
	builder.lineWidth = meta.get_float("lineWidth", 1.0f);

	std::vector<Node> const* nodes;
	if (nodes = meta.find_all("binding"))
	{
		for (auto const& child : *nodes)
		{
			VkVertexInputBindingDescription binding = {};

			binding.binding = child.get_uint("binding", child.to_uint());
			binding.stride = child.get_uint("stride");
			binding.inputRate = from_string_vk_vertex_input_rate(child.get_string("inputRate"));

			builder.vertexBindings.push_back(binding);
		}
	}
	if (nodes = meta.find_all("attribute"))
	{
		for (auto const& child : *nodes)
		{
			VkVertexInputAttributeDescription attribute = {};

			attribute.location = child.get_uint("location", child.to_uint());
			attribute.binding = child.get_uint("binding");
			attribute.format = from_string_vk_format(child.get_string("format"));
			attribute.offset = child.get_uint("offset");

			builder.vertexAttributes.push_back(attribute);
		}
	}
	if (nodes = meta.find_all("stage"))
	{
		for (auto const& child : *nodes)
		{
			ShaderPassBuilder::ShaderStageInfo info;

			info.stage = from_string_vk_shader_stage_flag_bits(child.get_string("stage", child.to_string()));
			std::string path = child.get_string("path");
			info.code = assets::load_chars(path);
			info.entry = child.get_string("entry", "main");

			builder.stages.push_back(info);
		}
	}

	return create_shader_pass(builder);
}

ID minty::Renderer::load_material_template(std::string const& path)
{
	if (check_asset(path, false))
	{
		return ERROR_ID;
	}

	Node meta = assets::load_node(path);

	MaterialTemplateBuilder builder;
	builder.name = file::name(path);

	std::vector<Node> const* nodes;
	if (nodes = meta.find_all("pass"))
	{
		for (auto const& child : *nodes)
		{
			builder.shaderPassIds.push_back(find_shader_pass(child.to_string()));
		}
	}
	if (Node const* node = meta.find("defaults"))
	{
		console::todo("Renderer::load_material_template defaults");
	}

	return create_material_template(builder);
}

ID minty::Renderer::load_material(std::string const& path)
{
	if (check_asset(path, false))
	{
		return ERROR_ID;
	}

	Node meta = assets::load_node(path);

	MaterialBuilder builder;
	builder.name = file::name(path);

	builder.templateId = find_material_template(meta.get_string("template"));

	if (Node const* node = meta.find("values"))
	{
		for (auto const& child : node->children)
		{
			Dynamic d;
			Reader reader(child.second.front());
			d.deserialize(reader);
		}
	}

	return create_material(builder);
}

ID minty::Renderer::load_mesh(std::string const& path)
{
	if (check_asset(path, false))
	{
		return ERROR_ID;
	}

	std::string extension = file::extension(path);

	if (extension != ".obj")
	{
		console::error(std::format("Cannot load mesh from file type \"{}\".", extension));
		return ERROR_ID;
	}

	ID id = create_mesh();

	if (extension == ".obj")
	{
		load_mesh_obj(path, id);
	}

	return id;
}

int minty::Renderer::check_asset(std::string const& path, bool const requiresMeta) const
{
	// can load if assets exists, and if no meta is required, or if a meta is required, it exists
	if (!assets::exists(path))
	{
		console::error(std::format("Cannot find asset at path \"{}\".", path));
		// cannot find asset itself
		return 1;
	}

	if (requiresMeta && !assets::exists_meta(path))
	{
		console::error(std::format("Cannot find meta file for asset at path \"{}\".", path));
		// cannot find asset meta file
		return 2;
	}

	// found both
	return 0;
}

void minty::Renderer::load_mesh_obj(std::string const& path, ID const id)
{
	Node meta = assets::load_meta(path);

	Mesh& mesh = get_mesh(id);

	std::vector<std::string> lines = assets::load_lines(path);

	std::vector<Vector3> positions;
	std::vector<Vector2> coords;
	std::vector<Vector3> normals;

	std::unordered_map<Vector3Int, uint16_t> faces;
	std::vector<Vertex3D> vertices;
	std::vector<uint16_t> indices;

	std::istringstream ss;
	std::string token;

	for (auto const& line : lines)
	{
		ss = std::istringstream(line);
		ss >> token;
		if (token == "v")
		{
			// position
			Vector3 position;
			ss >> position.x >> position.y >> position.z;
			positions.push_back(position);
		}
		else if(token == "vt")
		{
			// coord
			Vector2 coord;
			ss >> coord.x >> coord.y;
			coords.push_back(coord);
		}
		else if (token == "vn")
		{
			// normal
			Vector3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (token == "f")
		{
			// face
			// get pairs
			for (size_t i = 0; i < 3; i++)
			{
				std::string set;
				ss >> set;

				std::istringstream setss(set);
				Vector3Int faceIndices = Vector3Int();

				if (std::getline(setss, token, '/'))
				{
					faceIndices.x = parse::to_int(token);

					if (std::getline(setss, token, '/'))
					{
						faceIndices.y = parse::to_int(token);

						if (std::getline(setss, token, '/'))
						{
							faceIndices.z = parse::to_int(token);
						}
					}
				}

				// if combo exists, add that index
				auto const& found = faces.find(faceIndices);
				if (found == faces.end())
				{
					// vertex does not exist yet

					uint16_t index = static_cast<uint16_t>(vertices.size());
					vertices.push_back(Vertex3D
						{
							.pos = positions[faceIndices.x],
							.normal = normals[faceIndices.z],
							.coord = coords[faceIndices.y]
						});
					indices.push_back(index);
					faces.emplace(faceIndices, index);
				}
				else
				{
					// vertex already exists

					uint16_t index = found->second;
					indices.push_back(index);
				}
			}
		}
	}

	// all vertices and indices populated
	mesh.set_vertices(vertices);
	mesh.set_indices(indices);
}

void Renderer::setup_debug_messenger() {
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populate_debug_messenger_create_info(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		error::abort("Failed to set up debug messenger.");
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL Renderer::debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	// change colors based on severity
	if (messageSeverity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		console::error(pCallbackData->pMessage);
	}
	else if (messageSeverity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		console::warn(pCallbackData->pMessage);
	}
	else if (messageSeverity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
	{
		console::info(pCallbackData->pMessage);
	}
	else
	{
		console::info(pCallbackData->pMessage);
		//console::log(pCallbackData->pMessage);
	}

	return VK_FALSE;
}

void Renderer::create_render_pass()
{
	// need to tell vulkan how much of what to use for rendering

	VkAttachmentDescription colorAttachment
	{
		.format = swapChainImageFormat,
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
		error::abort("Failed to create render pass.");
	}
}

void Renderer::create_framebuffers()
{
	// resize to hold all framebuffers
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		std::array<VkImageView, 2> attachments = {
			swapChainImageViews[i],
			depthImageView
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			error::abort("Failed to create framebuffer.");
		}
	}
}

void minty::Renderer::build_textures()
{
	// go through each texture, create it with builder data

}

void minty::Renderer::build_shaders()
{
	// build custom

}

void minty::Renderer::build_materials()
{
	// not really a "pair"

}

void minty::Renderer::create_assets()
{
	// textures
	for (auto const& builder : _builder->textureBuilders)
	{
		create_texture(*builder.second);
	}

	// shaders
	for (auto const builder : _builder->shaderBuilders)
	{
		create_shader(*builder.second);
	}

	// shader passes
	for (auto const builder : _builder->shaderPassBuilders)
	{
		create_shader_pass(*builder.second);
	}

	// material templates
	for (auto const builder : _builder->materialTemplateBuilders)
	{
		create_material_template(*builder.second);
	}

	// materials
	for (auto const& builder : _builder->materialBuilders)
	{
		create_material(*builder.second);
	}
}

void Renderer::update_camera(CameraComponent const& camera, TransformComponent const& transform)
{
	Vector4 matPos = transform.global[3];
	Vector3 globalPos = Vector3(matPos.x, matPos.y, matPos.z);

	Matrix4 view = glm::lookAt(globalPos, globalPos + transform.local.rotation.forward(), Vector3(0.0f, 1.0f, 0.0f));

	// TODO: don't use lookat
	// maybe invert global?

	// get projection
	Matrix4 proj;
	switch (camera.perspective)
	{
	case CameraComponent::Perspective::Perspective:
		proj = glm::perspective(glm::radians(camera.fov), swapChainExtent.width / static_cast<float>(swapChainExtent.height), camera.nearPlane, camera.farPlane);
		break;
	default:
		proj = Matrix4(1.0f);
		break;
	}
	// flip y and x so that we have a left handed coordinate system
	//proj[0][0] *= -1.0f;	// x
	//proj[1][1] *= -1.0f;	// y
	//proj[2][2] *= -1.0f;	// z

	// multiply together
	Matrix4 transformMatrix = proj * view;

	// update buffer object
	CameraBufferObject obj =
	{
		.transform = transformMatrix,
	};

	// update all shaders
	for (auto& shader : _shaders)
	{
		shader.second.update_global_uniform_constant("camera", &obj, sizeof(CameraBufferObject));
		//shader.update_uniform_constant_frame("camera", &obj, sizeof(CameraBufferObject));
	}
}

ID minty::Renderer::create_buffer(VkDeviceSize const size, VkBufferUsageFlags const usage, VkMemoryPropertyFlags const properties)
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

	return _buffers.emplace(Buffer
		{
			.buffer = buffer,
			.memory = bufferMemory,
			.size = size
		});
}

ID minty::Renderer::create_buffer_uniform(VkDeviceSize const size)
{
	return create_buffer(size, VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void minty::Renderer::destroy_buffer(ID const id)
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

void* minty::Renderer::map_buffer(ID const id) const
{
	Buffer const& buffer = _buffers.at(id);

	void* data = nullptr;

	VK_ASSERT(vkMapMemory(_device, buffer.memory, 0, buffer.size, 0, &data), "Failed to map memory for new buffer.");

	return data;
}

void minty::Renderer::unmap_buffer(ID const id) const
{
	Buffer const& buffer = _buffers.at(id);

	vkUnmapMemory(_device, buffer.memory);
}

void minty::Renderer::set_buffer(ID const id, void* const data)
{
	Buffer const& buffer = _buffers.at(id);

	// set whole buffer
	set_buffer(id, data, 0, buffer.size);
}

void minty::Renderer::set_buffer(ID const id, void* const data, VkDeviceSize const size, VkDeviceSize const offset)
{
	Buffer const& buffer = _buffers.at(id);

	// map data
	void* mappedData = map_buffer(id);

	// offset if needed
	if (offset)
	{
		byte* temp = static_cast<byte*>(mappedData);

		temp += offset;

		mappedData = static_cast<void*>(temp);
	}

	// set data
	memcpy(mappedData, data, size);

	// unmap data
	unmap_buffer(id);
}

VkBuffer minty::Renderer::get_buffer(ID const id) const
{
	return _buffers.at(id).buffer;
}

void minty::Renderer::get_buffer_data(ID const id, void* const out) const
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

VkDeviceSize minty::Renderer::get_buffer_size(ID const id) const
{
	return _buffers.at(id).size;
}

void minty::Renderer::copy_buffer(ID const srcId, ID const dstId, VkDeviceSize const size)
{
	Buffer const& srcBuffer = _buffers.at(srcId);
	Buffer const& dstBuffer = _buffers.at(dstId);

	VkCommandBuffer commandBuffer = begin_single_time_commands(commandPool);

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer.buffer, dstBuffer.buffer, 1, &copyRegion);

	end_single_time_commands(commandBuffer, commandPool);
}

void minty::Renderer::destroy_buffer(rendering::Buffer const& buffer)
{
	vkDestroyBuffer(_device, buffer.buffer, nullptr);
	vkFreeMemory(_device, buffer.memory, nullptr);
}

uint32_t Renderer::find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	error::abort("Failed to find suitable memory type.");
	return 0;
}

void Renderer::create_command_pool(VkCommandPool& commandPool)
{
	// create info for command pool
	VkCommandPoolCreateInfo poolInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(),
	};

	if (vkCreateCommandPool(_device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		error::abort("Failed to create command pool.");
	}
}

void Renderer::create_command_buffers()
{
	// create space for all the buffers
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	// allocate a command buffer
	VkCommandBufferAllocateInfo allocInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = commandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = (uint32_t)commandBuffers.size(),
	};

	if (vkAllocateCommandBuffers(_device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		error::abort("Failed to allocate command buffers.");
	}
}

void minty::Renderer::draw_mesh(VkCommandBuffer commandBuffer, Matrix4 const& transformationMatrix, MeshComponent const& meshComponent)
{
	// do nothing if null mesh
	if (meshComponent.meshId == ERROR_ID)
	{
		return;
	}

	Mesh& mesh = get_mesh(meshComponent.meshId);

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
	Shader& shader = get_shader_from_material_id(meshComponent.materialId);
	shader.update_push_constant(commandBuffer, &info, sizeof(DrawCallObject3D));

	// draw
	vkCmdDrawIndexed(commandBuffer, mesh.get_index_count(), 1, 0, 0, 0);
}

void minty::Renderer::set_main_camera(Entity const entity)
{
	_mainCamera = entity;
}

void minty::Renderer::set_scene(Scene const* const scene, Entity const camera)
{
	// update scene that is being rendered
	_scene = scene;

	// if scene is null, so are other things
	if (_scene == nullptr)
	{
		_registry = nullptr;
		_mainCamera = NULL_ENTITY;
		return;
	}

	// set registry to scene registry
	_registry = _scene->get_entity_registry();

	// update camera we are rendering from
	if (camera == NULL_ENTITY)
	{
		// no camera provided, attempt to find first one in scene
		_mainCamera = _scene->get_entity_registry()->find_by_type<CameraComponent>();
	}
	else
	{
		// camera provided, use that
		_mainCamera = camera;
	}
}

void Renderer::record_command_buffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = 0, // Optional
		.pInheritanceInfo = nullptr, // Optional
	};

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		error::abort("Failed to begin recording command buffer.");
	}

	// begin the render pass
	VkRenderPassBeginInfo renderPassInfo
	{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = _renderPass,
		.framebuffer = swapChainFramebuffers[imageIndex],
	};

	// match size of attachments for best performance
	// define render area
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

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
		error::abort("Failed to record command buffer.");
	}
}

void Renderer::create_sync_objects()
{
	// resize to number of frames in flight
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

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
		if (vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(_device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

			error::abort("Failed to create synchronization objects for a frame.");
		}
	}
}

void Renderer::destroy()
{
	sync();

	// clean up vulkan
	cleanup_swap_chain();

	// destroy assets
	for (auto& tex : _textures)
	{
		tex.second.destroy();
	}
	_textures.clear();
	for (auto& material : _materials)
	{
		material.second.destroy();
	}
	_materials.clear();
	for (auto& materialTemplate : _materialTemplates)
	{
		materialTemplate.second.destroy();
	}
	_materialTemplates.clear();
	for (auto& shaderPass : _shaderPasses)
	{
		shaderPass.second.destroy();
	}
	_shaderPasses.clear();
	for (auto& shader : _shaders)
	{
		shader.second.destroy();
	}
	_shaders.clear();
	for (auto& buffer : _buffers)
	{
		destroy_buffer(buffer.second);
	}
	_buffers.clear();

	// destroy renderer data
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(_device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(_device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(_device, inFlightFences[i], nullptr);
	}
	vkDestroyCommandPool(_device, commandPool, nullptr);
	vkDestroyRenderPass(_device, _renderPass, nullptr);
	vkDestroyDevice(_device, nullptr);
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
}

void minty::Renderer::bind(VkCommandBuffer const commandBuffer)
{
	// bind using the bound ids
	if (_boundIds.at(0) == ERROR_ID)
	{
		return;
	}

	// get references to grab descriptor sets from
	Shader const& shader = get_shader(_boundIds.at(BIND_SHADER));
	ShaderPass const& shaderPass = get_shader_pass(_boundIds.at(BIND_SHADER_PASS));
	Material const& material = get_material(_boundIds.at(BIND_MATERIAL));

	// set descriptor sets we actually care about
	std::array<VkDescriptorSet, DESCRIPTOR_SET_COUNT> descriptorSets;
	descriptorSets[DESCRIPTOR_SET_SHADER] = shader.get_global_descriptor_set().at(_frame);
	descriptorSets[DESCRIPTOR_SET_SHADER_PASS] = shaderPass.get_descriptor_set().at(_frame);
	descriptorSets[DESCRIPTOR_SET_MATERIAL] = material.get_descriptor_set(0).at(_frame);

	// bind pipeline and descriptor sets
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPass.get_pipeline());
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader.get_pipeline_layout(), 0, DESCRIPTOR_SET_COUNT, descriptorSets.data(), 0, nullptr);
}

void minty::Renderer::bind(VkCommandBuffer const commandBuffer, ID const materialId, uint32_t const pass)
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

	Material const& material = get_material(materialId);
	ID const materialTemplateId = material.get_template_id();

	if (_boundIds.at(BIND_MATERIAL_TEMPLATE) == materialTemplateId)
	{
		// only update material
		bind(commandBuffer);
		return;
	}

	_boundIds[BIND_MATERIAL_TEMPLATE] = materialTemplateId;
	MaterialTemplate const& materialTemplate = get_material_template(materialTemplateId);
	ID const shaderPassId = materialTemplate.get_shader_pass_ids().front();

	if (_boundIds.at(BIND_SHADER_PASS) == shaderPassId)
	{
		// only update material and material template
		bind(commandBuffer);
		return;
	}

	_boundIds[BIND_SHADER_PASS] = shaderPassId;
	ShaderPass const& shaderPass = get_shader_pass(shaderPassId);
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

void minty::Renderer::sync()
{
	// wait for logical device to finish rendering before closing program
	vkDeviceWaitIdle(_device);
}

std::string minty::to_string(Renderer const& value)
{
	return std::format("Renderer()");
}
