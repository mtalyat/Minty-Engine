#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Core/Math.h"
#include "Minty/Render/Texture.h"

namespace Minty
{
	struct SpriteBuilder
	{
		UUID id = {};
		Ref<Texture> texture = nullptr;
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
		Float2 m_offset;
		Float2 m_size;
		Float2 m_pivot;
		Float m_pixelsPerUnit;
		Float m_scale;

	public:
		Sprite(SpriteBuilder const& builder)
			: Asset(builder.id)
			, m_texture(builder.texture)
			, m_offset(builder.offset)
			, m_size(builder.size)
			, m_pivot(builder.pivot)
			, m_pixelsPerUnit()
			, m_scale()
		{
			set_pixels_per_unit(builder.pixelsPerUnit);
		}

		~Sprite() = default;

	public:
		Ref<Texture> get_texture() const { return m_texture; }

		Float2 get_offset() const { return m_offset; }

		Float2 get_size() const { return m_size; }

		Float2 get_pivot() const { return m_pivot; }

		Float get_pixels_per_unit() const { return m_pixelsPerUnit; }

		void set_pixels_per_unit(Float const ppu);

		Float get_scale() const { return m_scale; }

	public:
		AssetType get_type() const override { return AssetType::Sprite; }

	public:
		static Owner<Sprite> create(SpriteBuilder const& builder = {});
	};
}