#pragma once

#include "ME_Minty.h"
#include "ME_Theme.h"

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
		Theme _theme;

	public:
		ApplicationRenderEngine(Application& app, minty::Runtime& runtime);

	public:
		void init(minty::RenderEngineBuilder const& builder) override;

		void destroy() override;

	private:
		void init_theme();

	protected:
		void draw(VkCommandBuffer commandBuffer) override;

		VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats) override;

	private:
		void create_descriptor_pool();
	};
}