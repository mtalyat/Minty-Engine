#include "pch.h"
#include "M_Info.h"

using namespace minty;

Info::Info(std::string const& name, uint32_t const major, uint32_t const minor, uint32_t const patch)
	: _applicationName(name)
	, _applicationVersion(VK_MAKE_VERSION(major, minor, patch))
{}

VkApplicationInfo minty::Info::get_application_info() const
{
	return VkApplicationInfo
	{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = _applicationName.c_str(),
		.applicationVersion = _applicationVersion,
		.pEngineName = MINTY_NAME,
		.engineVersion = MINTY_VERSION,
		.apiVersion = MINTY_API_VERSION,
	};
}
