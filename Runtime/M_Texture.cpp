#include "pch.h"
#include "M_Texture.h"

#include "M_Renderer.h"
#include <stdexcept>
#include <format>

using namespace minty;

minty::Texture::Texture(VkImage image, VkFormat format, VkImageView view, VkDeviceMemory memory, VkSampler sampler)
    : _image(image)
    , _format(format)
    , _view(view)
    , _memory(memory)
    , _sampler(sampler)
{
	
}

void minty::Texture::dispose(Renderer& engine)
{
    vkDestroySampler(engine.device, _sampler, nullptr);
    vkDestroyImageView(engine.device, _view, nullptr);
    vkDestroyImage(engine.device, _image, nullptr);
    vkFreeMemory(engine.device, _memory, nullptr);
}
