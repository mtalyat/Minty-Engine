#include "pch.h"
#include "Minty/Rendering/M_Sprite.h"

#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Rendering/M_RenderSystem.h"
#include "Minty/Rendering/M_Texture.h"
#include "Minty/Tools/M_Console.h"

using namespace Minty;
using namespace Minty;

Minty::Sprite::Sprite(SpriteBuilder const& builder)
	: Asset(builder.id, builder.path)
	, _texture(builder.texture)
	, _material(builder.material)
	, _minCoords()
	, _maxCoords()
	, _pivot()
{
	MINTY_ASSERT(builder.texture != nullptr);
	MINTY_ASSERT(builder.material != nullptr);

	// set values based on coordinate mode
	// all values should be normalized inside of Sprite
	set_min_coords(builder.minCoords, builder.coordinateMode);
	set_max_coords(builder.maxCoords, builder.coordinateMode);
	set_pivot(builder.pivot, builder.coordinateMode);
}

Minty::Sprite::~Sprite()
{
	destroy();
}

void Minty::Sprite::set_min_coords(Vector2 const coords, CoordinateMode const coordinateMode)
{
	switch (coordinateMode)
	{
	case CoordinateMode::Normalized:
		_minCoords = coords;
		break;
	case CoordinateMode::Pixel:
		_minCoords = normalize_coords(coords);
		break;
	default:
		MINTY_ERROR("set_min_coords invalid CoordinateMode.");
	}
}

void Minty::Sprite::set_max_coords(Vector2 const coords, CoordinateMode const coordinateMode)
{
	switch (coordinateMode)
	{
	case CoordinateMode::Normalized:
		_maxCoords = coords;
		break;
	case CoordinateMode::Pixel:
		_maxCoords = normalize_coords(coords);
		break;
	default:
		MINTY_ERROR("set_max_coords invalid CoordinateMode.");
	}
}

void Minty::Sprite::set_pivot(Vector2 const pivot, CoordinateMode const coordinateMode)
{
	switch (coordinateMode)
	{
	case CoordinateMode::Normalized:
		_pivot = pivot;
		break;
	case CoordinateMode::Pixel:
		_pivot = normalize_coords(pivot);
		break;
	default:
		MINTY_ERROR("set_pivot invalid CoordinateMode.");
	}
}

Vector2 Minty::Sprite::normalize_coords(Vector2 const coords) const
{
	return Vector2(coords.x / static_cast<float>(_texture->get_width()), coords.y / static_cast<float>(_texture->get_height()));
}
