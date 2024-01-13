#include "pch.h"
#include "M_Sprite.h"

#include "M_RenderEngine.h"
#include "M_Texture.h"

using namespace minty;
using namespace minty::rendering;

minty::Sprite::Sprite(rendering::SpriteBuilder const& builder, RenderEngine& renderer)
	: RenderObject::RenderObject(renderer)
	, _textureId(builder.textureId)
	, _materialId(builder.materialId)
	, _minCoords(builder.minimum)
	, _maxCoords(builder.maximum)
	, _pivot(builder.pivot)
{
	MINTY_ASSERT(builder.textureId != ERROR_ID, "Cannot create a sprite with ERROR_ID texture id.");
	MINTY_ASSERT(builder.materialId != ERROR_ID, "Cannot create a sprite with ERROR_ID material id.");
}

void minty::Sprite::destroy()
{}

ID minty::Sprite::get_material_id() const
{
	return _materialId;
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

void minty::Sprite::set_min_coords(Vector2 const coords, PixelCoordinateMode const coordinateMode)
{
	switch (coordinateMode)
	{
	case PixelCoordinateMode::Normalized:
		_minCoords = coords;
		break;
	case PixelCoordinateMode::Pixel:
		_minCoords = normalize_coords(coords);
		break;
	default:
		console::error("set_min_coords invalid PixelCoordinateMode.");
	}
}

void minty::Sprite::set_max_coords(Vector2 const coords, PixelCoordinateMode const coordinateMode)
{
	switch (coordinateMode)
	{
	case PixelCoordinateMode::Normalized:
		_maxCoords = coords;
		break;
	case PixelCoordinateMode::Pixel:
		_maxCoords = normalize_coords(coords);
		break;
	default:
		console::error("set_max_coords invalid PixelCoordinateMode.");
	}
}

void minty::Sprite::set_pivot(Vector2 const pivot, PixelCoordinateMode const coordinateMode)
{
	switch (coordinateMode)
	{
	case PixelCoordinateMode::Normalized:
		_pivot = pivot;
		break;
	case PixelCoordinateMode::Pixel:
		_pivot = normalize_coords(pivot);
		break;
	default:
		console::error("set_pivot invalid PixelCoordinateMode.");
	}
}

Vector2 minty::Sprite::normalize_coords(Vector2 const coords) const
{
	Texture const& tex = _renderer.get_texture(_textureId);

	return Vector2(coords.x / static_cast<float>(tex.get_width()), coords.y / static_cast<float>(tex.get_height()));
}
