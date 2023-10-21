#include "pch.h"
#include "M_Sprite.h"

using namespace minty;

minty::Sprite::Sprite(ID const materialId)
	: Sprite(materialId, Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f), Vector2(0.0f, 0.0f))
{}

minty::Sprite::Sprite(ID const materialId, Vector2 const pivot)
	: Sprite(materialId, Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f), pivot)
{}

minty::Sprite::Sprite(ID const materialId, Vector2 const minCoords, Vector2 const maxCoords)
	: Sprite(materialId, minCoords, maxCoords, Vector2(0.0f, 0.0f))
{}

minty::Sprite::Sprite(ID const materialId, Vector2 const minCoords, Vector2 const maxCoords, Vector2 const pivot)
	: _materialId(materialId)
	, _min(minCoords)
	, _max(maxCoords)
	, _pivot(pivot)
{}

ID minty::Sprite::get_material_id() const
{
	return _materialId;
}

Vector2 minty::Sprite::get_min_coords() const
{
	return _min;
}

Vector2 minty::Sprite::get_max_coords() const
{
	return _max;
}

Vector2 minty::Sprite::get_pivot() const
{
	return _pivot;
}

void minty::Sprite::set_min_coords(Vector2 const coords)
{
	_min = coords;
}

void minty::Sprite::set_max_coords(Vector2 const coords)
{
	_max = coords;
}

void minty::Sprite::set_pivot(Vector2 const pivot)
{
	_pivot = pivot;
}
