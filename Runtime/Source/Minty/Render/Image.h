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

	enum class ImageLayout
	{
		Undefined = 0,
		General = 1,
		ColorAttachmentOptimal = 2,
		DepthStencilAttachmentOptimal = 3,
		DepthStencilReadOnlyOptimal = 4,
		ShaderReadOnlyOptimal = 5,
		TransferSrcOptimal = 6,
		TransferDstOptimal = 7,
		Preinitialized = 8,
		DepthReadOnlyStencilAttachmentOptimal = 1000117000,
		DepthAttachmentStencilReadOnlyOptimal = 1000117001,
		DepthAttachmentOptimal = 1000241000,
		DepthReadOnlyOptimal = 1000241001,
		StencilAttachmentOptimal = 1000241002,
		StencilReadOnlyOptimal = 1000241003,
		ReadOnlyOptimal = 1000314000,
		AttachmentOptimal = 1000314001,
		PresentSrcKhr = 1000001002,
		VideoDecodeDstKhr = 1000024000,
		VideoDecodeSrcKhr = 1000024001,
		VideoDecodeDpbKhr = 1000024002,
		SharedPresentKhr = 1000111000,
		FragmentDensityMapOptimalExt = 1000218000,
		FragmentShadingRateAttachmentOptimalKhr = 1000164003,
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
		static Owner<Image> create(ImageBuilder const& builder = {});
	};
}