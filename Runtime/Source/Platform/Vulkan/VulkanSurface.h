#pragma once

#include "Minty/Library/Vulkan.h"
#include "Minty/Render/Surface.h"

namespace Minty
{
	class VulkanSurface
		: public Surface
	{
	private:
		VkSurfaceKHR m_surface;

	public:
		VulkanSurface(const SurfaceBuilder& builder);
		~VulkanSurface();
	};
}