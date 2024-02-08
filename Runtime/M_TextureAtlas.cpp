#include "pch.h"
#include "M_TextureAtlas.h"

#include "M_Texture.h"
#include "M_TextureAtlasBuilder.h"
#include "M_RenderEngine.h"
#include "M_Scene.h"
#include "M_Sprite.h"
#include "M_SpriteBuilder.h"
#include "M_RenderSystem.h"
#include "M_MaterialBuilder.h"

using namespace minty;

minty::TextureAtlas::TextureAtlas()
	: RenderObject()
	, _textureId(ERROR_ID)
	, _materialId(ERROR_ID)
	, _slice()
	, _pivot()
	, _createdSlices()
{}

minty::TextureAtlas::TextureAtlas(TextureAtlasBuilder const& builder, Engine& engine, ID const sceneId)
	: RenderObject(engine, sceneId)
	, _textureId(builder.textureId)
	, _materialId(builder.materialId)
	, _slice()
	, _pivot()
	, _createdSlices(0)
{
	MINTY_ASSERT(_textureId != ERROR_ID, "TextureAtlas::TextureAtlas(): Cannot create a TextureAtlas with a Texture ID of ERROR_ID.");

	// set sizes
	set_slice(builder.slice, builder.coordinateMode);
	set_pivot(builder.pivot, builder.coordinateMode);

	// if no material given, create a new one
	if (_materialId == ERROR_ID)
	{
		RenderSystem* renderSystem = get_render_system();

		MINTY_ASSERT(renderSystem != nullptr, "TextureAtlas::TextureAtlas(): renderSystem cannot be null.");

		// create a material based on this texture
		MaterialBuilder materialBuilder;
		materialBuilder.name = std::format("{}_atlas", renderSystem->get_texture_name(_textureId));
		materialBuilder.templateId = renderSystem->find_material_template("spriteMaterialTemplate"); // TODO: hard coded name

		// add texture to values
		materialBuilder.values.emplace("texture", Dynamic(&_textureId, sizeof(ID)));

		// create material
		_materialId = renderSystem->create_material(materialBuilder);
	}	
}

minty::TextureAtlas::~TextureAtlas()
{}

void minty::TextureAtlas::set_slice(Vector2 const size, CoordinateMode const coordinateMode)
{
	switch (coordinateMode)
	{
	case CoordinateMode::Normalized:
	{
		// convert from normalized to pixel
		Texture const& texture = get_render_system()->get_texture(_textureId);
		_slice = Vector2(size.x * static_cast<float>(texture.get_width()), size.y * static_cast<float>(texture.get_height()));
		break;
	}
	case CoordinateMode::Pixel:
		_slice = size;
		break;
	default:
		return;
	}
}

void minty::TextureAtlas::set_pivot(Vector2 const pivot, CoordinateMode const coordinateMode)
{
	switch (coordinateMode)
	{
	case CoordinateMode::Normalized:
	{
		// convert from normalized to pixel
		Texture const& texture = get_render_system()->get_texture(_textureId);
		_pivot = Vector2(pivot.x * static_cast<float>(texture.get_width()), pivot.y * static_cast<float>(texture.get_height()));
		break;
	}
	case CoordinateMode::Pixel:
		_pivot = pivot;
		break;
	default:
		return;
	}
}

Vector2Int minty::TextureAtlas::get_size_in_slices() const
{
	Texture const& texture = get_render_system()->get_texture(_textureId);

	return Vector2Int(Math::floor_to_int(static_cast<float>(texture.get_width()) / _slice.x), Math::floor_to_int(static_cast<float>(texture.get_height()) / _slice.y));
}

Vector2Int minty::TextureAtlas::get_size() const
{
	Texture const& texture = get_render_system()->get_texture(_textureId);

	return Vector2Int(texture.get_width(), texture.get_height());
}

ID minty::TextureAtlas::slice_sprite(Vector2 const minCoords, Vector2 const maxCoords, CoordinateMode const coordinateMode)
{
	return slice_sprite(minCoords, maxCoords, get_next_slice_name(), coordinateMode);
}

ID minty::TextureAtlas::slice_sprite(Vector2 const minCoords, Vector2 const maxCoords, String const& name, CoordinateMode const coordinateMode)
{
	return slice_sprite(minCoords, maxCoords, Vector2(0.5f, 0.5f), name, coordinateMode);
}

ID minty::TextureAtlas::slice_sprite(Vector2 const minCoords, Vector2 const maxCoords, Vector2 const pivot, String const& name, CoordinateMode const coordinateMode)
{
	// create builder
	SpriteBuilder builder
	{
		.name = name,
		.textureId = _textureId,
		.materialId = _materialId,
		.coordinateMode = CoordinateMode::Normalized,
		.minCoords = minCoords,
		.maxCoords = maxCoords,
		.pivot = pivot,
	};

	// increment total slices created
	_createdSlices++;

	// create the sprite
	return get_render_system()->create_sprite(builder);
}

ID minty::TextureAtlas::create_sprite(int const x, int const y, Vector2 const pivot, CoordinateMode const coordinateMode)
{
	// create name from the texture name and the x/y position
	return create_sprite(x, y, pivot, get_next_slice_name(), coordinateMode);
}

ID minty::TextureAtlas::create_sprite(int const x, int const y, Vector2 const pivot, String const& name, CoordinateMode const coordinateMode)
{
	// get texture
	Texture const& texture = get_render_system()->get_texture(_textureId);
	float width = static_cast<float>(texture.get_width());
	float height = static_cast<float>(texture.get_height());

	// find position using cell size
	Vector2 minCoords(static_cast<float>(x) * _slice.x / width, static_cast<float>(y) * _slice.y / height);
	Vector2 maxCoords(static_cast<float>(x + 1) * _slice.x / width, static_cast<float>(y + 1) * _slice.y / height);

	switch (coordinateMode)
	{
	case CoordinateMode::Normalized:

		break;
	case CoordinateMode::Pixel:

		break;
	default:
		return ERROR_ID;
	}

	return slice_sprite(minCoords, maxCoords, pivot, name, coordinateMode);
}

std::vector<ID> minty::TextureAtlas::create_all(Vector2 const pivot, CoordinateMode const coordinateMode)
{
	// get size in slices
	Vector2Int size = get_size_in_slices();

	// create all the sprites
	std::vector<ID> ids(size.x * size.y);

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			ids[y * size.x + x] = create_sprite(x, y, pivot, coordinateMode);
		}
	}

	return ids;
}

String minty::TextureAtlas::get_next_slice_name() const
{
	return std::format("{}_{}", get_render_system()->get_texture_name(_textureId), _createdSlices);
}
