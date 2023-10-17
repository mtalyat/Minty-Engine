#pragma once
#include "M_Rendering_Object.h"

#include <vulkan/vulkan.h>

namespace minty
{
	namespace rendering
	{
		class TextureBuilder;
	}

	class Texture :
		public rendering::RendererObject
	{
	private:
		VkFormat _format;
		VkImage _image;
		VkImageView _view;
		VkDeviceMemory _memory;
		VkSampler _sampler;

	public:
		Texture(std::string const& path, rendering::TextureBuilder const& builder, Renderer& renderer);

		void destroy();

		VkFormat get_format() const;

		VkImage get_image() const;

		VkImageView get_image_view() const;

		VkDeviceMemory get_device_memory() const;

		VkSampler get_sampler() const;
	};
}

