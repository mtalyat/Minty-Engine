#include "pch.h"
#include "M_Basic.h"

#include "M_Basic_Vertex.h"
#include "M_Mesh.h"
#include "M_Rendering_DrawCallObjectInfo.h"
#include "M_Rendering_RendererBuilder.h"
#include "M_Rendering_ShaderBuilder.h"

using namespace minty;
using namespace minty::rendering;
using namespace minty::basic;

void minty::basic::create_basic_renderer_builder(minty::rendering::RendererBuilder& builder)
{
	builder.set_max_textures(64);
	builder.set_max_shaders(64);
	builder.set_max_materials(64);
}

void minty::basic::create_basic_shader_builder(minty::rendering::RendererBuilder const& rendererBuilder, minty::rendering::ShaderBuilder& builder)
{
	// add vertex data
	builder.emplace_vertex_binding(0, sizeof(Vertex));
	builder.emplace_vertex_attribute(0, sizeof(glm::vec3), VkFormat::VK_FORMAT_R32G32B32_SFLOAT);
	builder.emplace_vertex_attribute(0, sizeof(glm::vec3), VkFormat::VK_FORMAT_R32G32B32_SFLOAT);
	builder.emplace_vertex_attribute(0, sizeof(glm::vec2), VkFormat::VK_FORMAT_R32G32_SFLOAT);

	// add uniform data that should be part of every shader
	builder.emplace_uniform_constant(
		"camera",
		VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
		0, 0,
		VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		sizeof(CameraBufferObject),
		1
	);
	builder.emplace_uniform_constant(
		"texSamplers",
		VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT,
		0, 1,
		VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		sizeof(VkSampler),
		rendererBuilder.get_max_textures()
	);

	// add push constant that should be part of every shader
	builder.emplace_push_constant(
		"object",
		sizeof(DrawCallObjectInfo),
		VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT
	);
}

void basic::create_basic_cube(Mesh& mesh)
{
	// create mesh data
	const float SIZE = 0.5f;

	glm::vec3 leftBottomBack = { -SIZE, -SIZE, -SIZE };
	glm::vec3 leftBottomFront = { -SIZE, -SIZE, SIZE };
	glm::vec3 leftTopBack = { -SIZE, SIZE, -SIZE };
	glm::vec3 leftTopFront = { -SIZE, SIZE, SIZE };
	glm::vec3 rightBottomBack = { SIZE, -SIZE, -SIZE };
	glm::vec3 rightBottomFront = { SIZE, -SIZE, SIZE };
	glm::vec3 rightTopBack = { SIZE, SIZE, -SIZE };
	glm::vec3 rightTopFront = { SIZE, SIZE, SIZE };

	glm::vec3 color = { 1.0f, 1.0f, 1.0f };

	glm::vec2 topLeft = { 0.0f, 0.0f };
	glm::vec2 topRight = { 1.0f, 0.0f };
	glm::vec2 bottomLeft = { 0.0f, 1.0f };
	glm::vec2 bottomRight = { 1.0f, 1.0f };

	std::vector<Vertex> vertices =
	{
		// up?
		{ leftTopBack, color, bottomLeft },
		{ leftTopFront, color, topLeft },
		{ rightTopFront, color, topRight },
		{ rightTopBack, color, bottomRight },

		// down?
		{ rightBottomBack, color, bottomLeft },
		{ rightBottomFront, color, topLeft },
		{ leftBottomFront, color, topRight },
		{ leftBottomBack, color, bottomRight },

		// right?
		{ rightBottomBack, color, bottomLeft },
		{ rightTopBack, color, topLeft },
		{ rightTopFront, color, topRight },
		{ rightBottomFront, color, bottomRight },

		// left?
		{ leftBottomFront, color, bottomLeft },
		{ leftTopFront, color, topLeft },
		{ leftTopBack, color, topRight },
		{ leftBottomBack, color, bottomRight },

		// front?
		{ rightBottomFront, color, bottomLeft },
		{ rightTopFront, color, topLeft },
		{ leftTopFront, color, topRight },
		{ leftBottomFront, color, bottomRight },

		// back?
		{ leftBottomBack, color, bottomLeft },
		{ leftTopBack, color, topLeft },
		{ rightTopBack, color, topRight },
		{ rightBottomBack, color, bottomRight },
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

	//// set mesh data
	//mesh.set_vertices(static_cast<void const*>(vertices.data()), vertices.size(), sizeof(Vertex), engine);
	//mesh.set_indices(static_cast<void const*>(indices.data()), indices.size(), sizeof(uint16_t), engine);

	// set mesh data
	mesh.set_vertices(vertices);
	mesh.set_indices(indices);
}

void basic::create_basic_quad(Mesh& mesh)
{
	// create mesh data
	const float SIZE = 0.5f;

	glm::vec3 leftTopBack = { -SIZE, 0.0f, -SIZE };
	glm::vec3 leftTopFront = { -SIZE, 0.0f, SIZE };
	glm::vec3 rightTopBack = { SIZE, 0.0f, -SIZE };
	glm::vec3 rightTopFront = { SIZE, 0.0f, SIZE };

	glm::vec3 color = { 1.0f, 1.0f, 1.0f };

	glm::vec2 topLeft = { 0.0f, 0.0f };
	glm::vec2 topRight = { 1.0f, 0.0f };
	glm::vec2 bottomLeft = { 0.0f, 1.0f };
	glm::vec2 bottomRight = { 1.0f, 1.0f };

	std::vector<Vertex> vertices =
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