#include "pch.h"
#include "M_Sprite.h"

#include "M_RenderEngine.h"
#include "M_Texture.h"

using namespace minty;
using namespace minty::rendering;

minty::Sprite::Sprite()
	: RenderObject::RenderObject()
	, _textureId(ERROR_ID)
	, _materialId(ERROR_ID)
	, _minCoords()
	, _maxCoords()
	, _pivot()
{}

minty::Sprite::Sprite(rendering::SpriteBuilder const& builder, RenderEngine& renderer)
	: RenderObject::RenderObject(&renderer)
	, _textureId(builder.textureId)
	, _materialId(builder.materialId)
	, _minCoords()
	, _maxCoords()
	, _pivot()
{
	MINTY_ASSERT(builder.textureId != ERROR_ID, "Cannot create a sprite with ERROR_ID texture id.");
	MINTY_ASSERT(builder.materialId != ERROR_ID, "Cannot create a sprite with ERROR_ID material id.");

	// set values based on coordinate mode
	// all values should be normalized inside of Sprite
	set_min_coords(builder.minCoords, builder.coordinateMode);
	set_max_coords(builder.maxCoords, builder.coordinateMode);
	set_pivot(builder.pivot, builder.coordinateMode);
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
		console::error("set_min_coords invalid CoordinateMode.");
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
		console::error("set_max_coords invalid CoordinateMode.");
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
		console::error("set_pivot invalid CoordinateMode.");
	}
}

Vector2 minty::Sprite::normalize_coords(Vector2 const coords) const
{
	Texture const& tex = get_renderer()->get_texture(_textureId);

	return Vector2(coords.x / static_cast<float>(tex.get_width()), coords.y / static_cast<float>(tex.get_height()));
}
