#pragma once

#include "ME_Minty.h"
#include "ME_Theme.h"

namespace mintye
{
	class EditorApplication;

	class EditorApplicationRenderEngine
		: public minty::RenderEngine
	{
	private:
		EditorApplication* _application;
		VkDescriptorPool _descriptorPool;
		ImVec4 _clearColor;
		Theme _theme;

		//VkImage _sceneImage;
		//VkImageView _sceneImageView;
		//VkDeviceMemory _sceneImageMemory;
		//VkSampler _sceneSampler;
		//VkDescriptorSet _sceneDescriptorSet;

	public:
		EditorApplicationRenderEngine(EditorApplication& app, minty::Runtime& runtime);

		~EditorApplicationRenderEngine();

	public:
		void init(minty::RenderEngineBuilder const& builder) override;

		void destroy() override;

	public:
		//VkDescriptorSet get_scene_descriptor_set() const;

	private:
		void init_theme();

	protected:
		void draw(VkCommandBuffer commandBuffer) override;

		VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats) override;

	private:
		void create_descriptor_pool();
	};
}