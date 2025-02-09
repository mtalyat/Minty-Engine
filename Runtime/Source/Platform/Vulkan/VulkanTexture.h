#pragma once
#include "Minty/Render/Texture.h"

#include "Minty/Library/Vulkan.h"
#include "Platform/Vulkan/VulkanImage.h"

namespace Minty
{
	class VulkanTexture
		: public Texture
	{
	private:
		VkSampler m_sampler;

	public:
		VulkanTexture(TextureBuilder const& builder);
		~VulkanTexture();

	public:
		VkImageView get_view() const;

		VkSampler get_sampler() const { return m_sampler; }
	};
}