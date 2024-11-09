#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Core/Math.h"
#include "Minty/Render/CoordinateMode.h"
#include "Minty/Render/Material.h"
#include "Minty/Render/Texture.h"

namespace Minty
{
	struct SpriteBuilder
	{
		UUID id = {};
		Ref<Texture> texture = nullptr;
		CoordinateMode coordinateMode = CoordinateMode::Normalized;
		Float2 offset = { 0.0f, 0.0f };
		Float2 size = { 1.0f, 1.0f };
		Float2 pivot = { 0.5f, 0.5f };
		Float pixelsPerUnit = 16.0f;
	};

	// a slice of a texture
	class Sprite
		: public Asset
	{
	private:
		Ref<Texture> m_texture;
		Ref<Material> m_material;
		CoordinateMode m_coordinateMode;
		// store offset, size, pivot as normalized, since that is what the shaders use
		Float2 m_offset;
		Float2 m_size;
		Float2 m_pivot;
		Float m_pixelsPerUnit;
		Float m_scale;

	public:
		Sprite(SpriteBuilder const& builder);

		~Sprite() = default;

	public:
		Ref<Texture> get_texture() const { return m_texture; }

		Ref<Material> get_material() const { return m_material; }

		CoordinateMode get_coordinate_mode() const { return m_coordinateMode; }

		Float2 get_offset() const;

		void set_offset(Float2 const offset);

		Float2 get_size() const;

		void set_size(Float2 const size);

		Float2 get_pivot() const;

		void set_pivot(Float2 const pivot);

		Float get_pixels_per_unit() const { return m_pixelsPerUnit; }

		void set_pixels_per_unit(Float const ppu);

		Float get_scale() const { return m_scale; }

	public:
		AssetType get_type() const override { return AssetType::Sprite; }

	private:
		Float2 get_coords(Float2 const raw) const;
		Float2 set_coords(Float2 const raw) const;
		void update_scale();

	public:
		static Owner<Sprite> create(SpriteBuilder const& builder = {});
	};
}