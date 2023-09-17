#pragma once
#include "M_Object.h"

#include "M_Texture.h"
#include <vulkan/vulkan.h>

namespace minty
{
	class RenderEngine;

	class Material :
		public Object
	{
	public: // TODO: TEMP PUBLIC
		VkPipelineLayout _layout;
		VkPipeline _pipeline;
		Texture* _texture;

	public:
		Material(VkPipelineLayout const& layout, VkPipeline const& pipeline);

		void dispose(RenderEngine& engine);
	};
}