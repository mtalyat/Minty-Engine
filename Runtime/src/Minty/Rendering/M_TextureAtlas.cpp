#include "pch.h"
#include "Minty/Rendering/M_TextureAtlas.h"

#include "Minty/Rendering/M_Texture.h"
#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Rendering/M_Sprite.h"
#include "Minty/Rendering/M_RenderSystem.h"
#include "Minty/Assets/M_AssetEngine.h"

using namespace Minty;

Minty::TextureAtlas::TextureAtlas(TextureAtlasBuilder const& builder)
	: Asset(builder.id, builder.path)
	, _texture(builder.texture)
	, _material(builder.material)
	, _slice()
	, _pivot()
	, _createdSlices(0)
{
	MINTY_TODO("TextureAtlas::TextureAtlas()");
	//MINTY_ASSERT(_texture != nullptr);

	//// set sizes
	//set_slice(builder.slice, builder.coordinateMode);
	//set_pivot(builder.pivot, builder.coordinateMode);

	//// if no material given, create a new one
	//if (!_material)
	//{
	//	RenderSystem* renderSystem = get_render_system();

	//	MINTY_ASSERT(renderSystem != nullptr);

	//	AssetEngine& engine = runtime.AssetEngine::instance();

	//	// create a material based on this texture
	//	MaterialBuilder materialBuilder;
	//	materialBuilder.materialTemplate = renderSystem->find_material_template("spriteMaterialTemplate"); // TODO: hard coded name

	//	// add texture to values
	//	materialBuilder.values.emplace("texture", Dynamic(&_textureId, sizeof(ID)));

	//	// create material
	//	_materialId = renderSystem->create_material(materialBuilder);
	//}
}

void Minty::TextureAtlas::set_slice(Vector2 const size, CoordinateMode const coordinateMode)
{
	switch (coordinateMode)
	{
	case CoordinateMode::Normalized:
	{
		// convert from normalized to pixel
		_slice = Vector2(size.x * static_cast<float>(_texture->get_width()), size.y * static_cast<float>(_texture->get_height()));
		break;
	}
	case CoordinateMode::Pixel:
		_slice = size;
		break;
	default:
		return;
	}
}

void Minty::TextureAtlas::set_pivot(Vector2 const pivot, CoordinateMode const coordinateMode)
{
	switch (coordinateMode)
	{
	case CoordinateMode::Normalized:
	{
		// convert from normalized to pixel
		_pivot = Vector2(pivot.x * static_cast<float>(_texture->get_width()), pivot.y * static_cast<float>(_texture->get_height()));
		break;
	}
	case CoordinateMode::Pixel:
		_pivot = pivot;
		break;
	default:
		return;
	}
}

Vector2Int Minty::TextureAtlas::get_size_in_slices() const
{
	return Vector2Int(Math::floor_to_int(static_cast<float>(_texture->get_width()) / _slice.x), Math::floor_to_int(static_cast<float>(_texture->get_height()) / _slice.y));
}

Vector2Int Minty::TextureAtlas::get_size() const
{
	return Vector2Int(_texture->get_width(), _texture->get_height());
}

Ref<Sprite> Minty::TextureAtlas::slice_sprite(Vector2 const minCoords, Vector2 const maxCoords, CoordinateMode const coordinateMode)
{
	return slice_sprite(minCoords, maxCoords, get_next_slice_name(), coordinateMode);
}

Ref<Sprite> Minty::TextureAtlas::slice_sprite(Vector2 const minCoords, Vector2 const maxCoords, String const& name, CoordinateMode const coordinateMode)
{
	return slice_sprite(minCoords, maxCoords, Vector2(0.5f, 0.5f), name, coordinateMode);
}

Ref<Sprite> Minty::TextureAtlas::slice_sprite(Vector2 const minCoords, Vector2 const maxCoords, Vector2 const pivot, String const& name, CoordinateMode const coordinateMode)
{
	// create builder
	SpriteBuilder builder
	{
		.texture = _texture,
		.material = _material,
		.coordinateMode = CoordinateMode::Normalized,
		.minCoords = minCoords,
		.maxCoords = maxCoords,
		.pivot = pivot,
	};

	// increment total slices created
	_createdSlices++;

	// create the sprite
	AssetEngine& assets = AssetEngine::instance();

	return assets.create<Sprite>(builder);
}

Ref<Sprite> Minty::TextureAtlas::create_sprite(int const x, int const y, Vector2 const pivot, CoordinateMode const coordinateMode)
{
	// create name from the texture name and the x/y position
	return create_sprite(x, y, pivot, get_next_slice_name(), coordinateMode);
}

Ref<Sprite> Minty::TextureAtlas::create_sprite(int const x, int const y, Vector2 const pivot, String const& name, CoordinateMode const coordinateMode)
{
	// get texture
	float width = static_cast<float>(_texture->get_width());
	float height = static_cast<float>(_texture->get_height());

	// find position using cell size
	Vector2 minCoords(static_cast<float>(x) * _slice.x / width, static_cast<float>(y) * _slice.y / height);
	Vector2 maxCoords(static_cast<float>(x + 1) * _slice.x / width, static_cast<float>(y + 1) * _slice.y / height);

	//switch (coordinateMode)
	//{
	//case CoordinateMode::Normalized:

	//	break;
	//case CoordinateMode::Pixel:

	//	break;
	//default:
	//	return nullptr;
	//}

	return slice_sprite(minCoords, maxCoords, pivot, name, coordinateMode);
}

std::vector<Ref<Sprite>> Minty::TextureAtlas::create_all(Vector2 const pivot, CoordinateMode const coordinateMode)
{
	// get size in slices
	Vector2Int size = get_size_in_slices();

	// create all the sprites
	std::vector<Ref<Sprite>> sprites(size.x * size.y);

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			sprites[y * size.x + x] = create_sprite(x, y, pivot, coordinateMode);
		}
	}

	return sprites;
}

String Minty::TextureAtlas::get_next_slice_name() const
{
	return std::format("{}", _createdSlices);
}
