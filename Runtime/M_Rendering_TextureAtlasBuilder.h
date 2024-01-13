#pragma once

#include "M_Base.h"
#include "M_PixelCoordinateMode.h"
#include "M_Vector.h"

namespace minty::rendering
{
	struct TextureAtlasBuilder
	{
		ID textureId = ERROR_ID;

		ID materialId = ERROR_ID;

		PixelCoordinateMode coordinateMode = PixelCoordinateMode::Normalized;

		Vector2 slice = Vector2(1.0f, 1.0f);

		Vector2 pivot = Vector2(0.5f, 0.5f);
	};
}