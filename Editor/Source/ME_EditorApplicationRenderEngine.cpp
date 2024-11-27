//#include "ME_EditorApplicationRenderEngine.h"
//
//#include "ME_EditorApplication.h"
//
//using namespace Minty;
//using namespace Mintye;
//
//Mintye::EditorApplicationRenderEngine::EditorApplicationRenderEngine(EditorApplication& app)
//	: Renderer()
//	, _application(&app)
//	, _descriptorPool()
//	, _clearColor()
//	, _theme()
//{ }
//
//Mintye::EditorApplicationRenderEngine::~EditorApplicationRenderEngine()
//{
//	destroy();
//}
//
//void Mintye::EditorApplicationRenderEngine::init()
//{
//	// init normal stuff
//	Renderer::init();
//	create_descriptor_pool();
//
//	// init ImGui
//
//	// Setup Dear ImGui context
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
//	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
//	//io.ConfigViewportsNoAutoMerge = true;
//	//io.ConfigViewportsNoTaskBarIcon = true;
//
//	// Setup Dear ImGui style
//	init_theme();
//
//	// Setup Platform/Renderer backends
//	ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(_window->get_native_window()), true);
//	ImGui_ImplVulkan_InitInfo initInfo = {};
//	initInfo.Instance = _vkInstance;
//	initInfo.PhysicalDevice = _physicalDevice;
//	initInfo.Device = _device;
//	//initInfo.QueueFamily = g_QueueFamily;
//	initInfo.Queue = _graphicsQueue;
//	//initInfo.PipelineCache = g_PipelineCache;
//	initInfo.DescriptorPool = _descriptorPool;
//	//initInfo.Subpass = 0;
//	initInfo.MinImageCount = MAX_FRAMES_IN_FLIGHT;
//	initInfo.ImageCount = MAX_FRAMES_IN_FLIGHT;
//	initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
//	//initInfo.Allocator = g_Allocator;
//	//initInfo.CheckVkResultFn = check_vk_result;
//	ImGui_ImplVulkan_Init(&initInfo, _renderPass);
//
//	VkCommandBuffer commandBuffer = begin_single_time_commands(_commandPool);
//	ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
//	end_single_time_commands(commandBuffer, _commandPool);
//
//	vkDeviceWaitIdle(_device);
//	ImGui_ImplVulkan_DestroyFontUploadObjects();
//
//	// Load Fonts
//	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
//	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
//	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
//	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
//	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
//	// - Read 'docs/FONTS.md' for more instructions and details.
//	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//	//io.Fonts->AddFontDefault();
//	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
//	//IM_ASSERT(font != nullptr);
//
//	//// Upload Fonts
//	//{
//	//	// Use any command queue
//	//	VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
//	//	VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;
//
//	//	err = vkResetCommandPool(g_Device, command_pool, 0);
//	//	check_vk_result(err);
//	//	VkCommandBufferBeginInfo begin_info = {};
//	//	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//	//	begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
//	//	err = vkBeginCommandBuffer(command_buffer, &begin_info);
//	//	check_vk_result(err);
//
//	//	ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
//
//	//	VkSubmitInfo end_info = {};
//	//	end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//	//	end_info.commandBufferCount = 1;
//	//	end_info.pCommandBuffers = &command_buffer;
//	//	err = vkEndCommandBuffer(command_buffer);
//	//	check_vk_result(err);
//	//	err = vkQueueSubmit(g_Queue, 1, &end_info, VK_NULL_HANDLE);
//	//	check_vk_result(err);
//
//	//	err = vkDeviceWaitIdle(g_Device);
//	//	check_vk_result(err);
//	//	ImGui_ImplVulkan_DestroyFontUploadObjects();
//	//}
//
//	//// Our state
//	//bool show_demo_window = true;
//	_clearColor = Float4(0.0f, 0.0f, 0.0f, 1.00f);
//
//	//VkSamplerCreateInfo samplerInfo{};
//	//samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
//	//samplerInfo.magFilter = VK_FILTER_LINEAR;
//	//samplerInfo.minFilter = VK_FILTER_LINEAR;
//	//samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//	//samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//	//samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//	//samplerInfo.anisotropyEnable = VK_FALSE;
//	//samplerInfo.maxAnisotropy = 1.0f;
//	//samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
//	//samplerInfo.unnormalizedCoordinates = VK_FALSE;
//	//samplerInfo.compareEnable = VK_FALSE;
//	//samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
//	//samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
//	//samplerInfo.mipLodBias = 0.0f;
//	//samplerInfo.minLod = 0.0f;
//	//samplerInfo.maxLod = 0.0f;
//
//	//if (vkCreateSampler(_device, &samplerInfo, nullptr, &_sceneSampler) != VK_SUCCESS)
//	//{
//	//	MINTY_ABORT("Failed to create scene sampler.");
//	//}
//
//	//create_image(_swapChainExtent.width, _swapChainExtent.height, _swapChainImageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, _sceneImage, _sceneImageMemory);
//	//_sceneImageView = create_image_view(_sceneImage, _swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
//	//_sceneDescriptorSet = ImGui_ImplVulkan_AddTexture(_sceneSampler, _sceneImageView, VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL);
//}
//
//void Mintye::EditorApplicationRenderEngine::destroy()
//{
//	//vkDestroyImage(_device, _sceneImage, nullptr);
//	//vkFreeMemory(_device, _sceneImageMemory, nullptr);
//
//	vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
//
//	// destroy ImGui
//	ImGui_ImplVulkan_Shutdown();
//	//ImGui_ImplGlfw_Shutdown();
//	ImGui::DestroyContext();
//
//	// destroy normal stuff
//	Renderer::destroy();
//}
//
////VkDescriptorSet Mintye::ApplicationRenderEngine::get_scene_descriptor_set() const
////{
////	return _sceneDescriptorSet;
////}
//
//void Mintye::EditorApplicationRenderEngine::init_theme()
//{
//	ImGui::StyleColorsDark();
//	//ImGui::StyleColorsLight();
//
//	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
//	ImGuiStyle& style = ImGui::GetStyle();
//	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//	{
//		style.WindowRounding = 0.0f;
//		style.Colors[GuiColorID::WindowBg].w = 1.0f;
//	}
//
//	_theme.copy(style);
//
//	// set to default colors
//	_theme[GuiColorID::Text] = Color(255, 255, 255);
//	_theme[GuiColorID::TextDisabled] = Color(200, 200, 200);
//
//	_theme[GuiColorID::WindowBg] = Color(31, 31, 31);				// window background
//	_theme[GuiColorID::ChildBg] = Color(40, 40, 40);				// child window background
//	_theme[GuiColorID::PopupBg] = Color(35, 35, 35);				// popup window background
//	_theme[GuiColorID::FrameBg] = Color(15, 15, 15);				// background of checkbox, radio button, plot, slider, text input
//	_theme[GuiColorID::FrameBgHovered] = Color(18, 18, 18);
//	_theme[GuiColorID::FrameBgActive] = Color(18, 27, 18);
//
//	_theme[GuiColorID::TitleBg] = Color(10, 10, 10);				// the color of the title bars of unfocused windows
//	_theme[GuiColorID::TitleBgActive] = Color(43, 118, 43);		// the color of the title bars of focused windows
//	//_theme[GuiColorID::TitleBgCollapsed] = Color(255, 0, 255);		// the color of the title bars of collapsed windows
//
//	_theme[GuiColorID::MenuBarBg] = Color(45, 55, 45);			// the background color of the main menu bar
//
//	_theme[GuiColorID::CheckMark] = Color(76, 154, 76);
//
//	_theme[GuiColorID::Button] = Color(55, 99, 55);
//	_theme[GuiColorID::ButtonHovered] = Color(28, 54, 28);
//	_theme[GuiColorID::ButtonActive] = Color(71, 127, 71);
//
//	_theme[GuiColorID::Header] = Color(55, 99, 55);
//	_theme[GuiColorID::HeaderHovered] = Color(38, 142, 38);
//	_theme[GuiColorID::HeaderActive] = Color(48, 118, 48);
//
//	_theme[GuiColorID::Tab] = Color(45, 83, 45);					// the color of the unselected tab in the focused window
//	_theme[GuiColorID::TabHovered] = Color(63, 154, 63);			// the color of the tab being hovered over
//	_theme[GuiColorID::TabActive] = Color(32, 83, 32);				// the color of the selected tab in the focused window
//	_theme[GuiColorID::TabUnfocused] = Color(20, 28, 20);			// the color of the unselected tab in an unfocused window
//	_theme[GuiColorID::TabUnfocusedActive] = Color(26, 66, 26);	// the color of the selected tab in an unfocused window
//
//	_theme[GuiColorID::DockingPreview] = Color(94, 208, 94, 63);	// color when previewing a location to dock the window in
//	_theme[GuiColorID::DockingEmptyBg] = Color(5, 5, 5, 0);		// color when nothing in the docked area
//
//	_theme[GuiColorID::DragDropTarget] = Color(94, 208, 94, 63);	// color when dragging and dropping something into an area
//
//	_theme.apply(style);
//}
//
//void Mintye::EditorApplicationRenderEngine::draw(VkCommandBuffer commandBuffer)
//{
//	//// Resize swap chain?
//	//if (_window->is_resized())
//	//{
//	//	//int width = _window->get_frame_width();
//	//	//int height = _window->get_frame_height();
//	//	//if (width > 0 && height > 0)
//	//	//{
//	//	//	ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
//	//	//	ImGui_ImplVulkanH_CreateOrResizeWindow(_instance, _physicalDevice, g_Device, &g_MainWindowData, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
//	//	//	g_MainWindowData.FrameIndex = 0;
//	//	//	g_SwapChainRebuild = false;
//	//	//}
//	//}
//
//	// Start the Dear ImGui frame
//	ImGui_ImplVulkan_NewFrame();
//	ImGui_ImplGlfw_NewFrame();
//	ImGui::NewFrame();
//
//	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//	//ImGui::ShowDemoWindow(&show_demo_window);
//
//	// set to scene camera
//	set_camera(Float3(0.0f, 0.0f, -10.0f), Quaternion(), Camera());
//
//	//// copy to scene image
//	//VkImageCopy copyRegion {};
//	//copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//	//copyRegion.srcSubresource.layerCount = 1;
//	//copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//	//copyRegion.dstSubresource.layerCount = 1;
//	//copyRegion.extent.width = _swapChainExtent.width;
//	//copyRegion.extent.height = _swapChainExtent.height;
//	//copyRegion.extent.depth = 1;
//
//	//vkCmdCopyImage(commandBuffer, _swapChainImages[get_frame()], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, _sceneImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
//
//	//draw application
//	_application->draw();
//
//	// draw normal stuff: will not draw if scene was closed automatically
//	Renderer::draw(commandBuffer);
//
//	// render ImGui
//	ImGui::Render();
//	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
//
//	ImGuiIO& io = ImGui::GetIO();
//
//	// Update and Render additional Platform Windows
//	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//	{
//		ImGui::UpdatePlatformWindows();
//		ImGui::RenderPlatformWindowsDefault();
//	}
//}
//
//VkSurfaceFormatKHR Mintye::EditorApplicationRenderEngine::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats)
//{
//	//return Renderer::choose_swap_surface_format(availableFormats);
//	// same as overriden function, but we use UNORM
//
//	for (const auto& availableFormat : availableFormats) {
//		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
//			return availableFormat;
//		}
//	}
//
//	return availableFormats[0];
//}
//
//void Mintye::EditorApplicationRenderEngine::create_descriptor_pool()
//{
//	std::array<VkDescriptorPoolSize, 1> poolSizes{};
//	poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//	poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//
//	VkDescriptorPoolCreateInfo poolInfo{};
//	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
//	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
//	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
//	poolInfo.pPoolSizes = poolSizes.data();
//	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//
//	if (vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS) {
//		throw std::runtime_error("failed to create descriptor pool!");
//	}
//}
