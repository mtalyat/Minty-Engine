#pragma once
#include "M_Object.h"

#include <vulkan/vulkan.h>

namespace minty
{
	class Renderer;
	
	/// <summary>
	/// Holds data for a Shader.
	/// </summary>
	struct Shader :
		public Object
	{
		/// <summary>
		/// The PipelineLayout for this Shader.
		/// </summary>
		VkPipelineLayout layout;

		/// <summary>
		/// The Pipeline for this Shader.
		/// </summary>
		VkPipeline pipeline;

	public:
		/// <summary>
		/// Creates a new Shader with the given layout and pipeline.
		/// </summary>
		/// <param name="layout"></param>
		/// <param name="pipeline"></param>
		Shader(VkPipelineLayout const& layout, VkPipeline const& pipeline);

		/// <summary>
		/// Disposes of any resources this Shader is using.
		/// </summary>
		/// <param name="renderer"></param>
		void dispose(Renderer& renderer);
	};
}

