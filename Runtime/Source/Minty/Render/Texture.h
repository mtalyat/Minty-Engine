#pragma once
#include "Minty/Asset/Asset.h"

#include "Minty/Render/Image.h"

namespace Minty
{
	struct TextureBuilder
	{
		UUID id = {};
		Owner<Image> image;
	};

	class Texture
		: public Asset
	{
	private:
		Owner<Image> m_image;

	protected:
		Texture(TextureBuilder const& builder)
			: Asset(builder.id)
			, m_image(builder.image)
		{}

	public:
		virtual ~Texture() = default;

	public:
		Ref<Image> get_image() const { return m_image.create_ref(); }

		UInt get_width() const { return m_image->get_width(); }

		UInt get_height() const { return m_image->get_height(); }

	public:
		AssetType get_type() const override { return AssetType::Texture; }

	public:
		static Owner<Texture> create(TextureBuilder const& builder = {});
	};
}