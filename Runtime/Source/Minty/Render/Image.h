#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Render/Enums.h"
#include "Minty/Render/Enums.h"

namespace Minty
{
	struct ImageBuilder
	{
		UUID id = INVALID_UUID;
		Format format = Format::R8G8B8A8_SRGB;
		ImageType type = ImageType::D2;
		ImageTiling tiling = ImageTiling::Optimal;
		ImageAspect aspect = ImageAspect::Color;
		ImageUsage usage = ImageUsage::Sampled;
		std::vector<Byte> pixels;
		UInt width = 0;
		UInt height = 0;
		Bool immutable = false;
	};

	/// <summary>
	/// Represents an image on the GPU.
	/// </summary>
	class Image
		: public Asset
	{
	protected:
		Format m_format;
		ImageType m_type;
		ImageTiling m_tiling;
		ImageAspect m_aspect;
		ImageUsage m_usage;
		UInt m_width;
		UInt m_height;
		Bool m_immutable;

	protected:
		Image(ImageBuilder const& builder);

	public:
		virtual ~Image() = default;

	public:
		virtual void set_pixels(const Byte* const data) = 0;

		virtual void* get_native() const = 0;

		virtual void resize(UInt const width, UInt const height, Format const format) = 0;

		void resize(UInt const width, UInt const height) { resize(width, height, m_format); }

		inline Format get_format() const { return m_format; }

		inline ImageType get_type() const { return m_type; }

		inline ImageTiling get_tiling() const { return m_tiling; }

		inline ImageAspect get_aspect() const { return m_aspect; }

		inline ImageUsage get_usage() const { return m_usage; }

		inline UInt get_width() const { return m_width; }

		inline UInt get_height() const { return m_height; }

		inline Bool is_immutable() const { return m_immutable; }

	public:
		AssetType get_asset_type() const override { return AssetType::Image; }

	public:
		static Owner<Image> create(ImageBuilder const& builder = {});
	};
}