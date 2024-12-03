#include "pch.h"
#include "VulkanRenderer.h"

#include "Minty/Asset/AssetManager.h"
#include "Minty/Core/Application.h"
#include "Minty/Core/Color.h"
#include "Minty/Core/Constants.h"
#include "Minty/Debug/Debug.h"
#include "Platform/Vulkan/VulkanViewport.h"
#include "Platform/Vulkan/VulkanRenderPass.h"
#include <array>
#include <map>
#include <set>

#define VK_ASSERT_RESULT(functionCall, message) { VkResult result = (functionCall); if (result != VK_SUCCESS) { throw std::runtime_error((message)); }}
#define VK_ASSERT_RESULT_RETURN_OBJECT(objectType, functionCall, message) { objectType object; VK_ASSERT_RESULT((functionCall), (message)); return object; }
#define VK_ASSERT_FAIL(message) throw std::runtime_error(message);

using namespace Minty;

VkInstance VulkanRenderer::s_instance = VK_NULL_HANDLE;
VkPhysicalDevice VulkanRenderer::s_physicalDevice = VK_NULL_HANDLE;
VulkanRenderer::QueueFamilyIndices VulkanRenderer::s_queueFamilyIndices = {};
VkDevice VulkanRenderer::s_device = VK_NULL_HANDLE;
VkQueue VulkanRenderer::s_graphicsQueue = {};
VkQueue VulkanRenderer::s_presentQueue = {};
VkSurfaceKHR VulkanRenderer::s_surface = VK_NULL_HANDLE;
Format VulkanRenderer::s_targetSwapchainFormat = Format::Undefined;
VkSurfaceFormatKHR VulkanRenderer::s_swapchainSurfaceFormat = {};
VkExtent2D VulkanRenderer::s_swapchainExtent = {};
VkSwapchainKHR VulkanRenderer::s_swapchain = VK_NULL_HANDLE;
uint32_t VulkanRenderer::s_imageIndex = 0;
std::vector<Owner<VulkanImage>> VulkanRenderer::s_swapchainImages = {};
Owner<VulkanImage> VulkanRenderer::s_depthImage = nullptr;
Ref<VulkanViewport> VulkanRenderer::s_viewport = nullptr;
Ref<VulkanScissor> VulkanRenderer::s_scissor = nullptr;
VkCommandPool VulkanRenderer::s_commandPool = VK_NULL_HANDLE;
std::array<VulkanRenderer::Frame, MAX_FRAMES_IN_FLIGHT> VulkanRenderer::s_frames = {};
Size VulkanRenderer::s_currentFrame = 0;

void Minty::VulkanRenderer::initialize(RendererBuilder const& builder)
{
	// get application instance for the data
	Application& application = Application::instance();

	// instance
	create_instance(application.get_info());

	// validation layers
	setup_debug_messenger();

	// surface
	s_surface = create_surface(Renderer::get_window());

	// physical device
	select_physical_device();

	// queue families
	s_queueFamilyIndices = find_queue_families(s_physicalDevice);

	// logical device
	create_device();

	// queues are created when the logical devices is created
	// get queues
	s_graphicsQueue = get_device_queue(s_queueFamilyIndices.graphicsFamily.value());
	s_presentQueue = get_device_queue(s_queueFamilyIndices.presentFamily.value());

	// swapchain, images, etc.
	s_targetSwapchainFormat = builder.targetSurfaceFormat;
	s_swapchain = VK_NULL_HANDLE;
	recreate_swapchain();

	// viewport
	ViewportBuilder viewportBuilder{};
	viewportBuilder.id = UUID::create();
	viewportBuilder.width = static_cast<Float>(s_swapchainExtent.width);
	viewportBuilder.height = static_cast<Float>(s_swapchainExtent.height);
	s_viewport = AssetManager::create<Viewport>(viewportBuilder);

	// scissor
	ScissorBuilder scissorBuilder{};
	scissorBuilder.id = UUID::create();
	scissorBuilder.width = s_swapchainExtent.width;
	scissorBuilder.height = s_swapchainExtent.height;
	s_scissor = AssetManager::create<Scissor>(scissorBuilder);

	// command pool
	create_command_pool(s_queueFamilyIndices.graphicsFamily.value());

	// depth testing
	create_depth_resources();

	// frames
	for (Frame& frame : s_frames)
	{
		initialize_frame(frame);
	}

	// set current frame back to 0
	s_currentFrame = 0;
}

void Minty::VulkanRenderer::shutdown()
{
	// wait until idle so it is safe to destroy stuff
	sync();

	// destroy frames
	for (Frame& frame : s_frames)
	{
		destroy_frame(frame);
	}

	// destroy depth stencil
	destroy_depth_resources();

	// destroy images
	s_swapchainImages.clear();

	// destroy swapchain
	destroy_swapchain();

	destroy_command_pool();

	destroy_device();

	cleanup_debug_messenger();

	destroy_surface(s_surface);
	s_surface = VK_NULL_HANDLE;

	destroy_instance();
}

void Minty::VulkanRenderer::initialize_frame(Frame& frame)
{
	frame.commandBuffer = create_command_buffer();

	// sync objects
	frame.imageAvailableSemaphore = create_semaphore();
	frame.renderFinishedSemaphore = create_semaphore();
	frame.inFlightFence = create_fence();
}

void Minty::VulkanRenderer::destroy_frame(Frame& frame)
{
	destroy_command_buffer(frame.commandBuffer);

	destroy_semaphore(frame.imageAvailableSemaphore);
	destroy_semaphore(frame.renderFinishedSemaphore);
	destroy_fence(frame.inFlightFence);
}

void Minty::VulkanRenderer::recreate_swapchain()
{
	// if minimized, wait until no longer minimized
	Ref<Window> window = WindowManager::get_main();
	window->refresh();
	while (window->get_frame_width() == 0 && window->get_frame_height() == 0)
	{
		window->wait_events();
		window->refresh();
	}

	// wait for idle
	sync();

	// destroy old swapchain, if able
	if (s_swapchain != VK_NULL_HANDLE)
	{
		// destroy images
		s_swapchainImages.clear();

		// destroy swapchain
		destroy_swapchain();
	}

	// create swapchain and images
	SwapChainSupportDetails swapchainSupport = query_swap_chain_support(s_physicalDevice);
	if (s_targetSwapchainFormat != Format::Undefined)
	{
		// aim for custom format
		s_swapchainSurfaceFormat = select_swap_surface_format(swapchainSupport.formats, format_to_vulkan(s_targetSwapchainFormat));
	}
	else
	{
		// use default format
		s_swapchainSurfaceFormat = select_swap_surface_format(swapchainSupport.formats);
	}
	VkPresentModeKHR presentMode = select_swap_present_mode(swapchainSupport.presentModes);
	s_swapchainExtent = select_swap_extent(swapchainSupport.capabilities);
	create_swapchain(s_swapchainSurfaceFormat, s_swapchainExtent, presentMode);

	// create images from swapchain
	std::vector<VkImage> swapchainImages = get_swapchain_images();
	s_swapchainImages.reserve(swapchainImages.size());
	std::vector<Ref<Image>> swapchainImageRefs;
	swapchainImageRefs.reserve(swapchainImages.size());
	ImageBuilder swapchainImageBuilder{};
	swapchainImageBuilder.width = s_swapchainExtent.width;
	swapchainImageBuilder.height = s_swapchainExtent.height;
	swapchainImageBuilder.format = static_cast<Format>(s_swapchainSurfaceFormat.format);
	swapchainImageBuilder.aspect = ImageAspect::Color;
	for (Size i = 0; i < swapchainImages.size(); i++)
	{
		Owner<VulkanImage> image = Owner<VulkanImage>(swapchainImageBuilder, swapchainImages.at(i));
		s_swapchainImages.push_back(image);
		swapchainImageRefs.push_back(image.create_ref());
	}

	// recreate depth resources
	if (s_depthImage == nullptr)
	{
		create_depth_resources();
	}
	else
	{
		recreate_depth_resources();
	}

	// re-init framebuffers using new images
	RenderTargetBuilder renderTargetBuilder{};
	renderTargetBuilder.images = swapchainImageRefs;
	for (auto const& renderTarget : Renderer::get_screen_render_targets())
	{
		if (renderTarget == nullptr)
		{
			continue;
		}

		// if not null, reinitialize with new data
		renderTargetBuilder.renderPass = renderTarget->get_render_pass();
		renderTarget->reinitialize(renderTargetBuilder);
	}

	// update window viewport and scissor
	if (s_viewport != nullptr)
	{
		s_viewport->set_size(static_cast<Float>(s_swapchainExtent.width), static_cast<Float>(s_swapchainExtent.height));
	}

	if (s_scissor != nullptr)
	{
		s_scissor->set_size(static_cast<Float>(s_swapchainExtent.width), static_cast<Float>(s_swapchainExtent.height));
	}
}

int Minty::VulkanRenderer::start_frame()
{
	Frame& frame = get_current_frame();

	// wait for last frame to finish
	wait_for_fence(frame.inFlightFence);
	reset_fence(frame.inFlightFence);

	// get the image in the swapchain to use
	VkResult swapchainResult = get_next_swapchain_image(frame.imageAvailableSemaphore, s_imageIndex);

	// check for swapchain rebuilding
	if (swapchainResult == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreate_swapchain();
		return 1;
	}

	// get working command buffer for the current frame
	VkCommandBuffer commandBuffer = get_command_buffer();

	// clear buffer
	reset_command_buffer(commandBuffer);

	// start new buffer
	begin_command_buffer(commandBuffer);

	return 0;
}

void Minty::VulkanRenderer::end_frame()
{
	Frame& frame = get_current_frame();

	// get working command buffer for the current frame
	VkCommandBuffer commandBuffer = get_command_buffer();

	// end buffer
	end_command_buffer(commandBuffer);

	// submit command buffer
	submit_command_buffer(commandBuffer, frame, s_graphicsQueue);

	// present to the screen
	VkResult result = present_frame(s_imageIndex, frame.renderFinishedSemaphore);

	// check for recreating swapchain
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		recreate_swapchain();
	}

	// increment frame index
	s_currentFrame = (s_currentFrame + 1ull) % MAX_FRAMES_IN_FLIGHT;
}

void Minty::VulkanRenderer::start_render_pass(Ref<VulkanRenderPass> const& renderPass, Ref<VulkanRenderTarget> const& renderTarget)
{
	// start pass
	UInt2 targetSize = renderTarget->get_size();
	VkRect2D renderArea{ {0, 0}, {targetSize.x, targetSize.y } };
	Color clearColor = Renderer::get_color();
	VkClearColorValue clearValue = { { clearColor.rf(), clearColor.gf(), clearColor.bf(), clearColor.af() } };
	begin_render_pass(get_command_buffer(), renderPass->get_render_pass(), renderTarget->get_framebuffer(s_imageIndex), renderArea, clearValue);
}

void Minty::VulkanRenderer::end_render_pass()
{
	end_render_pass(get_command_buffer());
}

void Minty::VulkanRenderer::transition_between_render_passes()
{
	Ref<VulkanImage> swapchainImage = s_swapchainImages.at(s_imageIndex).create_ref();

	// set barrier between pass 1 and 2
	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = swapchainImage->get_image();
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	vkCmdPipelineBarrier(
		get_command_buffer(),
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier);
}

void Minty::VulkanRenderer::draw_vertices(UInt const vertexCount)
{
	draw_vertices(get_command_buffer(), vertexCount);
}

void Minty::VulkanRenderer::draw_instances(UInt const instanceCount, UInt const vertexCount)
{
	draw_instances(get_command_buffer(), instanceCount, vertexCount);
}

void Minty::VulkanRenderer::draw_indices(UInt const indexCount)
{
	draw_indices(get_command_buffer(), indexCount);
}

void Minty::VulkanRenderer::sync()
{
	wait_until_device_idle();
}

void Minty::VulkanRenderer::refresh()
{
	recreate_swapchain();
}

Ref<RenderTarget> Minty::VulkanRenderer::create_render_target(Ref<RenderPass> const& renderPass)
{
	std::vector<Ref<Image>> swapchainImageRefs(s_swapchainImages.size());
	for (Size i = 0; i < swapchainImageRefs.size(); i++)
	{
		swapchainImageRefs[i] = s_swapchainImages.at(i).create_ref();
	}

	RenderTargetBuilder builder{};
	builder.renderPass = renderPass;
	builder.images = swapchainImageRefs;

	return renderPass->create_render_target(builder);
}

////////////////////// Utility

static const std::vector<const Char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

static const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// only use validation layers when in debug mode
#ifdef MINTY_DEBUG

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	String message = std::format("Validation layer: {}", pCallbackData->pMessage);

	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		Debug::log_error(message);
		Debug::log_stack_trace();
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		Debug::log_warning(message);
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
	{
		Debug::log(message);
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
	{
		Debug::log_info(message);
	}
	else
	{
		// should never get here, but, I digress
		Debug::log(std::format("Unknown debug message severity. Message: {}", message));
	}

	return VK_FALSE;
}

static Bool check_validation_layer_support()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// validate that each layer we want exists
	for (const char* layerName : validationLayers)
	{
		Bool layerFound = false;

		for (auto const& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

static VkDebugUtilsMessengerEXT s_debugMessenger;

static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

#endif // MINTY_DEBUG

void Minty::VulkanRenderer::setup_debug_messenger()
{
#ifdef MINTY_DEBUG
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	populateDebugMessengerCreateInfo(createInfo);

	VK_ASSERT_RESULT(CreateDebugUtilsMessengerEXT(s_instance, &createInfo, nullptr, &s_debugMessenger), "Failed to set up debug messenger.");
#endif // MINTY_DEBUG
}

void Minty::VulkanRenderer::cleanup_debug_messenger()
{
#ifdef MINTY_DEBUG
	DestroyDebugUtilsMessengerEXT(s_instance, s_debugMessenger, nullptr);
#endif // MINTY_DEBUG
}

static std::vector<const char*> get_required_extensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef MINTY_DEBUG
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif // MINTY_DEBUG

	return extensions;
}

void Minty::VulkanRenderer::create_instance(const Minty::ApplicationInfo& applicationInfo)
{
#ifdef MINTY_DEBUG
	if (!check_validation_layer_support())
	{
		throw std::runtime_error("Validation layers requested, but not available.");
	}
#endif // MINTY_DEBUG

	// Enter application info
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = applicationInfo.name.c_str();
	appInfo.applicationVersion = applicationInfo.version;
	appInfo.pEngineName = MINTY_NAME;
	appInfo.engineVersion = MINTY_VERSION;
	appInfo.apiVersion = VK_API_VERSION_1_2;

	// Enter args to create instance
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> extensions = get_required_extensions();

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
#ifdef MINTY_DEBUG
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();

	populateDebugMessengerCreateInfo(debugCreateInfo);
	createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
#else
	createInfo.enabledLayerCount = 0;
	createInfo.pNext = nullptr;
#endif // MINTY_DEBUG

	// create the VkInstance
	VK_ASSERT_RESULT(vkCreateInstance(&createInfo, nullptr, &s_instance), "Failed to create instance.");
}

void Minty::VulkanRenderer::destroy_instance()
{
	vkDestroyInstance(s_instance, nullptr);
	s_instance = VK_NULL_HANDLE;
}

std::vector<VkExtensionProperties> Minty::VulkanRenderer::get_instance_extensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	return extensions;
}

VkSurfaceKHR Minty::VulkanRenderer::create_surface(Ref<Window> const window)
{
	VK_ASSERT_RESULT_RETURN_OBJECT(VkSurfaceKHR, glfwCreateWindowSurface(s_instance, static_cast<GLFWwindow*>(window->get_native()), nullptr, &object), "Failed to create window surface.");
}

void Minty::VulkanRenderer::destroy_surface(VkSurfaceKHR const surface)
{
	vkDestroySurfaceKHR(s_instance, surface, nullptr);
}

static Bool check_device_extension_support(VkPhysicalDevice const physicalDevice)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

	std::set<String> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (auto const& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void Minty::VulkanRenderer::select_physical_device()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(s_instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("Failed to find a GPU with Vulkan support.");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(s_instance, &deviceCount, devices.data());

	// find best candidate from devices
	std::multimap<int, VkPhysicalDevice> candidates;

	for (auto const& device : devices)
	{
		int score = rate_device_suitability(device);
		candidates.insert(std::make_pair(score, device));
	}

	// if best candidate is not suitable at all, womp womp
	if (candidates.rbegin()->first <= 0)
	{
		throw std::runtime_error("Failed to find a suitable GPU.");
	}

	s_physicalDevice = candidates.rbegin()->second;
}

int Minty::VulkanRenderer::rate_device_suitability(VkPhysicalDevice const physicalDevice)
{
	// check if device queues can handle what we want
	QueueFamilyIndices queueFamilyIndices = find_queue_families(physicalDevice);
	if (!queueFamilyIndices.is_complete())
	{
		return 0;
	}

	// check if device supports extensions (needed for rendering to a window surface)
	Bool extensionsSupported = check_device_extension_support(physicalDevice);
	if (!extensionsSupported)
	{
		return 0;
	}

	// check if swap chains will work
	SwapChainSupportDetails swapChainSupport = query_swap_chain_support(physicalDevice);
	if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty())
	{
		return 0;
	}

	// check for sampler anisotropy
	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);
	if (!supportedFeatures.samplerAnisotropy)
	{
		return 0;
	}

	// anything works for now
	return 1;

	//VkPhysicalDeviceProperties deviceProperties;
	//vkGetPhysicalDeviceProperties(device, &deviceProperties);

	//VkPhysicalDeviceFeatures deviceFeatures;
	//vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	//int score = 0;

	//// Discrete GPUs have a significant performance advantage
	//if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
	//    score += 1000;
	//}

	//// Maximum possible size of textures affects graphics quality
	//score += deviceProperties.limits.maxImageDimension2D;

	//// Application can't function without geometry shaders
	//if (!deviceFeatures.geometryShader) {
	//    return 0;
	//}

	//return score;
}

Minty::VulkanRenderer::SwapChainSupportDetails Minty::VulkanRenderer::query_swap_chain_support(VkPhysicalDevice const physicalDevice)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, s_surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, s_surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, s_surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, s_surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, s_surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

VkSurfaceFormatKHR Minty::VulkanRenderer::select_swap_surface_format(std::vector<VkSurfaceFormatKHR> const& availableFormats, VkFormat const format, VkColorSpaceKHR const colorSpace)
{
	for (auto const& availableFormat : availableFormats)
	{
		if (availableFormat.format == format && availableFormat.colorSpace == colorSpace)
		{
			return availableFormat;
		}
	}

	return availableFormats.at(0);
}

VkExtent2D Minty::VulkanRenderer::select_swap_extent(VkSurfaceCapabilitiesKHR const& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		// extent has been initialized, use it
		return capabilities.currentExtent;
	}
	else
	{
		// create new extent from window size
		int width, height;
		glfwGetFramebufferSize(static_cast<GLFWwindow*>(Renderer::get_window()->get_native()), &width, &height);

		VkExtent2D actualExtent =
		{
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = Math::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = Math::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

VkPresentModeKHR Minty::VulkanRenderer::select_swap_present_mode(std::vector<VkPresentModeKHR> const& availablePresentModes, VkPresentModeKHR const presentMode)
{
	for (auto const& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == presentMode)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

Minty::VulkanRenderer::QueueFamilyIndices Minty::VulkanRenderer::find_queue_families(VkPhysicalDevice const physicalDevice)
{
	QueueFamilyIndices queueFamilyIndices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (auto const& queueFamily : queueFamilies)
	{
		// check graphics family
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			queueFamilyIndices.graphicsFamily = i;
		}

		// check present family
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, s_surface, &presentSupport);
		if (presentSupport)
		{
			queueFamilyIndices.presentFamily = i;
		}

		// if all found, quit looking
		if (queueFamilyIndices.is_complete())
		{
			break;
		}

		i++;
	}

	return queueFamilyIndices;
}

VkQueue Minty::VulkanRenderer::get_device_queue(const uint32_t index)
{
	VkQueue queue;

	vkGetDeviceQueue(s_device, index, 0, &queue);

	return queue;
}

void Minty::VulkanRenderer::create_device()
{
	// create all the queues needed for operations, based on the families
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { s_queueFamilyIndices.graphicsFamily.value(), s_queueFamilyIndices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (const uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

#ifdef MINTY_DEBUG
	// tie in validation layers
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
#else
	createInfo.enabledLayerCount = 0;
#endif // MINTY_DEBUG

	VK_ASSERT_RESULT(vkCreateDevice(s_physicalDevice, &createInfo, nullptr, &s_device), "Failed to create logical device.");
}

void Minty::VulkanRenderer::destroy_device()
{
	vkDestroyDevice(s_device, nullptr);
	s_device = VK_NULL_HANDLE;
}

void Minty::VulkanRenderer::wait_until_device_idle()
{
	vkDeviceWaitIdle(s_device);
}

void Minty::VulkanRenderer::create_swapchain(VkSurfaceFormatKHR const surfaceFormat, VkExtent2D const extent, VkPresentModeKHR const presentMode)
{
	SwapChainSupportDetails swapchainSupport = query_swap_chain_support(s_physicalDevice);

	// images in the swapchain
	uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;

	// limit image count
	if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount)
	{
		imageCount = swapchainSupport.capabilities.maxImageCount;
	}

	// create the swap chain
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = s_surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	uint32_t queueFamilyIndices[] = { s_queueFamilyIndices.graphicsFamily.value(), s_queueFamilyIndices.presentFamily.value() };

	if (s_queueFamilyIndices.graphicsFamily != s_queueFamilyIndices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	// specify if a transform is to be made (ex. 90 degree rotation)
	createInfo.preTransform = swapchainSupport.capabilities.currentTransform;

	// if this window should blend with other windows
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	// enable clipping, in case another window is in front of this one
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	// come back to this
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VK_ASSERT_RESULT(vkCreateSwapchainKHR(s_device, &createInfo, nullptr, &s_swapchain), "Failed to create swap chain.");
}

void Minty::VulkanRenderer::destroy_swapchain()
{
	vkDestroySwapchainKHR(s_device, s_swapchain, nullptr);
	s_swapchain = VK_NULL_HANDLE;
}

std::vector<VkImage> Minty::VulkanRenderer::get_swapchain_images()
{
	std::vector<VkImage> images;
	uint32_t imageCount;

	vkGetSwapchainImagesKHR(s_device, s_swapchain, &imageCount, nullptr);
	images.resize(imageCount);
	vkGetSwapchainImagesKHR(s_device, s_swapchain, &imageCount, images.data());

	return images;
}

VkResult Minty::VulkanRenderer::get_next_swapchain_image(VkSemaphore const waitSemaphore, uint32_t& index)
{
	VkResult result = vkAcquireNextImageKHR(s_device, s_swapchain, UINT64_MAX, waitSemaphore, VK_NULL_HANDLE, &index);

	// if not a success and not suboptimal, error
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed to acquire swap chain image.");
	}

	return result;
}

VkFormat Minty::VulkanRenderer::find_supported_format(const std::vector<VkFormat>& candidates, VkImageTiling const tiling, VkFormatFeatureFlags const features)
{
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(s_physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	VK_ASSERT_FAIL("Failed to find supported depthFormat.");
}

VkImage Minty::VulkanRenderer::create_image(const uint32_t width, const uint32_t height, VkImageType const type, VkFormat const format, VkImageTiling const tiling, VkImageUsageFlags const usage)
{
	// create image from builder
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = type;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0;

	VK_ASSERT_RESULT_RETURN_OBJECT(VkImage, vkCreateImage(s_device, &imageInfo, nullptr, &object), "Failed to create image.");
}

void Minty::VulkanRenderer::create_image_and_memory(const uint32_t width, const uint32_t height, VkImageType const type, VkFormat const format, VkImageTiling const tiling, VkImageUsageFlags const usage, VkMemoryPropertyFlags const memoryProperties, VkImage& image, VkDeviceMemory& memory)
{
	image = create_image(width, height, type, format, tiling, usage);

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(s_device, image, &memRequirements);

	memory = allocate_memory(memRequirements.size, find_memory_type(memRequirements.memoryTypeBits, memoryProperties));

	bind_image_memory(image, memory);
}

void Minty::VulkanRenderer::destroy_image(VkImage const image)
{
	vkDestroyImage(s_device, image, nullptr);
}

void Minty::VulkanRenderer::bind_image_memory(VkImage const image, VkDeviceMemory const memory)
{
	VK_ASSERT_RESULT(vkBindImageMemory(s_device, image, memory, 0), "Failed to bind image memory.");
}

void Minty::VulkanRenderer::transition_image_layout(VkImage const image, VkFormat const format, VkImageLayout const oldLayout, VkImageLayout const newLayout)
{
	VkCommandBuffer commandBuffer = begin_command_buffer_single();

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;

	// set if transfering queues
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	// affected part of image
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	// determine when the transfer can be conducted based on its layout
	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;
	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		MINTY_ABORT("Unsupported layout transition.");
	}

	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	end_command_buffer_single(commandBuffer, s_graphicsQueue);
}

VkImageView Minty::VulkanRenderer::create_image_view(VkImage const image, VkFormat const format, VkImageAspectFlags const aspectFlags, VkImageViewType const viewType)
{
	VkImageViewCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = viewType;
	createInfo.format = format;

	// move the channels around, if needed
	createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	createInfo.subresourceRange.aspectMask = aspectFlags;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	VK_ASSERT_RESULT_RETURN_OBJECT(VkImageView, vkCreateImageView(s_device, &createInfo, nullptr, &object), "Failed to create image view.");
}

std::vector<VkImageView> Minty::VulkanRenderer::create_image_views(const std::vector<VkImage>& images, VkFormat const format, VkImageAspectFlags const aspectFlags, VkImageViewType const viewType)
{
	std::vector<VkImageView> imageViews(images.size());

	for (Size i = 0; i < images.size(); i++)
	{
		imageViews[i] = create_image_view(images.at(i), format, aspectFlags, viewType);
	}

	return imageViews;
}

void Minty::VulkanRenderer::destroy_image_view(VkImageView const imageView)
{
	vkDestroyImageView(s_device, imageView, nullptr);
}

VkSampler Minty::VulkanRenderer::create_sampler(VkFilter const magFilter, VkFilter const minFilter, VkSamplerAddressMode const addressMode, VkBorderColor const borderColor, Bool const normalizedCoordinates)
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

	// what to do when image is too close
	samplerInfo.magFilter = magFilter;

	// what to do when image is too far
	samplerInfo.minFilter = minFilter;

	// what to do when sampling outside of image
	samplerInfo.addressModeU = addressMode;
	samplerInfo.addressModeV = addressMode;
	samplerInfo.addressModeW = addressMode;

	// limit the number of samples that can be used
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(s_physicalDevice, &properties);

	// defaults: VK_FALSE, 1.0f
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

	// set border color, if clamping
	samplerInfo.borderColor = borderColor;

	// use pixel coordinates instead of normalized?
	samplerInfo.unnormalizedCoordinates = !normalizedCoordinates;

	// 
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	// mipmapping
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	// create sampler
	VK_ASSERT_RESULT_RETURN_OBJECT(VkSampler, vkCreateSampler(s_device, &samplerInfo, nullptr, &object), "Failed to create image sampler.");
}

void Minty::VulkanRenderer::destroy_sampler(VkSampler const sampler)
{
	vkDestroySampler(s_device, sampler, nullptr);
}

VkFormat Minty::VulkanRenderer::find_depth_format()
{
	return find_supported_format(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

Bool Minty::VulkanRenderer::has_stencil_component(VkFormat const format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void Minty::VulkanRenderer::create_depth_resources()
{
	VkFormat depthFormat = find_depth_format();

	ImageBuilder depthImageBuilder{};
	depthImageBuilder.width = s_swapchainExtent.width;
	depthImageBuilder.height = s_swapchainExtent.height;
	depthImageBuilder.aspect = ImageAspect::Depth;
	depthImageBuilder.format = static_cast<Format>(depthFormat);
	depthImageBuilder.tiling = ImageTiling::Optimal;
	depthImageBuilder.type = ImageType::D2;
	depthImageBuilder.usage = ImageUsage::DepthStencil;
	depthImageBuilder.immutable = false;

	s_depthImage = Owner<VulkanImage>(depthImageBuilder);
}

void Minty::VulkanRenderer::destroy_depth_resources()
{
	s_depthImage.release();
}

void Minty::VulkanRenderer::recreate_depth_resources()
{
	VkFormat vulkanDepthFormat = find_depth_format();

	// TODO: convert to format in a safer way
	Format depthFormat = static_cast<Format>(vulkanDepthFormat);

	s_depthImage->resize(s_swapchainExtent.width, s_swapchainExtent.height, depthFormat);
}

VkShaderModule Minty::VulkanRenderer::create_shader_module(const std::vector<Minty::Char>& code)
{
	MINTY_ASSERT_MESSAGE(!code.empty(), "Cannot create a shader module with no code.");

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VK_ASSERT_RESULT_RETURN_OBJECT(VkShaderModule, vkCreateShaderModule(s_device, &createInfo, nullptr, &object), "Failed to create shader module.");
}

VkShaderModule Minty::VulkanRenderer::create_shader_module(Path const& path)
{
	std::vector<Minty::Char> code = AssetManager::read_file_chars(path);
	return create_shader_module(code);
}

void Minty::VulkanRenderer::destroy_shader_module(VkShaderModule const shaderModule)
{
	MINTY_ASSERT(shaderModule != VK_NULL_HANDLE);

	vkDestroyShaderModule(s_device, shaderModule, nullptr);
}

VkRenderPass Minty::VulkanRenderer::create_render_pass(VkAttachmentDescription const* const colorAttachment, VkAttachmentDescription const* const depthAttachment)
{
	std::vector<VkAttachmentDescription> attachments;
	attachments.reserve(2);
	std::array<VkAttachmentReference, 2> attachmentRefs = {};

	if (colorAttachment)
	{
		attachmentRefs[0].attachment = static_cast<uint32_t>(attachments.size());
		attachmentRefs[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachments.push_back(*colorAttachment);
	}

	if (depthAttachment)
	{
		attachmentRefs[1].attachment = static_cast<uint32_t>(attachments.size());
		attachmentRefs[1].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		attachments.push_back(*depthAttachment);
	}

	//// color attachment
	//VkAttachmentDescription colorAttachment{};
	//colorAttachment.format = s_swapchainSurfaceFormat.format;
	//colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // adjust if multisampling
	//colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // determine what to do with previous data
	//colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // determine what to do with the current data
	//colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	//colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	//colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	//// color attachment ref
	//VkAttachmentReference colorAttachmentRef{};
	//colorAttachmentRef.attachment = 0;
	//colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//// depth attachment
	//VkAttachmentDescription depthAttachment{};
	//depthAttachment.format = find_depth_format();
	//depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	//depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	//depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	//depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	//depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	//depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//// depth attachment ref
	//VkAttachmentReference depthAttachmentRef{};
	//depthAttachmentRef.attachment = 1;
	//depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// subpass
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 0;
	subpass.pColorAttachments = nullptr;
	subpass.pDepthStencilAttachment = nullptr;

	// behave differently based on attachments
	if (colorAttachment)
	{
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &attachmentRefs[0];
	}

	if (depthAttachment)
	{
		subpass.pDepthStencilAttachment = &attachmentRefs[1];
	}

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 0;
	renderPassInfo.pDependencies = nullptr;

	VK_ASSERT_RESULT_RETURN_OBJECT(VkRenderPass, vkCreateRenderPass(s_device, &renderPassInfo, nullptr, &object), "Failed to create render pass.")
}

void Minty::VulkanRenderer::destroy_render_pass(VkRenderPass const renderPass)
{
	MINTY_ASSERT(renderPass != VK_NULL_HANDLE);

	vkDestroyRenderPass(s_device, renderPass, nullptr);
}

VkFramebuffer Minty::VulkanRenderer::create_framebuffer(VkRenderPass const renderPass, VkImageView const attachment, VkExtent2D const extent, Bool const useColorAttachment, Bool const useDepthAttachment)
{
	std::vector<VkImageView> attachments;
	attachments.reserve(2);

	if (useColorAttachment)
	{
		attachments.push_back(attachment);
	}
	if (useDepthAttachment)
	{
		attachments.push_back(s_depthImage->get_view());
	}

	VkFramebufferCreateInfo framebufferInfo{};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = renderPass;
	framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	framebufferInfo.pAttachments = attachments.data();
	framebufferInfo.width = extent.width;
	framebufferInfo.height = extent.height;
	framebufferInfo.layers = 1;

	VK_ASSERT_RESULT_RETURN_OBJECT(VkFramebuffer, vkCreateFramebuffer(s_device, &framebufferInfo, nullptr, &object), "Failed to create framebuffer.");
}

void Minty::VulkanRenderer::destroy_framebuffer(VkFramebuffer const framebuffer)
{
	vkDestroyFramebuffer(s_device, framebuffer, nullptr);
}

void Minty::VulkanRenderer::create_command_pool(const uint32_t queueFamilyIndex)
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndex;

	VK_ASSERT_RESULT(vkCreateCommandPool(s_device, &poolInfo, nullptr, &s_commandPool), "Failed to create command pool.");
}

void Minty::VulkanRenderer::destroy_command_pool()
{
	vkDestroyCommandPool(s_device, s_commandPool, nullptr);
	s_commandPool = VK_NULL_HANDLE;
}

VkCommandBuffer Minty::VulkanRenderer::create_command_buffer()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = s_commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	VK_ASSERT_RESULT_RETURN_OBJECT(VkCommandBuffer, vkAllocateCommandBuffers(s_device, &allocInfo, &object), "Failed to allocate command buffers.");
}

void Minty::VulkanRenderer::destroy_command_buffer(VkCommandBuffer const commandBuffer)
{
	vkFreeCommandBuffers(s_device, s_commandPool, 1, &commandBuffer);
}

void Minty::VulkanRenderer::begin_command_buffer(VkCommandBuffer const commandBuffer)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VK_ASSERT_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Failed to begin recording command buffer.");
}

void Minty::VulkanRenderer::begin_command_buffer_temp(VkCommandBuffer const commandBuffer)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VK_ASSERT_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Failed to begin recording temp command buffer.");
}

void Minty::VulkanRenderer::end_command_buffer(VkCommandBuffer const commandBuffer)
{
	VK_ASSERT_RESULT(vkEndCommandBuffer(commandBuffer), "Failed to record command buffer.");
}

VkCommandBuffer Minty::VulkanRenderer::begin_command_buffer_single()
{
	VkCommandBuffer commandBuffer = create_command_buffer();

	begin_command_buffer_temp(commandBuffer);

	return commandBuffer;
}

void Minty::VulkanRenderer::end_command_buffer_single(VkCommandBuffer& commandBuffer, VkQueue const queue)
{
	end_command_buffer(commandBuffer);

	submit_command_buffer(commandBuffer, queue);
	vkQueueWaitIdle(queue);

	destroy_command_buffer(commandBuffer);
}

void Minty::VulkanRenderer::reset_command_buffer(VkCommandBuffer const commandBuffer)
{
	vkResetCommandBuffer(commandBuffer, 0);
}

void Minty::VulkanRenderer::submit_command_buffer(VkCommandBuffer const commandBuffer, VkQueue const queue, VkSemaphore const waitSemaphore, VkSemaphore const signalSemaphore, VkFence const inFlightFence)
{
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { waitSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VkSemaphore signalSemaphores[] = { signalSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	VK_ASSERT_RESULT(vkQueueSubmit(queue, 1, &submitInfo, inFlightFence), "Failed to submit draw command buffer.");
}

void Minty::VulkanRenderer::submit_command_buffer(VkCommandBuffer const commandBuffer, Frame const& frame, VkQueue const queue)
{
	submit_command_buffer(commandBuffer, queue, frame.imageAvailableSemaphore, frame.renderFinishedSemaphore, frame.inFlightFence);
}

void Minty::VulkanRenderer::submit_command_buffer(VkCommandBuffer const commandBuffer, VkQueue const queue)
{
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VK_ASSERT_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit command buffer.");
}

void Minty::VulkanRenderer::begin_render_pass(VkCommandBuffer const commandBuffer, VkRenderPass const renderPass, VkFramebuffer const framebuffer, VkRect2D const renderArea, VkClearColorValue const clearColor)
{
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = framebuffer;

	// determine where to draw_vertices
	// keep same as viewport for efficiency
	renderPassInfo.renderArea = renderArea;

	// clear color to cover last frame
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = clearColor;
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void Minty::VulkanRenderer::end_render_pass(VkCommandBuffer const commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}

void Minty::VulkanRenderer::bind_pipeline(VkCommandBuffer const commandBuffer, VkPipeline const graphicsPipeline, VkPipelineBindPoint const bindPoint)
{
	vkCmdBindPipeline(commandBuffer, bindPoint, graphicsPipeline);
}

void Minty::VulkanRenderer::bind_descriptor_set(VkCommandBuffer const commandBuffer, VkPipelineLayout const graphicsPipelineLayout, VkDescriptorSet const descriptorSet)
{
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
}

void Minty::VulkanRenderer::bind_viewport(VkCommandBuffer const commandBuffer, VkViewport const& viewport)
{
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
}

void Minty::VulkanRenderer::bind_scissor(VkCommandBuffer const commandBuffer, VkRect2D const& scissor)
{
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Minty::VulkanRenderer::bind_vertex_buffer(VkCommandBuffer const commandBuffer, VkBuffer const buffer, UInt const binding)
{
	VkBuffer vertexBuffers[] = { buffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, binding, 1, vertexBuffers, offsets);
}

void Minty::VulkanRenderer::bind_index_buffer(VkCommandBuffer const commandBuffer, VkBuffer const buffer)
{
	vkCmdBindIndexBuffer(commandBuffer, buffer, 0, VK_INDEX_TYPE_UINT16);
}

void Minty::VulkanRenderer::draw_vertices(VkCommandBuffer const commandBuffer, const uint32_t count)
{
	vkCmdDraw(commandBuffer, count, 1, 0, 0);
}

void Minty::VulkanRenderer::draw_instances(VkCommandBuffer const commandBuffer, const uint32_t count, const uint32_t vertexCount)
{
	vkCmdDraw(commandBuffer, vertexCount, count, 0, 0);
}

void Minty::VulkanRenderer::draw_indices(VkCommandBuffer const commandBuffer, const uint32_t count)
{
	vkCmdDrawIndexed(commandBuffer, count, 1, 0, 0, 0);
}

VkSemaphore Minty::VulkanRenderer::create_semaphore()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VK_ASSERT_RESULT_RETURN_OBJECT(VkSemaphore, vkCreateSemaphore(s_device, &semaphoreInfo, nullptr, &object), "Failed to create semaphore.");
}

void Minty::VulkanRenderer::destroy_semaphore(VkSemaphore const semaphore)
{
	vkDestroySemaphore(s_device, semaphore, nullptr);
}

VkFence Minty::VulkanRenderer::create_fence()
{
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VK_ASSERT_RESULT_RETURN_OBJECT(VkFence, vkCreateFence(s_device, &fenceInfo, nullptr, &object), "Failed to create fence.");
}

void Minty::VulkanRenderer::destroy_fence(VkFence const fence)
{
	vkDestroyFence(s_device, fence, nullptr);
}

void Minty::VulkanRenderer::wait_for_fence(VkFence& fence)
{
	vkWaitForFences(s_device, 1, &fence, VK_TRUE, UINT64_MAX);
}

void Minty::VulkanRenderer::reset_fence(VkFence& fence)
{
	vkResetFences(s_device, 1, &fence);
}

VkResult Minty::VulkanRenderer::present_frame(uint32_t& imageIndex, VkSemaphore const signalSemaphore)
{
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	// specify which semaphore to wait on
	VkSemaphore signalSemaphores[] = { signalSemaphore };
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	// submit swap chain
	VkSwapchainKHR swapchains[] = { s_swapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapchains;

	// specify which image to use
	presentInfo.pImageIndices = &imageIndex;

	// check results of swap chain?
	presentInfo.pResults = nullptr;

	// present it
	VkResult result = vkQueuePresentKHR(s_presentQueue, &presentInfo);

	if (result != VK_SUCCESS && result != VK_ERROR_OUT_OF_DATE_KHR && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed to present the frame.");
	}

	return result;
}

VkBuffer Minty::VulkanRenderer::create_buffer(VkDeviceSize const size, VkBufferUsageFlags const usage)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // data can be shared between queues/queue families

	VK_ASSERT_RESULT_RETURN_OBJECT(VkBuffer, vkCreateBuffer(s_device, &bufferInfo, nullptr, &object), "Failed to create buffer.");
}

void Minty::VulkanRenderer::create_buffer_and_memory(VkDeviceSize const size, VkBufferUsageFlags const usage, VkMemoryPropertyFlags const properties, VkBuffer& buffer, VkDeviceMemory& memory)
{
	// create buffer
	buffer = create_buffer(size, usage);

	// get memory requirements
	VkMemoryRequirements requirements;
	get_buffer_memory_requirements(buffer, requirements);

	// allocate memory
	uint32_t memoryTypeIndex = find_memory_type(requirements.memoryTypeBits, properties);
	memory = allocate_memory(requirements.size, memoryTypeIndex);

	// bind memory to buffer
	bind_buffer_memory(buffer, memory);
}

void Minty::VulkanRenderer::destroy_buffer(VkBuffer const buffer)
{
	vkDestroyBuffer(s_device, buffer, nullptr);
}

void Minty::VulkanRenderer::destroy_buffer_and_memory(VkBuffer const buffer, VkDeviceMemory const memory)
{
	destroy_buffer(buffer);
	free_memory(memory);
}

void Minty::VulkanRenderer::get_buffer_memory_requirements(VkBuffer const buffer, VkMemoryRequirements& requirements)
{
	vkGetBufferMemoryRequirements(s_device, buffer, &requirements);
}

void Minty::VulkanRenderer::bind_buffer_memory(VkBuffer const buffer, VkDeviceMemory const memory)
{
	VK_ASSERT_RESULT(vkBindBufferMemory(s_device, buffer, memory, 0), "Failed to bind memory to buffer.");
}

void Minty::VulkanRenderer::copy_buffer_to_buffer(VkQueue const queue, VkBuffer const srcBuffer, VkBuffer const dstBuffer, VkDeviceSize const size)
{
	// get command buffer
	VkCommandBuffer commandBuffer = begin_command_buffer_single();

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	end_command_buffer_single(commandBuffer, queue);
}

void Minty::VulkanRenderer::copy_buffer_to_image(VkQueue const queue, VkBuffer const srcBuffer, VkImage const dstImage, const uint32_t width, const uint32_t height)
{
	VkCommandBuffer commandBuffer = begin_command_buffer_single();

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

	vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	end_command_buffer_single(commandBuffer, queue);
}

uint32_t Minty::VulkanRenderer::find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	// get physical device memory properties
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(s_physicalDevice, &memoryProperties);

	// find memory that is suitable for a buffer
	// also find memory find memory with the given specific properties
	for (uint32_t i = 0; memoryProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type.");
}

VkDeviceMemory Minty::VulkanRenderer::allocate_memory(VkDeviceSize const size, const uint32_t memoryTypeIndex)
{
	VkMemoryAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.allocationSize = size;
	allocateInfo.memoryTypeIndex = memoryTypeIndex;

	VK_ASSERT_RESULT_RETURN_OBJECT(VkDeviceMemory, vkAllocateMemory(s_device, &allocateInfo, nullptr, &object), "Failed to allocate buffer memory.");
}

void Minty::VulkanRenderer::free_memory(VkDeviceMemory const memory)
{
	vkFreeMemory(s_device, memory, nullptr);
}

void* Minty::VulkanRenderer::map_memory(VkDeviceMemory const memory, VkDeviceSize const offset, VkDeviceSize const size)
{
	VK_ASSERT_RESULT_RETURN_OBJECT(void*, vkMapMemory(s_device, memory, offset, size, 0, &object), "Failed to map memory.");
}

void Minty::VulkanRenderer::unmap_memory(VkDeviceMemory const memory)
{
	vkUnmapMemory(s_device, memory);
}

void Minty::VulkanRenderer::set_memory(VkDeviceMemory const memory, const void* const data, VkDeviceSize const offset, VkDeviceSize const size)
{
	void* dest = map_memory(memory, offset, size);
	memcpy(dest, data, size);
	unmap_memory(memory);
}

VkFormat Minty::VulkanRenderer::format_to_vulkan(const Minty::Format format)
{
	// numbers SHOULD be the same
	return static_cast<VkFormat>(format);
}

VkBufferUsageFlags Minty::VulkanRenderer::buffer_usage_to_vulkan(const Minty::BufferUsage bufferUsage)
{
	switch (bufferUsage)
	{
	case Minty::BufferUsage::TRANSFER:
		return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	case Minty::BufferUsage::VERTEX:
		return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	case Minty::BufferUsage::INDEX:
		return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	case Minty::BufferUsage::UNIFORM:
		return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	default:
		MINTY_ABORT("Unknown BufferUsage.");
	}
}

VkShaderStageFlags Minty::VulkanRenderer::shader_stage_to_vulkan(const Minty::ShaderStage shaderStage)
{
	switch (shaderStage)
	{
	case Minty::ShaderStage::Vertex:
		return VK_SHADER_STAGE_VERTEX_BIT;
	case Minty::ShaderStage::Fragment:
		return VK_SHADER_STAGE_FRAGMENT_BIT;
	default:
		MINTY_ABORT("Unknown ShaderStage.");
	}
}

VkDescriptorType Minty::VulkanRenderer::descriptor_type_to_vulkan(const Minty::ShaderInputType type)
{
	// numbers SHOULD be the same, offset
	return static_cast<VkDescriptorType>(static_cast<int>(type) - 1);
}

VkFormat Minty::VulkanRenderer::type_to_vulkan(const Minty::Type type)
{
	switch (type)
	{
	case Type::Float:
		return VkFormat::VK_FORMAT_R32_SFLOAT;
	case Type::Float2:
		return VkFormat::VK_FORMAT_R32G32_SFLOAT;
	case Type::Float3:
		return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
	case Type::Float4:
		return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
	case Type::Int:
		return VkFormat::VK_FORMAT_R32_SINT;
	case Type::Int2:
		return VkFormat::VK_FORMAT_R32G32_SINT;
	case Type::Int3:
		return VkFormat::VK_FORMAT_R32G32B32_SINT;
	case Type::Int4:
		return VkFormat::VK_FORMAT_R32G32B32A32_SINT;
	case Type::UInt:
		return VkFormat::VK_FORMAT_R32_UINT;
	case Type::UInt2:
		return VkFormat::VK_FORMAT_R32G32_UINT;
	case Type::UInt3:
		return VkFormat::VK_FORMAT_R32G32B32_UINT;
	case Type::UInt4:
		return VkFormat::VK_FORMAT_R32G32B32A32_UINT;
	default:
		MINTY_ABORT_FORMAT("Unable to convert Type \"{}\" to VkFormat.", to_string(type));
	}
}

VkImageType Minty::VulkanRenderer::image_type_to_vulkan(const Minty::ImageType type)
{
	switch (type)
	{
	case Minty::ImageType::D1:
		return VK_IMAGE_TYPE_1D;
	case Minty::ImageType::D2:
		return VK_IMAGE_TYPE_2D;
	case Minty::ImageType::D3:
		return VK_IMAGE_TYPE_3D;
	default:
		MINTY_ABORT("Unknown ImageType.");
	}
}

VkImageTiling Minty::VulkanRenderer::image_tiling_to_vulkan(const Minty::ImageTiling tiling)
{
	return static_cast<VkImageTiling>(tiling);
}

VkSamplerAddressMode Minty::VulkanRenderer::address_mode_to_vulkan(const Minty::ImageAddressMode addressMode)
{
	return static_cast<VkSamplerAddressMode>(addressMode);
}

VkImageAspectFlags Minty::VulkanRenderer::image_aspect_to_vulkan(const Minty::ImageAspect aspect)
{
	return static_cast<VkImageAspectFlags>(aspect);
}

VkImageUsageFlags Minty::VulkanRenderer::image_usage_to_vulkan(const Minty::ImageUsage usage)
{
	switch (usage)
	{
	case ImageUsage::Sampled:
		return VK_IMAGE_USAGE_SAMPLED_BIT;
	case ImageUsage::Storage:
		return VK_IMAGE_USAGE_STORAGE_BIT;
	case ImageUsage::Color:
		return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	case ImageUsage::DepthStencil:
		return VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	default:
		MINTY_ABORT("Unknown ImageUsage.");
	}
}

VkPrimitiveTopology Minty::VulkanRenderer::primitive_topology_to_vulkan(const Minty::ShaderPrimitiveTopology topology)
{
	return static_cast<VkPrimitiveTopology>(static_cast<Int>(topology) - 1);
}

VkCullModeFlags Minty::VulkanRenderer::cull_mode_to_vulkan(const Minty::ShaderCullMode mode)
{
	switch (mode)
	{
	case ShaderCullMode::None: return VkCullModeFlagBits::VK_CULL_MODE_NONE;
	case ShaderCullMode::Front: return VkCullModeFlagBits::VK_CULL_MODE_FRONT_BIT;
	case ShaderCullMode::Back: return VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;
	case ShaderCullMode::Both: return VkCullModeFlagBits::VK_CULL_MODE_FRONT_AND_BACK;
	default: return VkCullModeFlagBits::VK_CULL_MODE_NONE;
	}
}

VkFrontFace Minty::VulkanRenderer::front_face_to_vulkan(const Minty::ShaderFrontFace frontFace)
{
	switch (frontFace)
	{
	case ShaderFrontFace::Clockwise: return VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
	case ShaderFrontFace::CounterClockwise: return VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
	default: return VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
	}
}

VkPolygonMode Minty::VulkanRenderer::polygon_mode_to_vulkan(const Minty::ShaderPolygonMode mode)
{
	switch (mode)
	{
	case ShaderPolygonMode::Fill: return VkPolygonMode::VK_POLYGON_MODE_FILL;
	case ShaderPolygonMode::Line: return VkPolygonMode::VK_POLYGON_MODE_LINE;
	case ShaderPolygonMode::Point: return VkPolygonMode::VK_POLYGON_MODE_POINT;
	default: return VkPolygonMode::VK_POLYGON_MODE_FILL;
	}
}

VkVertexInputRate Minty::VulkanRenderer::input_rate_to_vulkan(const Minty::ShaderInputRate rate)
{
	return static_cast<VkVertexInputRate>(static_cast<Int>(rate) - 1);
}

VkAttachmentLoadOp Minty::VulkanRenderer::attachment_load_to_vulkan(Minty::RenderAttachmentLoadOperation const operation)
{
	return static_cast<VkAttachmentLoadOp>(operation);
}

VkAttachmentStoreOp Minty::VulkanRenderer::attachment_store_to_vulkan(Minty::RenderAttachmentStoreOperation const operation)
{
	return static_cast<VkAttachmentStoreOp>(operation);
}

VkAttachmentDescription Minty::VulkanRenderer::attachment_to_vulkan(Minty::RenderAttachment const& attachment)
{
	VkAttachmentDescription description{};

	description.format = format_to_vulkan(attachment.format);
	description.samples = VK_SAMPLE_COUNT_1_BIT;
	description.loadOp = attachment_load_to_vulkan(attachment.loadOperation);
	description.storeOp = attachment_store_to_vulkan(attachment.storeOperation);
	description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	description.initialLayout = VulkanRenderer::image_layout_to_vulkan(attachment.initialLayout);
	description.finalLayout = VulkanRenderer::image_layout_to_vulkan(attachment.finalLayout);

	return description;
}

VkImageLayout Minty::VulkanRenderer::image_layout_to_vulkan(Minty::ImageLayout const layout)
{
	return static_cast<VkImageLayout>(layout);
}
