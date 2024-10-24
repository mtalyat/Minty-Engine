#pragma once
#include "Minty/Library/Vulkan.h"

#include "Minty/Render/Material.h"
#include "Platform/Vulkan/VulkanBuffer.h"

namespace Minty
{
	class VulkanMaterial
		: public Material
	{
	private:
		struct Frame
		{
			std::vector<Owner<VulkanBuffer>> buffers;
			VkDescriptorSet descriptorSet;
		};

		std::vector<Frame> m_frames;

	public:
		VulkanMaterial(MaterialBuilder const& builder);

		~VulkanMaterial() = default;

	public:
		void on_bind() override;

		void set_input(String const& name, void const* const data) override;
	};
}