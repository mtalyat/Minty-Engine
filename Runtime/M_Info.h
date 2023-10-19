#pragma once

#include "M_Object.h"
#include <vulkan/vulkan.h>

//#define MINTY_NAME "Minty Engine"
//#define MINTY_VERSION VK_MAKE_VERSION(1, 0, 0)
//#define MINTY_API_VERSION VK_API_VERSION_1_2

namespace minty
{
	constexpr char const* MINTY_NAME = "Minty Engine";
	constexpr uint32_t const MINTY_VERSION = VK_MAKE_VERSION(0, 1, 0);
	constexpr uint32_t const MINTY_API_VERSION = VK_API_VERSION_1_2;

	class Info
		: public Object
	{
	private:
		std::string _applicationName;
		uint32_t _applicationVersion;

	public:
		Info(std::string const& name, uint32_t const major, uint32_t const minor, uint32_t const patch);

		VkApplicationInfo get_application_info() const;

	public:
		friend std::string to_string(Info const& value);
	};
}