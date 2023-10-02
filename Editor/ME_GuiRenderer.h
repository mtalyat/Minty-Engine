#pragma once

#include <Minty.h>

#include "libraries/imgui/imgui_impl_vulkan.h"
#include "libraries/imgui/imgui_impl_glfw.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace mintye
{
	class GuiRenderer
		: public minty::Renderer
	{
	public:
		VkCommandPool _viewportCommandPool;
		std::vector<VkFramebuffer> _viewportFramebuffers;
		std::vector<VkCommandBuffer> _viewportCommandBuffers;
		VkPipeline _viewportPipeline;
		VkRenderPass _viewportRenderPass;
		std::vector<VkImageView> _viewportImageViews;
		std::vector<VkDeviceMemory> _viewportDstImageMemory;
		std::vector<VkImage> _viewportImages;

		VkCommandPool _guiCommandPool;
		VkDescriptorPool _guiDescriptorPool;
		VkRenderPass _guiRenderPass;
		std::vector<VkFramebuffer> _guiFramebuffers;
		std::vector<VkCommandBuffer> _guiCommandBuffers;
		VkSampler _guiSampler;

		std::vector<VkDescriptorSet> _dset;
	public:
		GuiRenderer(minty::Window* const window);

	protected:
		void init() override;

	public:
		void start() override;

		void recreate_swap_chain() override;

	private:
		void create_gui_render_pass();

		void create_gui_command_buffers();

		void create_gui_framebuffers();

		void create_gui_sampler();
	};
}