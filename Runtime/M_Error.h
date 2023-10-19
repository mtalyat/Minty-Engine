#pragma once

#include <vulkan/vulkan.h>
#include <string>

#define VK_ASSERT(result, expression, message) VkResult result = expression; if(result != VkResult::VK_SUCCESS) minty::error::abort(std::format("[{}] {}", minty::error::result_to_string(result), message));

#ifdef NDEBUG
#define MINTY_ASSERT(expression, message)
#else
#define MINTY_ASSERT(expression, message) if(!(expression)) minty::error::abort(message);
#endif

namespace minty::error
{
	constexpr char const* ERROR_TEXT = "ERROR";

	std::string result_to_string(VkResult const result);

	void abort(std::string const& message);
}