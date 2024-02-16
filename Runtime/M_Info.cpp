#include "pch.h"
#include "M_Info.h"

using namespace minty;

minty::Info::Info()
	: _applicationName()
	, _applicationMajor()
	, _applicationMinor()
	, _applicationPatch()
{}

Info::Info(String const& name, uint32_t const major, uint32_t const minor, uint32_t const patch)
	: _applicationName(name)
	, _applicationMajor(major)
	, _applicationMinor(minor)
	, _applicationPatch(patch)
{}

String const& minty::Info::get_application_name() const
{
	return _applicationName;
}

uint32_t minty::Info::get_application_major() const
{
	return _applicationMajor;
}

uint32_t minty::Info::get_application_minor() const
{
	return _applicationMinor;
}

uint32_t minty::Info::get_application_patch() const
{
	return _applicationPatch;
}

uint32_t minty::Info::get_application_version() const
{
	return VK_MAKE_VERSION(_applicationMajor, _applicationMinor, _applicationPatch);
}

VkApplicationInfo minty::Info::get_application_info() const
{
	return VkApplicationInfo
	{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = _applicationName.c_str(),
		.applicationVersion = get_application_version(),
		.pEngineName = MINTY_NAME,
		.engineVersion = MINTY_VERSION,
		.apiVersion = MINTY_API_VERSION,
	};
}

void minty::Info::serialize(Writer& writer) const
{
	writer.write("name", _applicationName);
	writer.write("major", _applicationMajor);
	writer.write("minor", _applicationMinor);
	writer.write("patch", _applicationPatch);
}

void minty::Info::deserialize(Reader const& reader)
{
	_applicationName = reader.read_string("name");
	_applicationMajor = reader.read_uint("major");
	_applicationMinor = reader.read_uint("minor");
	_applicationPatch = reader.read_uint("patch");
}

String minty::to_string(Info const& value)
{
	return std::format("Info(application name = {}, application version = {}, engine name = {}, engine version = {}, Vulkan version = {})", value.get_application_name(), value.get_application_version(), MINTY_NAME, MINTY_VERSION, MINTY_API_VERSION);
}
