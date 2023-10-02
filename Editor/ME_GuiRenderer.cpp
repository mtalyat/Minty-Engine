#include "ME_GuiRenderer.h"

using namespace minty;

mintye::GuiRenderer::GuiRenderer(Window* const window)
	: minty::Renderer::Renderer(window)
{}

void mintye::GuiRenderer::init()
{
	Renderer::init();

	//create_command_pool(&_viewportCommandPool);
	//create_gui_sampler();
}

void mintye::GuiRenderer::start()
{
	Renderer::start();

	//ImGuiIO& io = ImGui::GetIO();
	//(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	//ImGui::StyleColorsDark();

	//// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	//ImGuiStyle& style = ImGui::GetStyle();
	//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
	//	style.WindowRounding = 0.0f;
	//	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	//}

	//{
	//	VkDescriptorPoolSize pool_sizes[] =
	//	{
	//			{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
	//			{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
	//			{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
	//			{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
	//			{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
	//			{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
	//			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
	//			{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
	//			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
	//			{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
	//			{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000} };

	//	VkDescriptorPoolCreateInfo pool_info = {};
	//	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	//	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	//	pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
	//	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	//	pool_info.pPoolSizes = pool_sizes;

	//	if (vkCreateDescriptorPool(device, &pool_info, nullptr, &_guiDescriptorPool) != VK_SUCCESS)
	//		throw std::runtime_error("Create DescriptorPool for m_ImGuiDescriptorPool failed!");
	//}

	//// Create RenderPass for m_ImGuiRenderPass
	//{
	//	create_gui_render_pass();
	//}

	//// Create CommandPool for m_ImGuiCommandPool
	//{
	//	create_command_pool(&_guiCommandPool);
	//}

	//// Create CommandBuffers for m_ImGuiCommandBuffers
	//{
	//	create_gui_command_buffers();
	//}

	//{
	//	create_gui_framebuffers();
	//}

	//ImGui_ImplGlfw_InitForVulkan(_window->getRaw(), true);
	//ImGui_ImplVulkan_InitInfo init_info = {};
	//init_info.Instance = instance;
	//init_info.PhysicalDevice = physicalDevice;
	//init_info.Device = device;
	//init_info.QueueFamily = queueFamilyIndices.graphicsFamily.value();
	//init_info.Queue = graphicsQueue;
	//init_info.PipelineCache = VK_NULL_HANDLE;
	//init_info.DescriptorPool = _guiDescriptorPool;
	//init_info.Subpass = 0;
	//init_info.MinImageCount = MAX_FRAMES_IN_FLIGHT;
	//init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
	//init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	//init_info.Allocator = nullptr;
	//init_info.CheckVkResultFn = nullptr;
	//ImGui_ImplVulkan_Init(&init_info, _guiRenderPass);

	//// Upload Fonts
	//{
	//	VkCommandBuffer commandBuffer = begin_single_time_commands(_guiCommandPool);
	//	ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
	//	end_single_time_commands(commandBuffer, _guiCommandPool);
	//	ImGui_ImplVulkan_DestroyFontUploadObjects();
	//}

	//_dset.resize(_viewportImageViews.size());
	//for (uint32_t i = 0; i < _viewportImageViews.size(); i++)
	//	_dset[i] = ImGui_ImplVulkan_AddTexture(_guiSampler, _viewportImageViews[i], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void mintye::GuiRenderer::recreate_swap_chain()
{
	Renderer::recreate_swap_chain();

	/*ImGui_ImplVulkan_SetMinImageCount(MAX_FRAMES_IN_FLIGHT);
	create_gui_render_pass();
	create_gui_framebuffers();
	create_gui_command_buffers();

	_dset.resize(_viewportImageViews.size());
	for (uint32_t i = 0; i < _viewportImageViews.size(); i++)
		_dset[i] = ImGui_ImplVulkan_AddTexture(_guiSampler, _viewportImageViews[i], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);*/
}

void mintye::GuiRenderer::create_gui_render_pass()
{
	VkAttachmentDescription attachment = {};
	attachment.format = swapChainImageFormat;
	attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference color_attachment = {};
	color_attachment.attachment = 0;
	color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0; // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.attachmentCount = 1;
	info.pAttachments = &attachment;
	info.subpassCount = 1;
	info.pSubpasses = &subpass;
	info.dependencyCount = 1;
	info.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &info, nullptr, &_guiRenderPass) != VK_SUCCESS)
		throw std::runtime_error("failed to create render pass!");
}

void mintye::GuiRenderer::create_gui_command_buffers()
{
	_guiCommandBuffers.resize(swapChainImageViews.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = _guiCommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)_guiCommandBuffers.size();

	if (vkAllocateCommandBuffers(device, &allocInfo, _guiCommandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void mintye::GuiRenderer::create_gui_framebuffers()
{
	_guiFramebuffers.resize(swapChainImageViews.size());

	VkImageView attachment[1];
	VkFramebufferCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	info.renderPass = _guiRenderPass;
	info.attachmentCount = 1;
	info.pAttachments = attachment;
	info.width = swapChainExtent.width;
	info.height = swapChainExtent.height;
	info.layers = 1;
	for (uint32_t i = 0; i < swapChainImageViews.size(); i++)
	{
		attachment[0] = swapChainImageViews[i];
		if (vkCreateFramebuffer(device, &info, nullptr, &_guiFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void mintye::GuiRenderer::create_gui_sampler()
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_FALSE;
	samplerInfo.maxAnisotropy = 1.0f;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(device, &samplerInfo, nullptr, &_guiSampler) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
}
//
//void mintye::GuiRenderer::pick_physical_device()
//{
//	uint32_t gpu_count;
//	VK_ASSERT(result, vkEnumeratePhysicalDevices(instance, &gpu_count, nullptr), "Failed to enumerate physical device count.");
//	IM_ASSERT(gpu_count > 0);
//
//	ImVector<VkPhysicalDevice> gpus;
//	gpus.resize(gpu_count);
//	err = vkEnumeratePhysicalDevices(instance, &gpu_count, gpus.Data);
//	//check_vk_result(err);
//
//	// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
//	// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
//	// dedicated GPUs) is out of scope of this sample.
//	for (VkPhysicalDevice& device : gpus)
//	{
//		VkPhysicalDeviceProperties properties;
//		vkGetPhysicalDeviceProperties(device, &properties);
//		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
//			return device;
//	}
//
//	// Use first GPU (Integrated) is a Discrete one is not available.
//	if (gpu_count > 0)
//		return gpus[0];
//	return VK_NULL_HANDLE;
//}
