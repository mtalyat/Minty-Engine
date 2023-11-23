#pragma once
#include "M_Rendering_Object.h"

#include "M_Rendering_DescriptorSet.h"
#include "M_Constants.h"
#include "M_Register.h"

#include <vulkan/vulkan.h>

namespace minty
{
	namespace rendering
	{
		class ShaderPassBuilder;
	}

	class ShaderPass
		: rendering::RendererObject
	{
	private:
		ID _shaderId;

		VkPipeline _pipeline;

		rendering::DescriptorSet _descriptorSet;

	public:
		ShaderPass(rendering::ShaderPassBuilder const& builder, Renderer& renderer);

		void destroy();

		ID get_shader_id() const;

		VkPipeline get_pipeline() const;

		rendering::DescriptorSet const& get_descriptor_set() const;

	private:
		void create_pipeline(rendering::ShaderPassBuilder const& builder);
	};
}