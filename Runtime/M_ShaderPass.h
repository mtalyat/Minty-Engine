#pragma once
#include "M_RenderObject.h"

#include "M_DescriptorSet.h"

#include "M_Vulkan.h"

namespace minty
{
	struct ShaderPassBuilder;
	
	/// <summary>
	/// Holds data for a ShaderPass.
	/// </summary>
	class ShaderPass
		: public RenderObject
	{
	private:
		ID _shaderId;

		VkPipeline _pipeline;

		DescriptorSet _descriptorSet;

	public:
		/// <summary>
		/// Creates an empty ShaderPass.
		/// </summary>
		ShaderPass();

		/// <summary>
		/// Creates a new ShaderPass.
		/// </summary>
		/// <param name="builder"></param>
		/// <param name="renderer"></param>
		ShaderPass(ShaderPassBuilder const& builder, Engine& engine, ID const sceneId);

		/// <summary>
		/// Destroys all of the resources associated with this ShaderPass.
		/// </summary>
		void destroy();

		/// <summary>
		/// Gets the Shader ID for this ShaderPass.
		/// </summary>
		/// <returns></returns>
		ID get_shader_id() const;

		/// <summary>
		/// Gets the pipeline for this ShaderPass.
		/// </summary>
		/// <returns></returns>
		VkPipeline get_pipeline() const;

		/// <summary>
		/// Gets the descriptor set for this ShaderPass.
		/// </summary>
		/// <returns></returns>
		DescriptorSet const& get_descriptor_set() const;

	private:
		// creates the graphics pipeline for this ShaderPass
		void create_pipeline(ShaderPassBuilder const& builder);
	};
}