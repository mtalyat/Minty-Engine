#pragma once
#include "M_Base.h"

#include "M_Color.h"
#include "vulkan.h"

namespace minty::rendering
{
	struct TextureBuilder
	{
		enum class PixelFormat
		{
			None = 0,
			Gray = 1,
			GrayAlpha = 2,
			RedGreenBlue = 3,
			RedGreenBlueAlpha = 4,
		};

		std::string name;
		std::string path;
		Color* pixelData;
		int width, height;
		PixelFormat pixelFormat;
		VkFilter filter;
		VkSamplerAddressMode addressMode;
		VkSamplerMipmapMode mipmapMode;
		VkFormat format;
	};

	std::string to_string(TextureBuilder::PixelFormat const value);
	TextureBuilder::PixelFormat from_string_texture_builder_pixel_format(std::string const& value);
}

