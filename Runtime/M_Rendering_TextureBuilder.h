#pragma once
#include "M_Object.h"

#include <vulkan/vulkan.h>

namespace minty::rendering
{
	class TextureBuilder :
		public Object
	{
	private:
		VkFilter _filter;
		VkSamplerAddressMode _addressMode;
		VkSamplerMipmapMode _mipmapMode;
		VkFormat _format;

	public:
		TextureBuilder();

		void set_filter(VkFilter const filter);

		void set_sampler_address_mode(VkSamplerAddressMode const addressMode);

		void set_sampler_mipmap_mode(VkSamplerMipmapMode const mipmapMode);

		void set_format(VkFormat const format);

		VkFilter get_filter() const;

		VkSamplerAddressMode get_sampler_address_mode() const;

		VkSamplerMipmapMode get_sampler_mipmap_mode() const;

		VkFormat get_format() const;
	};
}

