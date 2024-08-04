#include "ME_BuildInfo.h"

using namespace Minty;
using namespace Mintye;

Mintye::BuildInfo::BuildInfo()
	: _buildFlags(BuildFlags::All)
	, _release()
{}

bool Mintye::BuildInfo::get_flag(BuildFlags const flag) const
{
	return (_buildFlags & flag) != BuildFlags::None;
}

void Mintye::BuildInfo::set_flag(BuildFlags const flag)
{
	_buildFlags = _buildFlags | flag;
}

void Mintye::BuildInfo::clear_flags()
{
	_buildFlags = BuildFlags::None;
}

void Mintye::BuildInfo::set_config(bool const release)
{
	_release = release;
}

String const Mintye::BuildInfo::get_config_name() const
{
	return _release ? "Release" : "Debug";
}

BuildInfo::BuildFlags Mintye::operator|(BuildInfo::BuildFlags const left, BuildInfo::BuildFlags const right)
{
	return static_cast<BuildInfo::BuildFlags>(static_cast<int>(left) | static_cast<int>(right));
}

BuildInfo::BuildFlags Mintye::operator&(BuildInfo::BuildFlags const left, BuildInfo::BuildFlags const right)
{
	return static_cast<BuildInfo::BuildFlags>(static_cast<int>(left) & static_cast<int>(right));
}
