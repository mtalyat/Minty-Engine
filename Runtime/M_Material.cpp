#include "pch.h"
#include "M_Material.h"

#include "M_Renderer.h"

minty::Material::Material(VkPipelineLayout const& layout, VkPipeline const& pipeline)
	: _layout(layout)
	, _pipeline(pipeline)
	, _textures()
{}

void minty::Material::dispose(Renderer& engine)
{
	vkDestroyPipeline(engine.device, _pipeline, nullptr);
	vkDestroyPipelineLayout(engine.device, _layout, nullptr);
}
