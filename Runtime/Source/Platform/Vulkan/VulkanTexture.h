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
		Owner<VulkanImage> m_image;
		VkSampler m_sampler;

	public:
		VulkanTexture(const TextureBuilder& builder);
		~VulkanTexture();

	public:
		Ref<Image> get_image() const override { return m_image.create_ref(); }

		VkImageView get_view() const { return m_image->get_view(); }

		VkSampler get_sampler() const { return m_sampler; }
	};
}