#pragma once

#include "ME_Minty.h"

namespace mintye
{
	class Application;

	class ApplicationRenderEngine
		: public minty::RenderEngine
	{
	private:
		Application* _application;
		VkDescriptorPool _descriptorPool;
		ImVec4 _clearColor;

	public:
		ApplicationRenderEngine(Application& app);

	public:
		void init(minty::RenderEngineBuilder const& builder) override;

		void destroy() override;

	protected:
		void draw(VkCommandBuffer commandBuffer) override;

		VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats) override;

	private:
		void create_descriptor_pool();
	};
}