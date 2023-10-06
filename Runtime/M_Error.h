#pragma once

#include <vulkan/vulkan.h>
#include <string>

#define VK_ASSERT(result, func, message) VkResult result = func; if(result != VkResult::VK_SUCCESS) minty::error::abort(std::format("[{}] {}", minty::error::result_to_string(result), message));

namespace minty::error
{
	std::string result_to_string(VkResult const result);

	void abort(std::string const& message);
}