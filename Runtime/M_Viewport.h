#pragma once
#include "M_Object.h"

#include <vulkan/vulkan.h>

namespace minty
{
	class Viewport :
		public Object
	{
	public: // TODO: make private later
		VkViewport _viewport;
		VkRect2D _scissor;

	public:
		Viewport(int const x, int const y, unsigned int const width, unsigned int const height, float const minDepth, float const maxDepth);

		void setOffset(int const x, int const y);

		void setViewOffset(int const x, int const y);

		void setScissorOffset(int const x, int const y);

		void setExtent(unsigned int const width, unsigned int const height);

		void setViewExtent(unsigned int const width, unsigned int const height);

		void setScissorExtent(unsigned int const width, unsigned int const height);
	};
}

