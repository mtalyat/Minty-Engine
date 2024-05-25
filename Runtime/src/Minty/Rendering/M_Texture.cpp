#include "pch.h"
#include "Minty/Rendering/M_Texture.h"

#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Assets/M_Asset.h"
#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Files/M_File.h"
#include "Minty/Tools/M_Console.h"
#include <format>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace Minty;
using namespace Minty;

Texture::Texture(TextureBuilder const& builder)
	: Asset(builder.id, builder.path)
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
		if (builder.pixelFormat == PixelFormat::None)
		{
			MINTY_ERROR("Attempting to load_animation texture with a pixelFormat of None.");
			return;
		}

		// get data from file: pixels, width, height, color channels
		int channels;

		AssetEngine& assets = AssetEngine::instance();
		std::vector<Byte> fileData = assets.read_file_bytes(path);
		//pixels = stbi_load(absPath.c_str(), &_width, &_height, &channels, static_cast<int>(builder.pixelFormat));
		pixels = stbi_load_from_memory(fileData.data(), static_cast<int>(fileData.size()), &_width, &_height, &channels, static_cast<int>(builder.pixelFormat));

		// if no pixels, error
		if (!pixels)
		{
			MINTY_ERROR_FORMAT("Failed to load_animation texture: {}", path.string());
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
		MINTY_ERROR("Failed to create texture. Pixels are null.");
		return;
	}

	// get size needed to store the texture
	VkDeviceSize imageSize = _width * _height * sizeof(Color::color_t);

	RenderEngine& renderEngine = RenderEngine::instance();

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
	renderEngine.copy_buffer_to_image(stagingBuffer.get_buffer(), _image, _width, _height);

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

Minty::Texture::~Texture()
{
	destroy();
}

void Minty::Texture::destroy()
{
	if (!_width && !_height) return; // already destroyed

	RenderEngine& renderer = RenderEngine::instance();

	VkDevice device = renderer.get_device();

	vkDestroySampler(device, _sampler, nullptr);
	vkDestroyImageView(device, _view, nullptr);
	vkDestroyImage(device, _image, nullptr);
	vkFreeMemory(device, _memory, nullptr);

	_width = 0;
	_height = 0;
	_format = VK_FORMAT_UNDEFINED;
	_sampler = VK_NULL_HANDLE;
	_view = VK_NULL_HANDLE;
	_image = VK_NULL_HANDLE;
	_memory = VK_NULL_HANDLE;
}