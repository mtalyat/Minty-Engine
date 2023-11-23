#pragma once

#include "M_Base.h"
#include "M_Matrix.h"

namespace minty::builtin
{
	struct GlobalData
	{
		alignas(16) float time;
		float deltaTime;
		float unused0;
		float unused1;

		alignas(16) Matrix4 view;
	};
}