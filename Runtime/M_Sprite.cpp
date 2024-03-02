#include "pch.h"
#include "M_Sprite.h"

#include "M_RenderEngine.h"
#include "M_RenderSystem.h"
#include "M_Texture.h"
#include "M_Console.h"

using namespace minty;
using namespace minty;

minty::Sprite::Sprite()
	: Asset()
	, _texture(nullptr)
	, _material(nullptr)
	, _minCoords()
	, _maxCoords()
	, _pivot()
{}

minty::Sprite::Sprite(SpriteBuilder const& builder, Runtime& engine)
	: Asset(builder.id, builder.path, engine)
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

void minty::Sprite::destroy()
{}

Texture* minty::Sprite::get_texture() const
{
	return _texture;
}

Material* minty::Sprite::get_material() const
{
	return _material;
}

Vector2 minty::Sprite::get_min_coords() const
{
	return _minCoords;
}

Vector2 minty::Sprite::get_max_coords() const
{
	return _maxCoords;
}

Vector2 minty::Sprite::get_pivot() const
{
	return _pivot;
}

void minty::Sprite::set_min_coords(Vector2 const coords, CoordinateMode const coordinateMode)
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
		Console::error("set_min_coords invalid CoordinateMode.");
	}
}

void minty::Sprite::set_max_coords(Vector2 const coords, CoordinateMode const coordinateMode)
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
		Console::error("set_max_coords invalid CoordinateMode.");
	}
}

void minty::Sprite::set_pivot(Vector2 const pivot, CoordinateMode const coordinateMode)
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
		Console::error("set_pivot invalid CoordinateMode.");
	}
}

Vector2 minty::Sprite::normalize_coords(Vector2 const coords) const
{
	return Vector2(coords.x / static_cast<float>(_texture->get_width()), coords.y / static_cast<float>(_texture->get_height()));
}
