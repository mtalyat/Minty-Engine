#pragma once

#include "Minty/Core/String.h"
#include "Minty/Core/Type.h"

namespace Minty
{
	/// <summary>
	/// Holds info for an application instance.
	/// </summary>
	struct ApplicationInfo
	{
		String name;
		UInt version;
	};
}