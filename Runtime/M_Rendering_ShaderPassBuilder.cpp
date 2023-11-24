#include "pch.h"
#include "M_Rendering_ShaderPassBuilder.h"

#include "M_Renderer.h"

using namespace minty;
using namespace minty::rendering;

ShaderPassBuilder::ShaderPassBuilder(ID const shaderId)
	: _shaderId(shaderId)
	, _stages()
	, _topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
	, _polygonMode(VK_POLYGON_MODE_FILL)
	, _cullMode(VK_CULL_MODE_BACK_BIT)
	, _frontFace(VK_FRONT_FACE_CLOCKWISE)
	, _lineWidth(1.0f)
	, _vertexBindings()
	, _vertexAttributes()
{}

void minty::rendering::ShaderPassBuilder::emplace_stage(VkShaderStageFlagBits const stage, std::string const& path, Renderer const& renderer, std::string const entry)
{
	emplace_stage(stage, renderer.load_file(path), entry);
}

void minty::rendering::ShaderPassBuilder::emplace_stage(VkShaderStageFlagBits const stage, std::vector<char> const& code, std::string const entry)
{
	_stages.push_back(ShaderStageInfo
		{
			.stage = stage,
			.code = code,
			.entry = entry
		});
}

void minty::rendering::ShaderPassBuilder::set_topology(VkPrimitiveTopology const topology)
{
	_topology = topology;
}

void minty::rendering::ShaderPassBuilder::set_polygon_mode(VkPolygonMode const polygonMode)
{
	_polygonMode = polygonMode;
}

void minty::rendering::ShaderPassBuilder::set_cull_mode(VkCullModeFlags const cullModeFlags)
{
	_cullMode = cullModeFlags;
}

void minty::rendering::ShaderPassBuilder::set_front_face(VkFrontFace const frontFace)
{
	_frontFace = frontFace;
}

void minty::rendering::ShaderPassBuilder::set_line_width(float const width)
{
	_lineWidth = width;
}

void minty::rendering::ShaderPassBuilder::emplace_vertex_binding(uint32_t const binding, uint32_t const stride, VkVertexInputRate const inputRate)
{
	_vertexBindings.push_back(VkVertexInputBindingDescription
		{
			.binding = binding,
			.stride = stride,
			.inputRate = inputRate,
		});
}

void minty::rendering::ShaderPassBuilder::emplace_vertex_attribute(uint32_t const binding, uint32_t const location, VkDeviceSize const size, VkDeviceSize const offset, VkFormat const format)
{
	VkVertexInputAttributeDescription desc =
	{
		.location = location,
		.binding = binding,
		.format = format,
		.offset = static_cast<uint32_t>(offset),
	};

	_vertexAttributes.push_back(VertexInputAttribute
		{
			.description = desc,
			.size = size,
		});
}

ID minty::rendering::ShaderPassBuilder::get_shader_id() const
{
	return _shaderId;
}

std::vector<ShaderPassBuilder::ShaderStageInfo> const& minty::rendering::ShaderPassBuilder::get_stages() const
{
	return _stages;
}

VkPrimitiveTopology minty::rendering::ShaderPassBuilder::get_topology() const
{
	return _topology;
}

VkPolygonMode minty::rendering::ShaderPassBuilder::get_polygon_mode() const
{
	return _polygonMode;
}

VkCullModeFlags minty::rendering::ShaderPassBuilder::get_cull_mode() const
{
	return _cullMode;
}

VkFrontFace minty::rendering::ShaderPassBuilder::get_front_face() const
{
	return _frontFace;
}

float minty::rendering::ShaderPassBuilder::get_line_width() const
{
	return _lineWidth;
}

std::vector<VkVertexInputBindingDescription> const& minty::rendering::ShaderPassBuilder::get_vertex_bindings() const
{
	return _vertexBindings;
}

std::vector<VkVertexInputAttributeDescription> minty::rendering::ShaderPassBuilder::get_vertex_attributes() const
{
	// get the descriptions only
	std::vector<VkVertexInputAttributeDescription> result;
	result.reserve(_vertexAttributes.size());

	for (auto const& value : _vertexAttributes)
	{
		result.push_back(value.description);
	}

	return result;
}
