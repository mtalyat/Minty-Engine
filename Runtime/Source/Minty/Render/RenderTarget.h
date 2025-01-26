#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Render/Image.h"

namespace Minty
{
	class RenderPass;

	struct RenderTargetBuilder
	{
		UUID id = INVALID_UUID;
		Ref<RenderPass> renderPass = nullptr;
		std::vector<Ref<Image>> images;
	};

	/// <summary>
	/// A target for rendering.
	/// </summary>
	class RenderTarget
		: public Asset
	{
	private:
		Ref<RenderPass> m_renderPass;

	protected:
		RenderTarget(RenderTargetBuilder const& builder)
			: Asset(builder.id)
			, m_renderPass(builder.renderPass)
		{}

	public:
		virtual ~RenderTarget()
		{}

	public:
		Ref<RenderPass> get_render_pass() const { return m_renderPass; }

		virtual void reinitialize(RenderTargetBuilder const& builder) = 0;

		virtual UInt2 get_size() const = 0;

	public:
		AssetType get_asset_type() const override { return AssetType::RenderTarget; }

		static Owner<RenderTarget> create(RenderTargetBuilder const& builder = {});
	};
}