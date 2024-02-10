#include "ME_Application.h"

#include "ME_Constants.h"
#include "ME_Project.h"
#include "ME_BuildInfo.h"

#include "ME_ConsoleWindow.h"
#include "ME_HierarchyWindow.h"
#include "ME_SceneWindow.h"
#include "ME_GameWindow.h"
#include "ME_PropertiesWindow.h"
#include "ME_AssetsWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <ImGuiFileDialog.h>

#include <Minty.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <filesystem> // for accessing file system
#include <fstream> // for accessing files
#include <chrono> // for time
#include <format>

#include <cstdio>
//#include <iostream>
#include <memory>
#include <stdexcept>
//#include <string>
#include <array>

namespace fs = std::filesystem;

#define CMAKE_PATH "C:/Users/mitch/source/repos/Minty-Engine/Editor/cmake/bin/cmake.exe"
#define APPLICATION_NAME "TestProject"
#define EXE_NAME std::string(APPLICATION_NAME).append(".exe")

using namespace mintye;
using namespace minty;

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
//#include <vulkan/vulkan_beta.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//#define IMGUI_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

// Data
static VkAllocationCallbacks* g_Allocator = nullptr;
static VkInstance               g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice                 g_Device = VK_NULL_HANDLE;
static uint32_t                 g_QueueFamily = (uint32_t)-1;
static VkQueue                  g_Queue = VK_NULL_HANDLE;
static VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
static VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
static VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;

static ImGui_ImplVulkanH_Window g_MainWindowData;
static int                      g_MinImageCount = 2;
static bool                     g_SwapChainRebuild = false;

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
static void check_vk_result(VkResult err)
{
	if (err == 0)
		return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0)
		abort();
}

#ifdef IMGUI_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
	(void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
	fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
	return VK_FALSE;
}
#endif // IMGUI_VULKAN_DEBUG_REPORT

static bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension)
{
	for (const VkExtensionProperties& p : properties)
		if (strcmp(p.extensionName, extension) == 0)
			return true;
	return false;
}

static VkPhysicalDevice SetupVulkan_SelectPhysicalDevice()
{
	uint32_t gpu_count;
	VkResult err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, nullptr);
	check_vk_result(err);
	IM_ASSERT(gpu_count > 0);

	ImVector<VkPhysicalDevice> gpus;
	gpus.resize(gpu_count);
	err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, gpus.Data);
	check_vk_result(err);

	// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
	// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
	// dedicated GPUs) is out of scope of this sample.
	for (VkPhysicalDevice& device : gpus)
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);
		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			return device;
	}

	// Use first GPU (Integrated) is a Discrete one is not available.
	if (gpu_count > 0)
		return gpus[0];
	return VK_NULL_HANDLE;
}

static void SetupVulkan(ImVector<const char*> instance_extensions)
{
	VkResult err;

	// Create Vulkan Instance
	{
		VkInstanceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		// Enumerate available extensions
		uint32_t properties_count;
		ImVector<VkExtensionProperties> properties;
		vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
		properties.resize(properties_count);
		err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, properties.Data);
		check_vk_result(err);

		// Enable required extensions
		if (IsExtensionAvailable(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
			instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
		if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
		{
			instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
			create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
		}
#endif

		// Enabling validation layers
#ifdef IMGUI_VULKAN_DEBUG_REPORT
		const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
		create_info.enabledLayerCount = 1;
		create_info.ppEnabledLayerNames = layers;
		instance_extensions.push_back("VK_EXT_debug_report");
#endif

		// Create Vulkan Instance
		create_info.enabledExtensionCount = (uint32_t)instance_extensions.Size;
		create_info.ppEnabledExtensionNames = instance_extensions.Data;
		err = vkCreateInstance(&create_info, g_Allocator, &g_Instance);
		check_vk_result(err);

		// Setup the debug report callback
#ifdef IMGUI_VULKAN_DEBUG_REPORT
		auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkCreateDebugReportCallbackEXT");
		IM_ASSERT(vkCreateDebugReportCallbackEXT != nullptr);
		VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
		debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		debug_report_ci.pfnCallback = debug_report;
		debug_report_ci.pUserData = nullptr;
		err = vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci, g_Allocator, &g_DebugReport);
		check_vk_result(err);
#endif
	}

	// Select Physical Device (GPU)
	g_PhysicalDevice = SetupVulkan_SelectPhysicalDevice();

	// Select graphics queue family
	{
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, nullptr);
		VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
		vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, queues);
		for (uint32_t i = 0; i < count; i++)
			if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				g_QueueFamily = i;
				break;
			}
		free(queues);
		IM_ASSERT(g_QueueFamily != (uint32_t)-1);
	}

	// Create Logical Device (with 1 queue)
	{
		ImVector<const char*> device_extensions;
		device_extensions.push_back("VK_KHR_swapchain");

		// Enumerate physical device extension
		uint32_t properties_count;
		ImVector<VkExtensionProperties> properties;
		vkEnumerateDeviceExtensionProperties(g_PhysicalDevice, nullptr, &properties_count, nullptr);
		properties.resize(properties_count);
		vkEnumerateDeviceExtensionProperties(g_PhysicalDevice, nullptr, &properties_count, properties.Data);
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
		if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
			device_extensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
#endif

		const float queue_priority[] = { 1.0f };
		VkDeviceQueueCreateInfo queue_info[1] = {};
		queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info[0].queueFamilyIndex = g_QueueFamily;
		queue_info[0].queueCount = 1;
		queue_info[0].pQueuePriorities = queue_priority;
		VkDeviceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
		create_info.pQueueCreateInfos = queue_info;
		create_info.enabledExtensionCount = (uint32_t)device_extensions.Size;
		create_info.ppEnabledExtensionNames = device_extensions.Data;
		err = vkCreateDevice(g_PhysicalDevice, &create_info, g_Allocator, &g_Device);
		check_vk_result(err);
		vkGetDeviceQueue(g_Device, g_QueueFamily, 0, &g_Queue);
	}

	// Create Descriptor Pool
	// The example only requires a single combined image sampler descriptor for the font image and only uses one descriptor set (for that)
	// If you wish to load e.g. additional textures you may need to alter pools sizes.
	{
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
		};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1;
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		err = vkCreateDescriptorPool(g_Device, &pool_info, g_Allocator, &g_DescriptorPool);
		check_vk_result(err);
	}
}

// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
// Your real engine/app may not use them.
static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height)
{
	wd->Surface = surface;

	// Check for WSI support
	VkBool32 res;
	vkGetPhysicalDeviceSurfaceSupportKHR(g_PhysicalDevice, g_QueueFamily, wd->Surface, &res);
	if (res != VK_TRUE)
	{
		fprintf(stderr, "Error no WSI support on physical device 0\n");
		exit(-1);
	}

	// Select Surface Format
	const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
	const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

	// Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
	VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
	VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
	wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(g_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
	//printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

	// Create SwapChain, RenderPass, Framebuffer, etc.
	IM_ASSERT(g_MinImageCount >= 2);
	ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, wd, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
}

static void CleanupVulkan()
{
	vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);

#ifdef IMGUI_VULKAN_DEBUG_REPORT
	// Remove the debug report callback
	auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkDestroyDebugReportCallbackEXT");
	vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

	vkDestroyDevice(g_Device, g_Allocator);
	vkDestroyInstance(g_Instance, g_Allocator);
}

static void CleanupVulkanWindow()
{
	ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &g_MainWindowData, g_Allocator);
}

static void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
{
	VkResult err;

	VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
	VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
	err = vkAcquireNextImageKHR(g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		g_SwapChainRebuild = true;
		return;
	}
	check_vk_result(err);

	ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
	{
		err = vkWaitForFences(g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
		check_vk_result(err);

		err = vkResetFences(g_Device, 1, &fd->Fence);
		check_vk_result(err);
	}
	{
		err = vkResetCommandPool(g_Device, fd->CommandPool, 0);
		check_vk_result(err);
		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
		check_vk_result(err);
	}
	{
		VkRenderPassBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.renderPass = wd->RenderPass;
		info.framebuffer = fd->Framebuffer;
		info.renderArea.extent.width = wd->Width;
		info.renderArea.extent.height = wd->Height;
		info.clearValueCount = 1;
		info.pClearValues = &wd->ClearValue;
		vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	// Record dear imgui primitives into command buffer
	ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

	// Submit command buffer
	vkCmdEndRenderPass(fd->CommandBuffer);
	{
		VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &image_acquired_semaphore;
		info.pWaitDstStageMask = &wait_stage;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &fd->CommandBuffer;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &render_complete_semaphore;

		err = vkEndCommandBuffer(fd->CommandBuffer);
		check_vk_result(err);
		err = vkQueueSubmit(g_Queue, 1, &info, fd->Fence);
		check_vk_result(err);
	}
}

static void FramePresent(ImGui_ImplVulkanH_Window* wd)
{
	if (g_SwapChainRebuild)
		return;
	VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
	VkPresentInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores = &render_complete_semaphore;
	info.swapchainCount = 1;
	info.pSwapchains = &wd->Swapchain;
	info.pImageIndices = &wd->FrameIndex;
	VkResult err = vkQueuePresentKHR(g_Queue, &info);
	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		g_SwapChainRebuild = true;
		return;
	}
	check_vk_result(err);
	wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
}

// A quick way to split strings separated via spaces.
std::vector<std::string> splitString(std::string s)
{
	std::stringstream ss(s);
	std::string word;
	std::vector<std::string> words;
	while (ss >> word) {
		words.push_back(word);
	}
	return words;
}

Application::Application()
	: _project()
	, _window("", 1280, 720)
	, _editorWindows()
{
	// set to default window title
	set_window_title("");

	// create all the editor windows
	// add to list so they get drawn and updated
	_editorWindows.emplace("Console", new ConsoleWindow(*this));
	_editorWindows.emplace("Hierarchy", new HierarchyWindow(*this));
	_editorWindows.emplace("Scene", new SceneWindow(*this));
	_editorWindows.emplace("Game", new GameWindow(*this));
	_editorWindows.emplace("Properties", new PropertiesWindow(*this));
	_editorWindows.emplace("Assets", new AssetsWindow(*this));

	// initialize any if needed:

}

mintye::Application::~Application()
{
	for(auto const& pair : _editorWindows)
	{
		delete pair.second;
	}
}

int Application::run(int argc, char const* argv[])
{
	BuildInfo buildInfo
	{
		.debug = true
	};

	// set error callback
	glfwSetErrorCallback(glfw_error_callback);

	ImVector<const char*> extensions;
	uint32_t extensions_count = 0;
	const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
	for (uint32_t i = 0; i < extensions_count; i++)
		extensions.push_back(glfw_extensions[i]);
	SetupVulkan(extensions);

	// Create Window Surface
	VkSurfaceKHR surface;
	VkResult err = glfwCreateWindowSurface(g_Instance, _window.get_raw(), g_Allocator, &surface);
	check_vk_result(err);

	// Create Framebuffers
	int w, h;
	glfwGetFramebufferSize(_window.get_raw(), &w, &h);
	ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
	SetupVulkanWindow(wd, surface, w, h);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForVulkan(_window.get_raw(), true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = g_Instance;
	init_info.PhysicalDevice = g_PhysicalDevice;
	init_info.Device = g_Device;
	init_info.QueueFamily = g_QueueFamily;
	init_info.Queue = g_Queue;
	init_info.PipelineCache = g_PipelineCache;
	init_info.DescriptorPool = g_DescriptorPool;
	init_info.Subpass = 0;
	init_info.MinImageCount = g_MinImageCount;
	init_info.ImageCount = wd->ImageCount;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.Allocator = g_Allocator;
	init_info.CheckVkResultFn = check_vk_result;
	ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);

	// Upload Fonts
	{
		// Use any command queue
		VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
		VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

		err = vkResetCommandPool(g_Device, command_pool, 0);
		check_vk_result(err);
		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(command_buffer, &begin_info);
		check_vk_result(err);

		ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

		VkSubmitInfo end_info = {};
		end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		end_info.commandBufferCount = 1;
		end_info.pCommandBuffers = &command_buffer;
		err = vkEndCommandBuffer(command_buffer);
		check_vk_result(err);
		err = vkQueueSubmit(g_Queue, 1, &end_info, VK_NULL_HANDLE);
		check_vk_result(err);

		err = vkDeviceWaitIdle(g_Device);
		check_vk_result(err);
		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}

	// Our state
	bool show_demo_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	_window.maximize();

	// Main loop
	while (_window.is_open())
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		Window::poll_events();

		// Resize swap chain?
		if (g_SwapChainRebuild)
		{
			int width, height;
			glfwGetFramebufferSize(_window.get_raw(), &width, &height);
			if (width > 0 && height > 0)
			{
				ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
				ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
				g_MainWindowData.FrameIndex = 0;
				g_SwapChainRebuild = false;
			}
		}

		// Start the Dear ImGui frame
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		//ImGui::ShowDemoWindow(&show_demo_window);

		draw_application(buildInfo);

		// Rendering
		ImGui::Render();
		ImDrawData* main_draw_data = ImGui::GetDrawData();
		const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
		wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
		wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
		wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
		wd->ClearValue.color.float32[3] = clear_color.w;
		if (!main_is_minimized)
			FrameRender(wd, main_draw_data);

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		// Present Main Platform Window
		if (!main_is_minimized)
			FramePresent(wd);
	}

	// Cleanup
	cleanup();

	err = vkDeviceWaitIdle(g_Device);
	check_vk_result(err);
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	CleanupVulkanWindow();
	CleanupVulkan();

	//glfwDestroyWindow(window);
	//glfwTerminate();

	return 0;
}

void mintye::Application::cleanup()
{
	unload_scene();
	unload_project();
	reset_editor_windows();
}

void mintye::Application::set_window_title(minty::String const& subTitle)
{
	if (subTitle.length())
	{
		_window.set_title("Minty Editor");
	}
	else
	{
		_window.set_title(std::format("Minty Editor: {}", subTitle));
	}
}

void mintye::Application::new_project()
{
	ImGui::OpenPopup("Create New Project");
}

void mintye::Application::open_project()
{
	IGFD::FileDialogConfig config
	{
		.path = "."
	};
	ImGuiFileDialog::Instance()->OpenDialog("open_project", "Choose project directory...", nullptr, config);
}

void mintye::Application::load_project(minty::Path const& path)
{
	// if existing project, unload it
	unload_project();

	// load the new one if path is valid
	if (!std::filesystem::exists(path))
	{
		ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");
		console->log_error(std::format("Cannot load project with path \"{}\". It does not exist.", path.string()));
		return;
	}

	_project = new Project(path);
	_project->collect_assets();
	std::filesystem::current_path(path);
	// set window text to file name
}

void mintye::Application::unload_project()
{
	if (_project)
	{
		delete _project;
		_project = nullptr;
	}
}

void mintye::Application::create_new_project(minty::String const& name, minty::Path const& path, NewProjectSetupType initType)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	// make a new directory with the name
	Path fullPath = path / name;

	// create new project directory
	if (!fs::create_directory(fullPath))
	{
		console->log_error(std::format("Failed to create project at path: {}", fullPath.string()));
		return;
	}

	// create all default project necessary directories

	if (!fs::create_directory(fullPath / ASSETS_DIRECTORY_NAME))
	{
		console->log_error(std::format("Failed to create Assets folder in project path: {}", fullPath.string()));
		return;
	}
	if (!fs::create_directory(fullPath / BUILD_DIRECTORY_NAME))
	{
		console->log_error(std::format("Failed to create Build folder in project path: {}", fullPath.string()));
		return;
	}

	// create all default project necessary files
	generate_init(fullPath);

	// init other folders/files if needed
	switch (initType)
	{
	case NewProjectSetupType::Default:
		break;
	case NewProjectSetupType::VSCode:
	{
		// create directory
		if (!fs::create_directory(fullPath / VSCODE_DIRECTORY_NAME))
		{
			console->log_error(std::format("Failed to create .vscode folder in project path: {}", fullPath.string()));
			return;
		}

		// create files
		generate_vscode();
	}
		break;
	}

	// done
	console->log(std::format("Created new project: {}", fullPath.string()));
}

void mintye::Application::load_scene(minty::String const& name)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error(std::format("Cannot load scene \"{}\". No project loaded.", name));
		return;
	}

	Path scenePath = _project->find_asset(name, Project::CommonFileType::Scene);

	// if none found, do nothing
	if (scenePath.empty())
	{
		console->log_error(std::format("Cannot load scene \"{}\". Project could not find the asset.", name));
	}

	// unload existing scene
	unload_scene();

	// load new scene

}

void mintye::Application::unload_scene()
{

}

void mintye::Application::save_project()
{
	minty::Console::todo("Application::save_project()");
}

void mintye::Application::save_as_project()
{
	minty::Console::todo("Application::save_as_project()");
}

void mintye::Application::close_project()
{
	unload_project();
}

void mintye::Application::draw_application(BuildInfo& buildInfo)
{
	draw_dock_space();
	draw_menu_bar();
	draw_commands(buildInfo);
	draw_editor_windows();
}

void mintye::Application::draw_dock_space()
{
	static bool fullscreen = true;
	static bool padding = false;
	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

	// set up full screen mode
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (fullscreen)
	{
		ImGuiViewport const* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// do not render background if pass thru
	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) windowFlags |= ImGuiWindowFlags_NoBackground;

	// add (non)padding style
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Dock Space", nullptr, windowFlags);

	// undo padding style
	ImGui::PopStyleVar();

	// remove fullscreen styles
	if (fullscreen) ImGui::PopStyleVar(2);

	// dock space
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}

	ImGui::End();
}

void mintye::Application::draw_menu_bar()
{
	// dumb work around: cannot open a popup in a menu bar
	// https://github.com/ocornut/imgui/issues/331

	bool createNewProject = false;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project"))
			{
				createNewProject = true;
			}
			if (ImGui::MenuItem("Open Project")) // "Ctrl+O"
			{
				open_project();
			}
			if (ImGui::MenuItem("Save Project", nullptr, false, false)) // "Ctrl+S", disabled for now
			{
				save_project();
			}
			if (ImGui::MenuItem("Save Project as...", nullptr, false, false)) // disabled for now
			{
				save_as_project();
			}
			if (ImGui::MenuItem("Close Project", nullptr, false, _project))
			{
				close_project();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
			{
				std::exit(0);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (createNewProject)
	{
		new_project();
	}

	static char newProjectTitle[64] = "";
	static NewProjectSetupType newProjectSetupType = NewProjectSetupType::Default;

	if (ImGui::BeginPopupModal("Create New Project"))
	{
		ImGui::Text("Create New Project");

		ImGui::InputText("Project Name", newProjectTitle, IM_ARRAYSIZE(newProjectTitle));

		static int currentItem = 0;
		char const* const items[] = { "Default", "VSCode" };
		int itemsCount = sizeof(items) / sizeof(*items);

		if (ImGui::Combo("Setup Type", &currentItem, items, itemsCount))
		{
			newProjectSetupType = static_cast<NewProjectSetupType>(currentItem);
		}

		if (ImGui::Button("Create"))
		{
			IGFD::FileDialogConfig config
			{
				.path = "."
			};
			ImGuiFileDialog::Instance()->OpenDialog("new_project", "Choose directory to create project in...", nullptr, config);

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	// draw open file dialog
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (ImGuiFileDialog::Instance()->Display("new_project"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			Path filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

			// create project
			create_new_project(newProjectTitle, filePath, newProjectSetupType);

			// load it also
			load_project(filePath / newProjectTitle);
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (ImGuiFileDialog::Instance()->Display("open_project"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			//Path filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			Path filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

			load_project(filePath);
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
}

void Application::draw_commands(BuildInfo& buildInfo)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	// begin new window
	ImGui::Begin("Commands");

	bool disabled = !_project || console->is_command_running();

	if (disabled)
	{
		ImGui::BeginDisabled();
	}

	// get debug mode
	ImGui::Checkbox("Debug", &buildInfo.debug);

	// commands:

	if (ImGui::Button("Clean"))
	{
		clean();
	}
	if (ImGui::Button("Build"))
	{
		build(buildInfo);
	}
	if (ImGui::Button("Run"))
	{
		run(buildInfo);
	}

	if (disabled)
	{
		ImGui::EndDisabled();
	}

	ImGui::End();
}

void mintye::Application::draw_editor_windows()
{
	for (auto const& pair : _editorWindows)
	{
		pair.second->draw();
	}
}

void mintye::Application::reset_editor_windows()
{
	for (auto const& pair : _editorWindows)
	{
		pair.second->reset();
	}
}

void Application::generate_cmake(BuildInfo const& buildInfo)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot generate cmake: no project loaded.");
		return;
	}

	// get path to cmake file
	std::string path = (Path(_project->get_build_path()) / "CMakeLists.txt").string();

	// open file to overwrite
	std::ofstream file(path, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		console->log_error(String("Could not open cmake file: ") + path);
		return;
	}

	// get timestamp
	const auto now = std::chrono::system_clock::now();

	// write contents
	file <<
		// show generation time
		"# " << std::format("{:%Y-%m-%d %H:%M:%OS}", now) << std::endl <<
		// cmake version requirement
		"cmake_minimum_required(VERSION 3.16)" << std::endl <<
		// project name, c++ settings
		"project(" << APPLICATION_NAME << " LANGUAGES CXX)" << std::endl <<
		"find_package(Vulkan REQUIRED)" << std::endl <<
		"set(CMAKE_CXX_STANDARD 20)" << std::endl <<
		"set(CMAKE_CXX_STANDARD_REQUIRED ON)" << std::endl <<
		"set(CMAKE_CXX_EXTENSIONS OFF)" << std::endl;

	if (buildInfo.debug)
	{
		// only ignore if in debug mode
		file << "set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\\\"LIBCMT\\\")" << std::endl;
	}

	// get all local paths for source files
	std::stringstream pathsStream;
	minty::Path buildPath = _project->get_build_path();
	for (minty::Path const& path : _project->find_assets(Project::CommonFileType::Source))
	{
		pathsStream << " " << std::filesystem::relative(path, buildPath).generic_string();
	}

	file <<
		// add source files for project
		"set(SOURCES main.cpp" << pathsStream.str() << ")" << std::endl <<
		// make executable with the source files
		"add_executable(${PROJECT_NAME} ${SOURCES})" << std::endl <<
		// set to MT building so it works with the static runtime library
		"set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)" << std::endl <<
		"set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY \"MultiThreaded$<$<CONFIG:Debug>:Debug>\")" << std::endl <<
		// include the runtime dir
		"target_include_directories(${PROJECT_NAME} PRIVATE C:/Users/mitch/source/repos/Minty-Engine/Runtime PUBLIC ${VULKAN_INCLUDE_DIRS})" << std::endl <<
		// include and link Vulkan
		"include_directories(${Vulkan_INCLUDE_DIRS})" << std::endl <<
		"target_link_libraries(${PROJECT_NAME} C:/Users/mitch/source/repos/Minty-Engine/Runtime/x64/" << buildInfo.get_config() << "/MintyRuntime.lib)" << std::endl <<
		"target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})";

	file.close();
}

void Application::generate_main()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot generate main: no project loaded.");
		return;
	}

	// get path to cmake file
	std::string path = (std::filesystem::path(_project->get_build_path()) / "main.cpp").string();

	// open file to overwrite
	std::ofstream file(path, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		minty::Console::error(std::string("Could not open main file: ") + path);
		return;
	}

	// get timestamp
	const auto now = std::chrono::system_clock::now();

	// write contents
	file <<
		"// " << std::format("{:%Y-%m-%d %H:%M:%OS}", now) << std::endl <<
		"#include <Minty.h>" << std::endl <<
		"#include <string>" << std::endl <<
		"#include <format>" << std::endl <<
		"#include <filesystem>" << std::endl <<
		"#include \"../Assets/init.h\"" << std::endl <<
		"int main(int argc, char const* argv[]) {" << std::endl <<
		"	std::filesystem::current_path(\"" << _project->get_base_path().generic_string() << "\");" << std::endl << // move out of Build/Debug or Build/Release folder, into base folder
		"	minty::Console::log(std::filesystem::current_path().string());" << std::endl <<
		"	minty::Info info(\"" << _project->get_name() << "\", 0, 0, 0);" << std::endl <<
		"	minty::Engine engine(info);" << std::endl <<
		"	engine.init();" << std::endl <<
		"	if(int code = init(engine)) { minty::Console::error(std::format(\"Failed to init program with error code {}.\", code)); return code; }" << std::endl <<
		"	engine.start();" << std::endl <<
		"	engine.run();" << std::endl <<
		"	engine.cleanup();" << std::endl <<
		"	if(int code = destroy(engine)) { minty::Console::error(std::format(\"Failed to destroy program with error code {}.\", code)); return code; }" << std::endl <<
		"	engine.destroy();" << std::endl <<
		"	return engine.get_exit_code();" << std::endl <<
		"}";

	file.close();
}

void mintye::Application::generate_vscode()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot generate vscode: no project loaded.");
		return;
	}

	// get path to files file
	Path vsCodePath = _project->get_sub_path(VSCODE_DIRECTORY_NAME);
	String cppPropertiesPath = (vsCodePath / "c_cpp_properties.json").string();
	String launchPath = (vsCodePath / "launch.json").string();
	String settingsPath = (vsCodePath / "settings.json").string();

	// open file to overwrite
	std::ofstream file(cppPropertiesPath, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		console->log_error(std::format("Could not open c_cpp_properties.json file: {}", cppPropertiesPath));
		return;
	}
	
	file
		<< "{" << std::endl
		<< "    \"configurations\": [" << std::endl
		<< "        {" << std::endl
		<< "            \"name\": \"Win32\"," << std::endl
		<< "            \"includePath\": [" << std::endl
		<< "                \"${workspaceFolder}/**\"," << std::endl
		<< "                \"C:\\\\Users\\\\mitch\\\\source\\\\repos\\\\Minty-Engine\\\\Runtime\"," << std::endl
		<< "                \"${Vulkan_INCLUDE_DIR}\"" << std::endl
		<< "            ]," << std::endl
		<< "            \"defines\": [" << std::endl
		<< "                \"_DEBUG\"," << std::endl
		<< "                \"UNICODE\"," << std::endl
		<< "                \"_UNICODE\"" << std::endl
		<< "            ]," << std::endl
		<< "            \"windowsSdkVersion\": \"10.0.22621.0\"," << std::endl
		<< "            \"compilerPath\": \"cl.exe\"," << std::endl
		<< "            \"cStandard\": \"c17\"," << std::endl
		<< "            \"cppStandard\": \"c++20\"," << std::endl
		<< "            \"intelliSenseMode\": \"windows-msvc-x64\"" << std::endl
		<< "        }" << std::endl
		<< "    ]," << std::endl
		<< "    \"version\": 4" << std::endl
		<< "}" << std::endl;

	console->log_warning("c_cpp_properties.json: hard coded cmake.sourceDirectory");

	file.close();

	file = std::ofstream(launchPath, std::ios::trunc);

	if (!file.is_open())
	{
		console->log_error(std::format("Could not open launch.json file: {}", launchPath));
		return;
	}

	file
		<< "{" << std::endl
		<< "    \"version\": \"0.2.0\"," << std::endl
		<< "    \"configurations\": [" << std::endl
		<< "        {" << std::endl
		<< "            \"name\": \"(msvc) Launch\"," << std::endl
		<< "            \"type\": \"cppvsdbg\"," << std::endl
		<< "            \"request\": \"launch\"," << std::endl
		<< "            // Resolved by CMake Tools:" << std::endl
		<< "            \"program\": \"${command:cmake.launchTargetPath}\"," << std::endl
		<< "            \"args\": []," << std::endl
		<< "            \"stopAtEntry\": false," << std::endl
		<< "            \"cwd\": \"${workspaceFolder}\"," << std::endl
		<< "            \"environment\": [" << std::endl
		<< "                {" << std::endl
		<< "                    // add the directory where our target was built to the PATHs" << std::endl
		<< "                    // it gets resolved by CMake Tools:" << std::endl
		<< "                    \"name\": \"PATH\"," << std::endl
		<< "                    \"value\": \"${env:PATH}:${command:cmake.getLaunchTargetDirectory}\"" << std::endl
		<< "                }" << std::endl
		<< "            ]," << std::endl
		<< "            \"console\": \"internalConsole\"" << std::endl
		<< "        }" << std::endl
		<< "    ]" << std::endl
		<< "}" << std::endl;

	file.close();

	file = std::ofstream(settingsPath, std::ios::trunc);

	if (!file.is_open())
	{
		console->log_error(std::format("Could not open settings.json file: {}", settingsPath));
		return;
	}

	file
		<< "{" << std::endl
		<< "    \"files.associations\": {" << std::endl
		<< "        \"ostream\": \"cpp\"," << std::endl
		<< "        \"algorithm\": \"cpp\"," << std::endl
		<< "        \"array\": \"cpp\"," << std::endl
		<< "        \"atomic\": \"cpp\"," << std::endl
		<< "        \"bit\": \"cpp\"," << std::endl
		<< "        \"cctype\": \"cpp\"," << std::endl
		<< "        \"charconv\": \"cpp\"," << std::endl
		<< "        \"chrono\": \"cpp\"," << std::endl
		<< "        \"clocale\": \"cpp\"," << std::endl
		<< "        \"cmath\": \"cpp\"," << std::endl
		<< "        \"compare\": \"cpp\"," << std::endl
		<< "        \"concepts\": \"cpp\"," << std::endl
		<< "        \"cstddef\": \"cpp\"," << std::endl
		<< "        \"cstdint\": \"cpp\"," << std::endl
		<< "        \"cstdio\": \"cpp\"," << std::endl
		<< "        \"cstdlib\": \"cpp\"," << std::endl
		<< "        \"cstring\": \"cpp\"," << std::endl
		<< "        \"ctime\": \"cpp\"," << std::endl
		<< "        \"cwchar\": \"cpp\"," << std::endl
		<< "        \"deque\": \"cpp\"," << std::endl
		<< "        \"exception\": \"cpp\"," << std::endl
		<< "        \"filesystem\": \"cpp\"," << std::endl
		<< "        \"format\": \"cpp\"," << std::endl
		<< "        \"forward_list\": \"cpp\"," << std::endl
		<< "        \"fstream\": \"cpp\"," << std::endl
		<< "        \"functional\": \"cpp\"," << std::endl
		<< "        \"initializer_list\": \"cpp\"," << std::endl
		<< "        \"iomanip\": \"cpp\"," << std::endl
		<< "        \"ios\": \"cpp\"," << std::endl
		<< "        \"iosfwd\": \"cpp\"," << std::endl
		<< "        \"iostream\": \"cpp\"," << std::endl
		<< "        \"istream\": \"cpp\"," << std::endl
		<< "        \"iterator\": \"cpp\"," << std::endl
		<< "        \"limits\": \"cpp\"," << std::endl
		<< "        \"list\": \"cpp\"," << std::endl
		<< "        \"locale\": \"cpp\"," << std::endl
		<< "        \"map\": \"cpp\"," << std::endl
		<< "        \"memory\": \"cpp\"," << std::endl
		<< "        \"mutex\": \"cpp\"," << std::endl
		<< "        \"new\": \"cpp\"," << std::endl
		<< "        \"optional\": \"cpp\"," << std::endl
		<< "        \"ratio\": \"cpp\"," << std::endl
		<< "        \"set\": \"cpp\"," << std::endl
		<< "        \"sstream\": \"cpp\"," << std::endl
		<< "        \"stdexcept\": \"cpp\"," << std::endl
		<< "        \"stop_token\": \"cpp\"," << std::endl
		<< "        \"streambuf\": \"cpp\"," << std::endl
		<< "        \"string\": \"cpp\"," << std::endl
		<< "        \"system_error\": \"cpp\"," << std::endl
		<< "        \"thread\": \"cpp\"," << std::endl
		<< "        \"tuple\": \"cpp\"," << std::endl
		<< "        \"type_traits\": \"cpp\"," << std::endl
		<< "        \"typeinfo\": \"cpp\"," << std::endl
		<< "        \"unordered_map\": \"cpp\"," << std::endl
		<< "        \"unordered_set\": \"cpp\"," << std::endl
		<< "        \"utility\": \"cpp\"," << std::endl
		<< "        \"vector\": \"cpp\"," << std::endl
		<< "        \"xfacet\": \"cpp\"," << std::endl
		<< "        \"xhash\": \"cpp\"," << std::endl
		<< "        \"xiosbase\": \"cpp\"," << std::endl
		<< "        \"xlocale\": \"cpp\"," << std::endl
		<< "        \"xlocbuf\": \"cpp\"," << std::endl
		<< "        \"xlocinfo\": \"cpp\"," << std::endl
		<< "        \"xlocmes\": \"cpp\"," << std::endl
		<< "        \"xlocmon\": \"cpp\"," << std::endl
		<< "        \"xlocnum\": \"cpp\"," << std::endl
		<< "        \"xloctime\": \"cpp\"," << std::endl
		<< "        \"xmemory\": \"cpp\"," << std::endl
		<< "        \"xstring\": \"cpp\"," << std::endl
		<< "        \"xtr1common\": \"cpp\"," << std::endl
		<< "        \"xtree\": \"cpp\"," << std::endl
		<< "        \"xutility\": \"cpp\"," << std::endl
		<< "        \"regex\": \"cpp\"" << std::endl
		<< "    }," << std::endl
		<< "    \"liveServer.settings.port\": 5501," << std::endl
		<< "    \"cmake.sourceDirectory\": \"C:/Users/mitch/source/repos/Minty-Engine/Projects/Tests/TestProject/Build\"," << std::endl
		<< "    \"cmake.loggingLevel\": \"debug\"," << std::endl
		<< "}" << std::endl;

	console->log_warning("settings.json: hard coded cmake.sourceDirectory");

	file.close();
}

void mintye::Application::generate_init(minty::Path const& path)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	// get path to files file
	Path assetsPath = path / ASSETS_DIRECTORY_NAME;
	Path hPath = assetsPath / INIT_H_NAME;
	Path cppPath = assetsPath / INIT_CPP_NAME;

	// open file to overwrite
	std::ofstream file(hPath, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		console->log_error(std::format("Could not open {} file: {}", INIT_H_NAME, hPath.string()));
		return;
	}

	file
		<< "#pragma once" << std::endl
		<< "" << std::endl
		<< "#include <Minty.h>" << std::endl
		<< "" << std::endl
		<< "int init(minty::Engine& engine);" << std::endl
		<< "int destroy(minty::Engine& engine);" << std::endl;

	file.close();

	// open file to overwrite
	file = std::ofstream(cppPath, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		console->log_error(std::format("Could not open {} file: {}", INIT_CPP_NAME, cppPath.string()));
		return;
	}

	file
		<< "#include \"init.h\"" << std::endl
		<< "" << std::endl
		<< "using namespace minty;" << std::endl
		<< "" << std::endl
		<< "int init(Engine &engine)" << std::endl
		<< "{" << std::endl
		<< "    return 0;" << std::endl
		<< "}" << std::endl
		<< "" << std::endl
		<< "int destroy(Engine& engine)" << std::endl
		<< "{" << std::endl
		<< "    return 0;" << std::endl
		<< "}" << std::endl;

	file.close();
}

void Application::clean()
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot clean: no project loaded.");
		return;
	}

	console->log_important("clean");

	// clean the build
	console->run_command("cd " + _project->get_build_path().string() + " && " + std::filesystem::absolute(CMAKE_PATH).string() + " --build . --target clean");
}

void Application::build(BuildInfo const& buildInfo)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot build: no project loaded.");
		return;
	}

	console->log_important("build");

	console->log_important("\tgenerating cmake");

	generate_cmake(buildInfo);

	console->log_important("\tgenerating main");

	generate_main();

	console->log_important("\tbuilding program");

	std::string command = "cd " + _project->get_build_path().string() + " && " + std::filesystem::absolute(CMAKE_PATH).string();
	console->run_commands({
		// // make cmake files if needed
		command + " .",
		// build program
		command + " --build . --config " + buildInfo.get_config(),
		});
}

void Application::run(BuildInfo const& buildInfo)
{
	ConsoleWindow* console = find_editor_window<ConsoleWindow>("Console");

	if (!_project)
	{
		console->log_error("Cannot run: no project loaded.");
		return;
	}

	console->log_important("run");

	// call executable, pass in project path as argument for the runtime, so it knows what to run
	console->run_command("cd " + _project->get_build_path().string() + " && cd " + buildInfo.get_config() + " && call " + EXE_NAME + " " + _project->get_base_path().string());
}