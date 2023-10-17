#include "pch.h"
#include "M_Rendering_TextureBuilder.h"

using namespace minty::rendering;

minty::rendering::TextureBuilder::TextureBuilder()
	: _filter(VkFilter::VK_FILTER_LINEAR)
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
