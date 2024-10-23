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
	};

	// a slice of a texture
	class Sprite
		: public Asset
	{
	private:
		Ref<Texture> m_texture;
		Float2 m_offset;
		Float2 m_size;

	public:
		Sprite(SpriteBuilder const& builder)
			: Asset(builder.id)
			, m_texture(builder.texture)
			, m_offset(builder.offset)
			, m_size(builder.size)
		{}

		~Sprite() = default;

	public:
		Ref<Texture> get_texture() const { return m_texture; }

		Float2 get_offset() const { return m_offset; }

		Float2 get_size() const { return m_size; }

	public:
		AssetType get_type() const override { return AssetType::Sprite; }

	public:
		static Owner<Sprite> create(SpriteBuilder const& builder = {});
	};
}