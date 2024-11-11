#pragma once

#include "Minty/Core/Math.h"
#include "Minty/Core/Type.h"

namespace Minty
{
	struct Rect
	{
		union
		{
			struct {
				Float x, y, width, height;
			};
			struct {
				Float2 position, size;
			};
			Float4 rect;
		};

		Rect()
			: rect()
		{}

		Rect(Float const x, Float const y, Float const width, Float const height)
			: x(x), y(y), width(width), height(height)
		{}

		Rect(Float2 const position, Float2 const size)
			: position(position), size(size)
		{}

		Rect(Float4 const rect)
			: rect(rect)
		{}
	};
}