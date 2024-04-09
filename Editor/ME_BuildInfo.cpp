#include "ME_BuildInfo.h"

using namespace minty;
using namespace mintye;

mintye::BuildInfo::BuildInfo()
	: _buildFlags(BuildFlags::All)
	, _release()
{}

bool mintye::BuildInfo::get_flag(BuildFlags const flag) const
{
	return (_buildFlags & flag) != BuildFlags::None;
}

void mintye::BuildInfo::set_flag(BuildFlags const flag)
{
	_buildFlags = _buildFlags | flag;
}

void mintye::BuildInfo::clear_flags()
{
	_buildFlags = BuildFlags::None;
}

void mintye::BuildInfo::set_config(bool const release)
{
	_release = release;
}

bool mintye::BuildInfo::get_config() const
{
	return _release;
}

String const mintye::BuildInfo::get_config_name() const
{
	return _release ? "Release" : "Debug";
}

BuildInfo::BuildFlags mintye::operator|(BuildInfo::BuildFlags const left, BuildInfo::BuildFlags const right)
{
	return static_cast<BuildInfo::BuildFlags>(static_cast<int>(left) | static_cast<int>(right));
}

BuildInfo::BuildFlags mintye::operator&(BuildInfo::BuildFlags const left, BuildInfo::BuildFlags const right)
{
	return static_cast<BuildInfo::BuildFlags>(static_cast<int>(left) & static_cast<int>(right));
}
