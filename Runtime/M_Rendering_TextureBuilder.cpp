#include "pch.h"
#include "M_Rendering_TextureBuilder.h"

using namespace minty;
using namespace minty::rendering;

minty::rendering::TextureBuilder::TextureBuilder(std::string const& path)
	: _path(path)
	, _pixelData(nullptr)
	, _width(0)
	, _height(0)
	, _pixelFormat(PixelFormat::RedGreenBlueAlpha)
	, _filter(VkFilter::VK_FILTER_LINEAR)
	, _addressMode(VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT)
	, _mipmapMode(VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR)
	, _format(VkFormat::VK_FORMAT_R8G8B8A8_SRGB)
{}

minty::rendering::TextureBuilder::TextureBuilder(int const width, int const height)
	: _path()
	, _pixelData(nullptr)
	, _width(width)
	, _height(height)
	, _pixelFormat(PixelFormat::RedGreenBlueAlpha)
	, _filter(VkFilter::VK_FILTER_LINEAR)
	, _addressMode(VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT)
	, _mipmapMode(VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR)
	, _format(VkFormat::VK_FORMAT_R8G8B8A8_SRGB)
{}

minty::rendering::TextureBuilder::TextureBuilder(int const width, int const height, Color* const pixels)
	: _path()
	, _pixelData(pixels)
	, _width(width)
	, _height(height)
	, _pixelFormat(PixelFormat::RedGreenBlueAlpha)
	, _filter(VkFilter::VK_FILTER_LINEAR)
	, _addressMode(VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT)
	, _mipmapMode(VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR)
	, _format(VkFormat::VK_FORMAT_R8G8B8A8_SRGB)
{}

void minty::rendering::TextureBuilder::set_filter(VkFilter const filter)
{
	_filter = filter;
}

void minty::rendering::TextureBuilder::set_sampler_address_mode(VkSamplerAddressMode const addressMode)
{
	_addressMode = addressMode;
}

void minty::rendering::TextureBuilder::set_sampler_mipmap_mode(VkSamplerMipmapMode const mipmapMode)
{
	_mipmapMode = mipmapMode;
}

void minty::rendering::TextureBuilder::set_format(VkFormat const format)
{
	_format = format;
}

std::string const& minty::rendering::TextureBuilder::get_path() const
{
	return _path;
}

int minty::rendering::TextureBuilder::get_width() const
{
	return _width;
}

int minty::rendering::TextureBuilder::get_height() const
{
	return _height;
}

Color* minty::rendering::TextureBuilder::get_pixel_data() const
{
	return _pixelData;
}

byte* minty::rendering::TextureBuilder::get_pixel_data_raw() const
{
	return reinterpret_cast<byte*>(_pixelData);
}

TextureBuilder::PixelFormat minty::rendering::TextureBuilder::get_pixel_format() const
{
	return _pixelFormat;
}

VkFilter minty::rendering::TextureBuilder::get_filter() const
{
	return _filter;
}

VkSamplerAddressMode minty::rendering::TextureBuilder::get_sampler_address_mode() const
{
	return _addressMode;
}

VkSamplerMipmapMode minty::rendering::TextureBuilder::get_sampler_mipmap_mode() const
{
	return _mipmapMode;
}

VkFormat minty::rendering::TextureBuilder::get_format() const
{
	return _format;
}

std::string minty::rendering::to_string(TextureBuilder const& value)
{
	return std::format("TextureBuilder()");
}
