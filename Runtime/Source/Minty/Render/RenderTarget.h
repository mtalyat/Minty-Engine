#pragma once

#include "Minty/Core/Math.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Render/Image.h"

namespace Minty
{
	struct RenderTargetBuilder
	{
		std::vector<Ref<Image>> images;
	};

	/// <summary>
	/// A target for rendering.
	/// </summary>
	class RenderTarget
	{
	protected:
		RenderTarget() = default;

	public:
		virtual ~RenderTarget() = default;

	public:
		virtual UInt2 get_size() const = 0;

	public:
		static Owner<RenderTarget> create(RenderTargetBuilder const& builder = {});
	};
}