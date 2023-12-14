#pragma once
#include "M_Rendering_Object.h"

#include "M_Rendering_ShaderPassBuilder.h"
#include "M_Rendering_DescriptorSet.h"
#include "M_Constants.h"
#include "M_Register.h"

#include <vulkan/vulkan.h>

namespace minty
{
	class ShaderPass
		: rendering::RenderObject
	{
	private:
		ID _shaderId;

		VkPipeline _pipeline;

		rendering::DescriptorSet _descriptorSet;

	public:
		/// <summary>
		/// Creates a new ShaderPass.
		/// </summary>
		/// <param name="builder"></param>
		/// <param name="renderer"></param>
		ShaderPass(rendering::ShaderPassBuilder const& builder, RenderEngine& renderer);

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
		rendering::DescriptorSet const& get_descriptor_set() const;

	private:
		// creates the graphics pipeline for this ShaderPass
		void create_pipeline(rendering::ShaderPassBuilder const& builder);
	};
}