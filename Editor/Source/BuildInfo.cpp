#include "BuildInfo.h"

using namespace Minty;
using namespace Mintye;

Mintye::BuildInfo::BuildInfo()
	: m_buildFlags(BuildFlags::All)
	, m_release()
{}

bool Mintye::BuildInfo::get_flag(BuildFlags const flag) const
{
	return (m_buildFlags & flag) != BuildFlags::None;
}

void Mintye::BuildInfo::set_flag(BuildFlags const flag)
{
	m_buildFlags = m_buildFlags | flag;
}

void Mintye::BuildInfo::clear_flags()
{
	m_buildFlags = BuildFlags::None;
}

void Mintye::BuildInfo::set_config(bool const release)
{
	m_release = release;
}

String const Mintye::BuildInfo::get_config_name() const
{
	return m_release ? "Release" : "Debug";
}

BuildInfo::BuildFlags Mintye::operator|(BuildInfo::BuildFlags const left, BuildInfo::BuildFlags const right)
{
	return static_cast<BuildInfo::BuildFlags>(static_cast<int>(left) | static_cast<int>(right));
}

BuildInfo::BuildFlags Mintye::operator&(BuildInfo::BuildFlags const left, BuildInfo::BuildFlags const right)
{
	return static_cast<BuildInfo::BuildFlags>(static_cast<int>(left) & static_cast<int>(right));
}
