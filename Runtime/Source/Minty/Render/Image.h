#pragma once

#include "Minty/Core/Pointer.h"
#include "Minty/Render/Format.h"

namespace Minty
{
	/// <summary>
	/// The amount of dimensions on the image.
	/// </summary>
	enum class ImageType
	{
		D1 = 0,
		D2 = 1,
		D3 = 2,
	};

	/// <summary>
	/// The format for how pixels are read.
	/// </summary>
	enum class ImagePixelFormat
	{
		/// <summary>
		/// No format.
		/// </summary>
		None = 0,
		/// <summary>
		/// Gray channel only.
		/// </summary>
		Gray = 1,
		/// <summary>
		/// Gray and alpha channels.
		/// </summary>
		GrayAlpha = 2,
		/// <summary>
		/// Red, green and blue channels.
		/// </summary>
		RedGreenBlue = 3,
		/// <summary>
		/// Red, green, blue and alpha channels.
		/// </summary>
		RedGreenBlueAlpha = 4,
	};

	enum class ImageTiling
	{
		Optimal = 0,
		Linear = 1,
	};

	enum class ImageAspect
	{
		Undefined = 0,
		Color = 1,
		Depth = 2
	};

	enum class ImageAddressMode
	{
		Repeat = 0,
		MirroredRepeat = 1,
		ClampToEdge = 2,
		ClampToBorder = 3,
		MirroredClampToEdge = 4,
	};

	enum class ImageUsage
	{
		Sampled,
		Storage,
		Color,
		DepthStencil
	};

	struct ImageBuilder
	{
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
	{
	protected:
		Image() = default;

	public:
		virtual ~Image() = default;

	public:
		virtual void set_pixels(const Byte* const data) = 0;

	public:
		virtual void* get_native() const = 0;

		virtual UInt get_width() const = 0;

		virtual UInt get_height() const = 0;

	public:
		static Owner<Image> create(const ImageBuilder& builder = {});
	};
}