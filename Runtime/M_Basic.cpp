#include "pch.h"
#include "M_Basic.h"

#include "M_Basic_Vertex.h"
#include "M_Mesh.h"
#include "M_Rendering_DrawCallObjectInfo.h"
#include "M_Rendering_RendererBuilder.h"
#include "M_Rendering_ShaderBuilder.h"
#include "M_Rendering_ShaderPassBuilder.h"

using namespace minty;
using namespace minty::rendering;
using namespace minty::basic;

void minty::basic::create_basic_renderer_builder(minty::rendering::RendererBuilder& builder)
{

}

void minty::basic::create_basic_shader_pass_builder_3d(minty::rendering::RendererBuilder const& rendererBuilder, minty::rendering::ShaderPassBuilder& shaderPassBuilder)
{
	// add vertex data
	shaderPassBuilder.emplace_vertex_binding(0, sizeof(Vertex3D));
	shaderPassBuilder.emplace_vertex_attribute(0, 0, sizeof(Vector3), offsetof(Vertex3D, pos), VkFormat::VK_FORMAT_R32G32B32_SFLOAT);
	shaderPassBuilder.emplace_vertex_attribute(0, 1, sizeof(Vector3), offsetof(Vertex3D, normal), VkFormat::VK_FORMAT_R32G32B32_SFLOAT);
	shaderPassBuilder.emplace_vertex_attribute(0, 2, sizeof(Vector2), offsetof(Vertex3D, coord), VkFormat::VK_FORMAT_R32G32_SFLOAT);
}

void minty::basic::create_basic_shader_builder_3d(RendererBuilder const& rendererBuilder, ShaderBuilder& shaderBuilder)
{
	// add uniform data that should be part of every shader
	shaderBuilder.emplace_uniform_constant(UniformConstantInfo("camera", VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, sizeof(CameraBufferObject), DESCRIPTOR_SET_SHADER, 0));

	// add push constant that should be part of every shader
	shaderBuilder.emplace_push_constant(PushConstantInfo("object", VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(DrawCallObject3D)));
}

void minty::basic::create_basic_shader_builder_ui(RendererBuilder const& rendererBuilder, ShaderBuilder& shaderBuilder)
{
	// no vertex data for UI
	
	// add push constant that should be part of every shader
	shaderBuilder.emplace_push_constant(PushConstantInfo("object", VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(DrawCallObjectUI)));
}

void minty::basic::create_basic_shader_pass_builder_ui(minty::rendering::RendererBuilder const& rendererBuilder, minty::rendering::ShaderPassBuilder& shaderPassBuilder)
{
}