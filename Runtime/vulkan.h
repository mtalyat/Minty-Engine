#pragma once

#include <vulkan/vulkan.h>
#include "M_String.h"

#define VK_ASSERT(expression, message) { VkResult result = expression; if(result != VkResult::VK_SUCCESS) { minty::error::abort(std::format("[{}] {}", minty::vk::to_string(result), message)); } }

namespace minty::vk
{
	std::string to_string(VkResult const result);

	std::string to_string(VkFilter const value);
	VkFilter from_string_vk_filter(std::string const& value);

	std::string to_string(VkFormat const value);
	VkFormat from_string_vk_format(std::string const& value);

	std::string to_string(VkSamplerAddressMode const value);
	VkSamplerAddressMode from_string_vk_sampler_address_mode(std::string const& value);

	std::string to_string(VkSamplerMipmapMode const value);
	VkSamplerMipmapMode from_string_vk_sampler_mipmap_mode(std::string const& value);

	std::string to_string(VkShaderStageFlagBits const value);
	VkShaderStageFlagBits from_string_vk_shader_stage_flag_bits(std::string const& value);

	std::string to_string(VkDescriptorType const value);
	VkDescriptorType from_string_vk_descriptor_type(std::string const& value);

	std::string to_string(VkPrimitiveTopology const value);
	VkPrimitiveTopology from_string_vk_primitive_topology(std::string const& value);

	std::string to_string(VkPolygonMode const value);
	VkPolygonMode from_string_vk_polygon_mode(std::string const& value);

	std::string to_string(VkCullModeFlagBits const value);
	VkCullModeFlagBits from_string_vk_cull_mode_flag_bits(std::string const& value);

	std::string to_string(VkFrontFace const value);
	VkFrontFace from_string_vk_front_face(std::string const& value);

	std::string to_string(VkVertexInputRate const value);
	VkVertexInputRate from_string_vk_vertex_input_rate(std::string const& value);
}