#include "pch.h"
#include "M_Texture.h"

#include "M_Renderer.h"
#include "M_Rendering_TextureBuilder.h"
#include "M_File.h"
#include "M_Error.h"
#include <format>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace minty;
using namespace minty::rendering;

Texture::Texture(std::string const& path, rendering::TextureBuilder const& builder, Renderer& renderer)
	: RendererObject::RendererObject(renderer)
{
	// get data from file: pixels, width, height, color channels
	int channels;

	// load a texture with r g b and a
	stbi_uc* pixels = stbi_load(path.c_str(), &_width, &_height, &channels, STBI_rgb_alpha);

	// if no pixels, error
	if (!pixels)
	{
		console::error(std::format("Failed to load texture: {}", path));
		return;
	}

	// get size needed to store the texture
	VkDeviceSize imageSize = _width * _height * sizeof(color_t);

	// copy to device via a staging buffer

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory;

	// create a buffer that can be used as the source of a transfer command
	// the memory can be mapped, and specify that flush is not needed (we do not need to flush to make writes)
	renderer.create_buffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

	VkDevice device = renderer.get_device();

	// map memory and copy it to buffer memory

	// map memory: get pointer where we can actually copy data to
	void* data;
	vkMapMemory(device, stagingMemory, 0, imageSize, 0, &data);

	// copy data over
	memcpy(data, pixels, static_cast<size_t>(imageSize));

	// no longer need access to the data
	vkUnmapMemory(device, stagingMemory);

	// done with the pixels from file
	stbi_image_free(pixels);

	// image data
	_format = static_cast<VkFormat>(builder.get_format());

	// create the image on gpu
	renderer.create_image(_width, _height, _format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _image, _memory);

	// prep texture for copying
	renderer.change_image_layout(_image, _format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	// copy pixel data to image
	renderer.copy_buffer_to_image(stagingBuffer, _image, _width, _height);

	// prep texture for rendering
	renderer.change_image_layout(_image, _format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	// cleanup staging buffer, no longer needed
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingMemory, nullptr);

	// create view, so the shaders can access the image data
	_view = renderer.create_image_view(_image, _format, VK_IMAGE_ASPECT_COLOR_BIT);

	// create sampler
	// create the sampler in constructor for now
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = static_cast<VkFilter>(builder.get_filter());
	samplerInfo.minFilter = static_cast<VkFilter>(builder.get_filter());
	//samplerInfo.magFilter = VK_FILTER_LINEAR;
	//samplerInfo.minFilter = VK_FILTER_LINEAR;

	// how to draw if size is too small or big, etc.
	samplerInfo.addressModeU = static_cast<VkSamplerAddressMode>(builder.get_sampler_address_mode());
	samplerInfo.addressModeV = static_cast<VkSamplerAddressMode>(builder.get_sampler_address_mode());
	samplerInfo.addressModeW = static_cast<VkSamplerAddressMode>(builder.get_sampler_address_mode());

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(renderer.get_physical_device(), &properties);

	// how sharp the sampling can get
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

	// color if out of bounds
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

	// depends on how to access bytes. false = normalized between 0 and 1
	samplerInfo.unnormalizedCoordinates = VK_FALSE;

	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	// mip maps
	samplerInfo.mipmapMode = static_cast<VkSamplerMipmapMode>(builder.get_sampler_mipmap_mode());
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(device, &samplerInfo, nullptr, &_sampler) != VK_SUCCESS) {
		error::abort(std::format("Failed to load texture: {}", path));
	}
}

void minty::Texture::destroy()
{
	auto device = _renderer.get_device();

	vkDestroySampler(device, _sampler, nullptr);
	vkDestroyImageView(device, _view, nullptr);
	vkDestroyImage(device, _image, nullptr);
	vkFreeMemory(device, _memory, nullptr);
}

int minty::Texture::get_width() const
{
	return _width;
}

int minty::Texture::get_height() const
{
	return _height;
}

VkFormat minty::Texture::get_format() const
{
	return _format;
}

VkImage minty::Texture::get_image() const
{
	return _image;
}

VkImageView minty::Texture::get_image_view() const
{
	return _view;
}

VkDeviceMemory minty::Texture::get_device_memory() const
{
	return _memory;
}

VkSampler minty::Texture::get_sampler() const
{
	return _sampler;
}

std::string minty::to_string(Texture const& value)
{
	return std::format("Texture(width = {}, height = {})", value._width, value._height);
}
