#pragma once
#include "M_Object.h"

#include "M_Texture.h"
#include <vulkan/vulkan.h>
#include <vector>

namespace minty
{
	class Renderer;

	class Material :
		public Object
	{
	public: // TODO: TEMP PUBLIC
		VkPipelineLayout _layout;
		VkPipeline _pipeline;
		std::vector<Texture> _textures;

	public:
		Material(VkPipelineLayout const& layout, VkPipeline const& pipeline);

		void dispose(Renderer& engine);
	};
}