#pragma once
#include "M_Object.h"

#include "M_Color.h"
#include <vulkan/vulkan.h>

namespace minty::rendering
{
	class TextureBuilder :
		public Object
	{
	public:
		enum class PixelFormat
		{
			None = 0,
			Gray = 1,
			GrayAlpha = 2,
			RedGreenBlue = 3,
			RedGreenBlueAlpha = 4,
		};
	private:
		std::string _path;
		Color* _pixelData;
		int _width, _height;
		PixelFormat _pixelFormat;
		VkFilter _filter;
		VkSamplerAddressMode _addressMode;
		VkSamplerMipmapMode _mipmapMode;
		VkFormat _format;

	public:
		TextureBuilder(std::string const& path);

		TextureBuilder(int const width, int const height);

		TextureBuilder(int const width, int const height, Color* const pixels);

		void set_filter(VkFilter const filter);

		void set_sampler_address_mode(VkSamplerAddressMode const addressMode);

		void set_sampler_mipmap_mode(VkSamplerMipmapMode const mipmapMode);

		void set_format(VkFormat const format);

		std::string const& get_path() const;

		int get_width() const;

		int get_height() const;

		Color* get_pixel_data() const;

		byte* get_pixel_data_raw() const;

		PixelFormat get_pixel_format() const;

		VkFilter get_filter() const;

		VkSamplerAddressMode get_sampler_address_mode() const;

		VkSamplerMipmapMode get_sampler_mipmap_mode() const;

		VkFormat get_format() const;

	public:
		friend std::string to_string(TextureBuilder const& value);
	};
}

