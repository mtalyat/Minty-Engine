#pragma once
#include "M_Object.h"

#include <vulkan/vulkan.h>

namespace minty
{
	class Renderer;
	
	class Shader :
		public Object
	{
	public: // TODO: set to private
		VkPipelineLayout _layout;
		VkPipeline _pipeline;

	public:
		Shader(VkPipelineLayout const& layout, VkPipeline const& pipeline);

		void dispose(Renderer& renderer);
	};
}

