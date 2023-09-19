#pragma once
#include "M_Object.h"

#include <vulkan/vulkan.h>

namespace minty
{
	class Renderer;

	class Texture :
		public Object
	{
	public: // TODO: TESTING, SET TO PRIVATE LATER
		VkFormat _format;
		VkImage _image;
		VkImageView _view;
		VkDeviceMemory _memory;
		VkSampler _sampler;

	public:
		Texture(VkImage image, VkFormat format, VkImageView view, VkDeviceMemory memory, VkSampler sampler);

		void dispose(Renderer& engine);
	};
}

