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

void basic::create_basic_cube(Mesh& mesh)
{
	// create mesh data
	const float SIZE = 0.5f;

	Vector3 leftBottomBack = { -SIZE, -SIZE, -SIZE };
	Vector3 leftBottomFront = { -SIZE, -SIZE, SIZE };
	Vector3 leftTopBack = { -SIZE, SIZE, -SIZE };
	Vector3 leftTopFront = { -SIZE, SIZE, SIZE };
	Vector3 rightBottomBack = { SIZE, -SIZE, -SIZE };
	Vector3 rightBottomFront = { SIZE, -SIZE, SIZE };
	Vector3 rightTopBack = { SIZE, SIZE, -SIZE };
	Vector3 rightTopFront = { SIZE, SIZE, SIZE };

	Vector3 up = { 0.0f, 1.0f, 0.0f };
	Vector3 down = { 0.0f, -1.0f, 0.0f };
	Vector3 left = { -1.0f, 0.0f, 0.0f };
	Vector3 right = { 1.0f, 0.0f, 0.0f };
	Vector3 forward = { 0.0f, 0.0f, 1.0f };
	Vector3 backward = { 0.0f, 0.0f, -1.0f };

	Vector2 bottomLeft = { 0.0f, 0.0f };
	Vector2 bottomRight = { 1.0f, 0.0f };
	Vector2 topLeft = { 0.0f, 1.0f };
	Vector2 topRight = { 1.0f, 1.0f };

	std::vector<Vertex3D> vertices =
	{
		// up
		{ leftTopBack, up, bottomLeft },
		{ leftTopFront, up, topLeft },
		{ rightTopFront, up, topRight },
		{ rightTopBack, up, bottomRight },

		// down
		{ rightBottomBack, down, bottomLeft },
		{ rightBottomFront, down, topLeft },
		{ leftBottomFront, down, topRight },
		{ leftBottomBack, down, bottomRight },

		// right
		{ rightBottomBack, right, bottomLeft },
		{ rightTopBack, right, topLeft },
		{ rightTopFront, right, topRight },
		{ rightBottomFront, right, bottomRight },

		// left
		{ leftBottomFront, left, bottomLeft },
		{ leftTopFront, left, topLeft },
		{ leftTopBack, left, topRight },
		{ leftBottomBack, left, bottomRight },

		// front
		{ rightBottomFront, forward, bottomLeft },
		{ rightTopFront, forward, topLeft },
		{ leftTopFront, forward, topRight },
		{ leftBottomFront, forward, bottomRight },

		// back
		{ leftBottomBack, backward, bottomLeft },
		{ leftTopBack, backward, topLeft },
		{ rightTopBack, backward, topRight },
		{ rightBottomBack, backward, bottomRight },
	};

	std::vector<uint16_t> indices =
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
	};

	// set mesh data
	mesh.set_vertices(vertices);
	mesh.set_indices(indices);
}

void basic::create_basic_quad(Mesh& mesh)
{
	// create mesh data
	const float SIZE = 0.5f;

	Vector3 leftTopBack = { -SIZE, 0.0f, -SIZE };
	Vector3 leftTopFront = { -SIZE, 0.0f, SIZE };
	Vector3 rightTopBack = { SIZE, 0.0f, -SIZE };
	Vector3 rightTopFront = { SIZE, 0.0f, SIZE };

	Vector3 color = { 1.0f, 1.0f, 1.0f };

	Vector2 topLeft = { 0.0f, 0.0f };
	Vector2 topRight = { 1.0f, 0.0f };
	Vector2 bottomLeft = { 0.0f, 1.0f };
	Vector2 bottomRight = { 1.0f, 1.0f };

	std::vector<Vertex3D> vertices =
	{
		// up?
		{ leftTopBack, color, bottomLeft },
		{ leftTopFront, color, topLeft },
		{ rightTopFront, color, topRight },
		{ rightTopBack, color, bottomRight },
	};

	std::vector<uint16_t> indices =
	{
		0, 1, 2, 0, 2, 3
	};

	// set mesh data
	mesh.set_vertices(vertices);
	mesh.set_indices(indices);
}
