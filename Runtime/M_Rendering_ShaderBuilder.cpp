#include "pch.h"
#include "M_Rendering_ShaderBuilder.h"

#include "M_Console.h"

using namespace minty;
using namespace minty::rendering;
//
//VkPrimitiveTopology _topology;
//VkPolygonMode _polygonMode;
//VkCullModeFlagBits _cullMode;
//VkFrontFace _frontFace;
//float _lineWidth = 1.0f; // use if polygone mode != fill
//
//std::vector<VkVertexInputBindingDescription> _vertexBindings;
//std::vector<VertexInputAttribute> _vertexAttributes;
//
//Register<VkPushConstantRange> _pushConstants;
//Register<UniformConstantInfo> _uniformConstants;

ShaderBuilder::ShaderBuilder()
	: _vertexEnterPoint("main")
	, _fragmentEnterPoint("main")
	, _topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
	, _polygonMode(VK_POLYGON_MODE_FILL)
	, _cullMode(VK_CULL_MODE_BACK_BIT)
	, _frontFace(VK_FRONT_FACE_CLOCKWISE)
	, _lineWidth(1.0f)
{}

void ShaderBuilder::set_vertex_enter_point(std::string const& enterPoint)
{
	_vertexEnterPoint = enterPoint;
}

void ShaderBuilder::set_fragment_enter_point(std::string const& enterPoint)
{
	_fragmentEnterPoint = enterPoint;
}

void minty::rendering::ShaderBuilder::set_line_width(float const width)
{
	_lineWidth = width;
}

void minty::rendering::ShaderBuilder::set_topology(VkPrimitiveTopology const topology)
{
	_topology = topology;
}

void minty::rendering::ShaderBuilder::set_polygon_mode(VkPolygonMode const polygonMode)
{
	_polygonMode = polygonMode;
}

void minty::rendering::ShaderBuilder::set_cull_mode(VkCullModeFlags const cullModeFlags)
{
	_cullMode = cullModeFlags;
}

void minty::rendering::ShaderBuilder::set_front_face(VkFrontFace const frontFace)
{
	_frontFace = frontFace;
}

std::string const& ShaderBuilder::get_vertex_enter_point() const
{
	return _vertexEnterPoint;
}

std::string const& ShaderBuilder::get_fragment_enter_point() const
{
	return _fragmentEnterPoint;
}

VkPrimitiveTopology minty::rendering::ShaderBuilder::get_topology() const
{
	return VkPrimitiveTopology();
}

VkPolygonMode minty::rendering::ShaderBuilder::get_polygon_mode() const
{
	return VkPolygonMode();
}

VkCullModeFlags minty::rendering::ShaderBuilder::get_cull_mode() const
{
	return VkCullModeFlags();
}

VkFrontFace minty::rendering::ShaderBuilder::get_front_face() const
{
	return VkFrontFace();
}

float minty::rendering::ShaderBuilder::get_line_width() const
{
	return _lineWidth;
}

std::vector<VkVertexInputBindingDescription> const& ShaderBuilder::get_vertex_bindings() const
{
	return _vertexBindings;
}

std::vector<VkVertexInputAttributeDescription> ShaderBuilder::get_vertex_attributes() const
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

std::vector<ShaderBuilder::PushConstantInfo> const& minty::rendering::ShaderBuilder::get_push_constant_infos() const
{
	return _pushConstants;
}

std::vector<VkDescriptorSetLayoutBinding> ShaderBuilder::get_descriptor_set_layouts() const
{
	std::vector<VkDescriptorSetLayoutBinding> result;

	for (auto const& constant : _uniformConstants)
	{
		result.push_back(VkDescriptorSetLayoutBinding
			{
				.binding = constant.binding,
				.descriptorType = constant.type,
				.descriptorCount = constant.descriptorCount,
				.stageFlags = constant.stageFlags,
				.pImmutableSamplers = nullptr
			});
	}

	return result;
}

std::vector<ShaderBuilder::UniformConstantInfo> const& minty::rendering::ShaderBuilder::get_uniform_constant_infos() const
{
	return _uniformConstants;
}

uint32_t minty::rendering::ShaderBuilder::get_uniform_constant_count() const
{
	return static_cast<uint32_t>(_uniformConstants.size());
}
