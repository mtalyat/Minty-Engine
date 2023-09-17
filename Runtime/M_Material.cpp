#include "pch.h"
#include "M_Material.h"

#include "M_RenderEngine.h"

minty::Material::Material(VkPipelineLayout const& layout, VkPipeline const& pipeline)
	: _layout(layout)
	, _pipeline(pipeline)
	, _texture()
{}

void minty::Material::dispose(RenderEngine& engine)
{
	vkDestroyPipeline(engine.device, _pipeline, nullptr);
	vkDestroyPipelineLayout(engine.device, _layout, nullptr);
}
