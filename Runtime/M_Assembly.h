#pragma once

#include "M_Types.h"
#include "M_Mono.h"

namespace minty
{
	/// <summary>
	/// A generic assembly.
	/// </summary>
	class Assembly
	{
	public:
		virtual ~Assembly();

		virtual void load(Path const& path);

		virtual void update();
	};
}