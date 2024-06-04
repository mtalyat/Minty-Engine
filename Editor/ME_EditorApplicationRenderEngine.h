#pragma once

#include "ME_Minty.h"
#include "ME_Theme.h"

namespace Mintye
{
	class EditorApplication;

	class EditorApplicationRenderEngine
		: public Minty::RenderEngine
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
		EditorApplicationRenderEngine(EditorApplication& app);

		~EditorApplicationRenderEngine();

	public:
		void init() override;

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

	public:
		static EditorApplicationRenderEngine& instance() { return static_cast<EditorApplicationRenderEngine&>(RenderEngine::instance()); }
	};
}