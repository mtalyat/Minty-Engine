#include "pch.h"
#include "M_TextureAtlas.h"

#include "M_RenderEngine.h"
#include "M_Sprite.h"

using namespace minty;

minty::TextureAtlas::TextureAtlas(rendering::TextureAtlasBuilder const& builder, RenderEngine& renderer)
	: rendering::RenderObject(renderer)
	, _textureId(builder.textureId)
	, _materialId(builder.materialId)
	, _slice()
	, _pivot()
{
	MINTY_ASSERT(_textureId != ERROR_ID, "Cannot create a TextureAtlas with a Texture ID of ERROR_ID.");

	// set sizes
	set_slice(builder.slice, builder.coordinateMode);
	set_pivot(builder.pivot, builder.coordinateMode);

	// if no material given, create a new one
	if (_materialId == ERROR_ID)
	{
		// create a material based on this texture
		rendering::MaterialBuilder materialBuilder
		{
			.name = std::format("{}_atlas", _renderer.get_texture_name(_textureId)),
			.templateId = _renderer.find_material_template("spriteMaterialTemplate"), // TODO: hard coded name
		};

		// add texture to values
		materialBuilder.values.emplace("texture", Dynamic(&_textureId, sizeof(ID)));

		// create material
		_materialId = _renderer.create_material(materialBuilder);
	}	
}

minty::TextureAtlas::~TextureAtlas()
{}

void minty::TextureAtlas::set_slice(Vector2 const size, PixelCoordinateMode const coordinateMode)
{
	switch (coordinateMode)
	{
	case PixelCoordinateMode::Normalized:
	{
		// convert from normalized to pixel
		Texture const& texture = _renderer.get_texture(_textureId);
		_slice = Vector2(size.x * static_cast<float>(texture.get_width()), size.y * static_cast<float>(texture.get_height()));
		break;
	}
	case PixelCoordinateMode::Pixel:
		_slice = size;
		break;
	default:
		return;
	}
}

void minty::TextureAtlas::set_pivot(Vector2 const pivot, PixelCoordinateMode const coordinateMode)
{
	switch (coordinateMode)
	{
	case PixelCoordinateMode::Normalized:
	{
		// convert from normalized to pixel
		Texture const& texture = _renderer.get_texture(_textureId);
		_pivot = Vector2(pivot.x * static_cast<float>(texture.get_width()), pivot.y * static_cast<float>(texture.get_height()));
		break;
	}
	case PixelCoordinateMode::Pixel:
		_pivot = pivot;
		break;
	default:
		return;
	}
}

Vector2Int minty::TextureAtlas::get_size_in_slices() const
{
	Texture const& texture = _renderer.get_texture(_textureId);

	return Vector2Int(math::floor_to_int(static_cast<float>(texture.get_width()) / _slice.x), math::floor_to_int(static_cast<float>(texture.get_height()) / _slice.y));
}

Vector2Int minty::TextureAtlas::get_size() const
{
	Texture const& texture = _renderer.get_texture(_textureId);

	return Vector2Int(texture.get_width(), texture.get_height());
}

ID minty::TextureAtlas::create_sprite(int const x, int const y, Vector2 const pivot, Vector2 const size, PixelCoordinateMode const coordinateMode) const
{
	// create name from the texture name and the x/y position
	return create_sprite(x, y, pivot, size, std::format("{}_{}_{}", _renderer.get_texture_name(_textureId), x, y), coordinateMode);
}

ID minty::TextureAtlas::create_sprite(int const x, int const y, Vector2 const pivot, Vector2 const size, String const& name, PixelCoordinateMode const coordinateMode) const
{
	// get the appropriate pivot
	Vector2 fixedPivot;

	switch (coordinateMode)
	{
	case PixelCoordinateMode::Normalized:
	{
		fixedPivot = pivot;
		break;
	}
	case PixelCoordinateMode::Pixel:
	{
		// convert from pixel to normalized
		Texture const& texture = _renderer.get_texture(_textureId);
		fixedPivot = Vector2(pivot.x / static_cast<float>(texture.get_width()), pivot.y / static_cast<float>(texture.get_height()));
		break;
	}
	default:
		return ERROR_ID;
	}

	// get texture
	Texture const& texture = _renderer.get_texture(_textureId);
	float width = static_cast<float>(texture.get_width());
	float height = static_cast<float>(texture.get_height());

	// find position using cell size
	Vector2 minCoords(static_cast<float>(x) * _slice.x / width, static_cast<float>(y) * _slice.y / height);
	Vector2 maxCoords(static_cast<float>(x + 1) * _slice.x / width, static_cast<float>(y + 1) * _slice.y / height);

	// create builder
	rendering::SpriteBuilder builder
	{
		.name = name,
		.textureId = _textureId,
		.materialId = _materialId,
		.mode = PixelCoordinateMode::Normalized,
		.minCoords = minCoords,
		.maxCoords = maxCoords,
		.pivot = fixedPivot,
		.size = Vector2(1.0f, 1.0f),
	};

	// create the sprite
	return _renderer.create_sprite(builder);
}
