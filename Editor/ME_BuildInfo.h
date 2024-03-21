#pragma once

#include "ME_Minty.h"

namespace mintye
{
	class Project;

	struct BuildInfo
	{
		bool debug;

		minty::String const get_config() const;
	};
}