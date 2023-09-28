#include "pch.h"
#include "M_Shader.h"

#include "M_Renderer.h"

minty::Shader::Shader(VkPipelineLayout const& layout, VkPipeline const& pipeline)
	: layout(layout)
	, pipeline(pipeline)
{}

void minty::Shader::dispose(Renderer& renderer)
{
	vkDestroyPipeline(renderer.device, pipeline, nullptr);
	vkDestroyPipelineLayout(renderer.device, layout, nullptr);
}
