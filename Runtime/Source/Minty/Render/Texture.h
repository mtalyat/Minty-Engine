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
	protected:
		Texture(UUID const id)
			: Asset(id)
		{}

	public:
		virtual ~Texture() = default;

	public:
		virtual Ref<Image> get_image() const = 0;

	public:
		AssetType get_type() const override { return AssetType::Texture; }

	public:
		static Owner<Texture> create(const TextureBuilder& builder = {});
	};
}