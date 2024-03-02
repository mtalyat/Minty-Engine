#include "pch.h"
#include "M_Texture.h"

#include "M_Runtime.h"
#include "M_RenderEngine.h"
#include "M_Asset.h"
#include "M_File.h"
#include "M_Console.h"
#include <format>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace minty;
using namespace minty;

minty::Texture::Texture()
	: Asset()
	, _width()
	, _height()
	, _format()
	, _image()
	, _view()
	, _memory()
	, _sampler()
{}

Texture::Texture(TextureBuilder const& builder, Runtime& engine)
	: Asset(builder.id, builder.path, engine)
	, _width(builder.width)
	, _height(builder.height)
	, _format()
	, _image()
	, _view()
	, _memory()
	, _sampler()
{
	// determine how to load data
	stbi_uc* pixels = reinterpret_cast<stbi_uc*>(builder.pixelData);
	bool allocated = false;

	Path const& path = builder.path;
	bool fromFilePathSize = path.string().size();

	if (fromFilePathSize)
	{
		if (!Asset::exists(path))
		{
			Console::error(std::format("Cannot load_animation texture. File not found at: {}", path.string()));
			return;
		}

		if (builder.pixelFormat == PixelFormat::None)
		{
			Console::error("Attempting to load_animation texture with a pixelFormat of None.");
			return;
		}

		// get data from file: pixels, width, height, color channels
		int channels;

		String absPath = Asset::absolute(path).string();
		pixels = stbi_load(absPath.c_str(), &_width, &_height, &channels, static_cast<int>(builder.pixelFormat));

		// if no pixels, error
		if (!pixels)
		{
			Console::error(std::format("Failed to load_animation texture: {}", path.string()));
			return;
		}
	}
	else
	{
		// get data from builder
		_width = builder.width;
		_height = builder.height;

		// create color data if needed
		if (!pixels && _width * _height > 0)
		{
			size_t size = _width * _height * 4;
			pixels = new stbi_uc[size];
			std::memset(pixels, 0, size);

			allocated = true;
		}
	}

	if (!pixels)
	{
		Console::error("Failed to create texture. Pixels are null.");
		return;
	}

	// get size needed to store the texture
	VkDeviceSize imageSize = _width * _height * sizeof(Color::color_t);

	RenderEngine& renderEngine = get_runtime().get_render_engine();

	// copy to device via a staging buffer

	// create a buffer that can be used as the source of a transfer command
	// the memory can be mapped, and specify that flush is not needed (we do not need to flush to make writes)
	Buffer const& stagingBuffer = renderEngine.create_buffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VkDevice device = renderEngine.get_device();

	// map memory and copy it to buffer memory

	void* mappedData = renderEngine.map_buffer(stagingBuffer);
	memcpy(mappedData, pixels, static_cast<size_t>(imageSize));
	renderEngine.unmap_buffer(stagingBuffer);

	// done with the pixels from file
	if (fromFilePathSize)
	{
		stbi_image_free(pixels);
	}
	else if (allocated)
	{
		delete[] pixels;
	}

	// image data
	_format = static_cast<VkFormat>(builder.format);

	// create the image on gpu
	renderEngine.create_image(_width, _height, _format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _image, _memory);

	// prep texture for copying
	renderEngine.change_image_layout(_image, _format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	// copy pixel data to image
	renderEngine.copy_buffer_to_image(stagingBuffer.buffer, _image, _width, _height);

	// prep texture for rendering
	renderEngine.change_image_layout(_image, _format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	// cleanup staging buffer, no longer needed
	renderEngine.destroy_buffer(stagingBuffer);

	// create view, so the shaders can access the image data
	_view = renderEngine.create_image_view(_image, _format, VK_IMAGE_ASPECT_COLOR_BIT);

	// create sampler
	// create the sampler in constructor for now
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = static_cast<VkFilter>(builder.filter);
	samplerInfo.minFilter = static_cast<VkFilter>(builder.filter);

	// how to draw if size is too small or big, etc.
	samplerInfo.addressModeU = static_cast<VkSamplerAddressMode>(builder.addressMode);
	samplerInfo.addressModeV = static_cast<VkSamplerAddressMode>(builder.addressMode);
	samplerInfo.addressModeW = static_cast<VkSamplerAddressMode>(builder.addressMode);

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(renderEngine.get_physical_device(), &properties);

	// how sharp the sampling can get
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

	// color if out of bounds
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

	// depends on how to access bytes. false = normalized between 0 and 1
	samplerInfo.unnormalizedCoordinates = VK_FALSE;

	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	// mip maps
	samplerInfo.mipmapMode = static_cast<VkSamplerMipmapMode>(builder.mipmapMode);
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(device, &samplerInfo, nullptr, &_sampler) != VK_SUCCESS) {
		MINTY_ABORT(std::format("Failed to load_animation texture: {}", path.string()));
	}
}

void minty::Texture::destroy()
{
	RenderEngine& renderer = get_runtime().get_render_engine();

	auto device = renderer.get_device();

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

String minty::to_string(Texture const& value)
{
	return std::format("Texture(width = {}, height = {})", value._width, value._height);
}
