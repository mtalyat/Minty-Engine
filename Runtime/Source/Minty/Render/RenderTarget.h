#pragma once

#include "Minty/Core/Math.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Render/Image.h"

namespace Minty
{
	class RenderPass;

	struct RenderTargetBuilder
	{
		Ref<RenderPass> renderPass = nullptr;
		std::vector<Ref<Image>> images;
	};

	/// <summary>
	/// A target for rendering.
	/// </summary>
	class RenderTarget
	{
	private:
		Ref<RenderPass> m_renderPass;

	protected:
		RenderTarget(RenderTargetBuilder const& builder)
			: m_renderPass(builder.renderPass)
		{}

	public:
		virtual ~RenderTarget()
		{}

	public:
		Ref<RenderPass> get_render_pass() const { return m_renderPass; }

		virtual void reinitialize(RenderTargetBuilder const& builder) = 0;

		virtual UInt2 get_size() const = 0;

	public:
		static Owner<RenderTarget> create(RenderTargetBuilder const& builder = {});
	};
}