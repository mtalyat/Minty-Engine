#include "pch.h"
#include "M_Texture.h"

#include "M_Color.h"
#include "M_Renderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>
#include <format>

using namespace minty;

void minty::Texture::dispose(Renderer& engine)
{
    vkDestroyImageView(engine.device, _view, nullptr);
    vkDestroyImage(engine.device, _image, nullptr);
    vkFreeMemory(engine.device, _memory, nullptr);
}

Texture minty::Texture::load(std::string const& path, Renderer& engine)
{
    // get data from file: pixels, width, height, color channels
    int width, height, channels;

    // load a texture with r g b and a
    stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    // if no pixels, error
    if (!pixels)
    {
        throw std::runtime_error(std::format("Failed to load texture: {}", path));
    }

    // get size needed to store the texture
    VkDeviceSize imageSize = width * height * sizeof(color_t);

    // copy to device via a staging buffer

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingMemory;

    // create a buffer that can be used as the source of a transfer command
    // the memory can be mapped, and specify that flush is not needed (we do not need to flush to make writes)
    engine.createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

    // map memory and copy it to buffer memory

    // map memory: get pointer where we can actually copy data to
    void* data;
    vkMapMemory(engine.device, stagingMemory, 0, imageSize, 0, &data);

    // copy data over
    memcpy(data, pixels, static_cast<size_t>(imageSize));

    // no longer need access to the data
    vkUnmapMemory(engine.device, stagingMemory);

    // done with the pixels from file
    stbi_image_free(pixels);

    // create image object on cpu
    Texture texture;

    // image data
    texture._format = VK_FORMAT_R8G8B8A8_SRGB;

    // create the image on gpu
    engine.createImage(width, height, texture._format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, texture._image, texture._memory);

    // prep texture for copying
    engine.transitionImageLayout(texture._image, texture._format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    
    // copy pixel data to image
    engine.copyBufferToImage(stagingBuffer, texture._image, width, height);

    // prep texture for rendering
    engine.transitionImageLayout(texture._image, texture._format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // cleanup staging buffer, no longer needed
    vkDestroyBuffer(engine.device, stagingBuffer, nullptr);
    vkFreeMemory(engine.device, stagingMemory, nullptr);

    // create view, so the shaders can access the image data
    texture._view = engine.createImageView(texture._image, texture._format, VK_IMAGE_ASPECT_COLOR_BIT);

    return texture;
}
