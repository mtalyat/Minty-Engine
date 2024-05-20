#pragma once

#include <vulkan/vulkan.h>
#include "types/M_Types.h"

#define VK_ASSERT(expression, message) { VkResult result = expression; if(result != VkResult::VK_SUCCESS) { MINTY_ABORT(std::format("[{}] {}", minty::vk::to_string(result), message)); } }

namespace minty::vk
{
	String to_string(VkResult const result);

	String to_string(VkFilter const value);
	VkFilter from_string_vk_filter(String const& value);

	String to_string(VkFormat const value);
	VkFormat from_string_vk_format(String const& value);

	String to_string(VkSamplerAddressMode const value);
	VkSamplerAddressMode from_string_vk_sampler_address_mode(String const& value);

	String to_string(VkSamplerMipmapMode const value);
	VkSamplerMipmapMode from_string_vk_sampler_mipmap_mode(String const& value);

	String to_string(VkShaderStageFlagBits const value);
	VkShaderStageFlagBits from_string_vk_shader_stage_flag_bits(String const& value);

	String to_string(VkDescriptorType const value);
	VkDescriptorType from_string_vk_descriptor_type(String const& value);

	String to_string(VkPrimitiveTopology const value);
	VkPrimitiveTopology from_string_vk_primitive_topology(String const& value);

	String to_string(VkPolygonMode const value);
	VkPolygonMode from_string_vk_polygon_mode(String const& value);

	String to_string(VkCullModeFlagBits const value);
	VkCullModeFlagBits from_string_vk_cull_mode_flag_bits(String const& value);

	String to_string(VkFrontFace const value);
	VkFrontFace from_string_vk_front_face(String const& value);

	String to_string(VkVertexInputRate const value);
	VkVertexInputRate from_string_vk_vertex_input_rate(String const& value);
}