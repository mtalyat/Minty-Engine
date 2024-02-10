#include "ME_BuildInfo.h"

using namespace minty;

String const mintye::BuildInfo::get_config() const
{
	return debug ? "Debug" : "Release";
}
