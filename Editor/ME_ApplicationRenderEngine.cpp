#include "ME_ApplicationRenderEngine.h"

#include "ME_Application.h"

using namespace minty;
using namespace mintye;

mintye::ApplicationRenderEngine::ApplicationRenderEngine(Application& app, Runtime& runtime)
	: RenderEngine(runtime)
	, _application(&app)
	, _descriptorPool()
	, _clearColor()
{}

void mintye::ApplicationRenderEngine::init(RenderEngineBuilder const& builder)
{
	// init normal stuff
	RenderEngine::init(builder);
	create_descriptor_pool();

	// init ImGui

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
	ImGui_ImplGlfw_InitForVulkan(_window->get_raw(), true);
	ImGui_ImplVulkan_InitInfo initInfo = {};
	initInfo.Instance = _instance;
	initInfo.PhysicalDevice = _physicalDevice;
	initInfo.Device = _device;
	//initInfo.QueueFamily = g_QueueFamily;
	initInfo.Queue = _graphicsQueue;
	//initInfo.PipelineCache = g_PipelineCache;
	initInfo.DescriptorPool = _descriptorPool;
	//initInfo.Subpass = 0;
	initInfo.MinImageCount = MAX_FRAMES_IN_FLIGHT;
	initInfo.ImageCount = MAX_FRAMES_IN_FLIGHT;
	initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	//initInfo.Allocator = g_Allocator;
	//initInfo.CheckVkResultFn = check_vk_result;
	ImGui_ImplVulkan_Init(&initInfo, _renderPass);

	VkCommandBuffer commandBuffer = begin_single_time_commands(_commandPool);
	ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
	end_single_time_commands(commandBuffer, _commandPool);

	vkDeviceWaitIdle(_device);
	ImGui_ImplVulkan_DestroyFontUploadObjects();

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

	//// Upload Fonts
	//{
	//	// Use any command queue
	//	VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
	//	VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

	//	err = vkResetCommandPool(g_Device, command_pool, 0);
	//	check_vk_result(err);
	//	VkCommandBufferBeginInfo begin_info = {};
	//	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//	begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	//	err = vkBeginCommandBuffer(command_buffer, &begin_info);
	//	check_vk_result(err);

	//	ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

	//	VkSubmitInfo end_info = {};
	//	end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//	end_info.commandBufferCount = 1;
	//	end_info.pCommandBuffers = &command_buffer;
	//	err = vkEndCommandBuffer(command_buffer);
	//	check_vk_result(err);
	//	err = vkQueueSubmit(g_Queue, 1, &end_info, VK_NULL_HANDLE);
	//	check_vk_result(err);

	//	err = vkDeviceWaitIdle(g_Device);
	//	check_vk_result(err);
	//	ImGui_ImplVulkan_DestroyFontUploadObjects();
	//}

	//// Our state
	//bool show_demo_window = true;
	_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

void mintye::ApplicationRenderEngine::destroy()
{
	// destroy ImGui
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// destroy normal stuff
	RenderEngine::destroy();
}

void mintye::ApplicationRenderEngine::draw(VkCommandBuffer commandBuffer)
{
	//// Resize swap chain?
	//if (_window->is_resized())
	//{
	//	//int width = _window->get_frame_width();
	//	//int height = _window->get_frame_height();
	//	//if (width > 0 && height > 0)
	//	//{
	//	//	ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
	//	//	ImGui_ImplVulkanH_CreateOrResizeWindow(_instance, _physicalDevice, g_Device, &g_MainWindowData, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
	//	//	g_MainWindowData.FrameIndex = 0;
	//	//	g_SwapChainRebuild = false;
	//	//}
	//}

	// Start the Dear ImGui frame
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//ImGui::ShowDemoWindow(&show_demo_window);

	// draw normal stuff
	RenderEngine::draw(commandBuffer);

	// draw application
	_application->draw();

	// render ImGui
	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

	ImGuiIO& io = ImGui::GetIO();

	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

VkSurfaceFormatKHR mintye::ApplicationRenderEngine::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	// same as overriden function, but we use UNORM

	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

void mintye::ApplicationRenderEngine::create_descriptor_pool()
{
	std::array<VkDescriptorPoolSize, 1> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}
