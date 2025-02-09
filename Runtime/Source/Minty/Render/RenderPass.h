#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/Types.h"
#include "Minty/Render/RenderAttachment.h"
#include "Minty/Render/RenderTarget.h"

namespace Minty
{
	class RenderTarget;

	struct RenderPassBuilder
	{
		UUID id = INVALID_UUID;
		RenderAttachment const* colorAttachment = nullptr;
		RenderAttachment const* depthAttachment = nullptr;
	};

	class RenderPass
		: public Asset
	{
	private:
		std::vector<Ref<RenderTarget>> m_targets;
		Ref<RenderTarget> m_activeTarget;
		Bool m_useColorAttachment;
		Bool m_useDepthAttachment;

	protected:
		RenderPass(RenderPassBuilder const& builder)
			: Asset(builder.id)
			, m_targets()
			, m_activeTarget(nullptr)
			, m_useColorAttachment(static_cast<Bool>(builder.colorAttachment))
			, m_useDepthAttachment(static_cast<Bool>(builder.depthAttachment))
		{}

	public:
		virtual ~RenderPass();

	public:
		Ref<RenderTarget> create_render_target(RenderTargetBuilder const& builder);

		void destroy_render_target(Ref<RenderTarget> const& target);

		void reinitialize_render_targets(RenderTargetBuilder const& builder);

		void set_active_target(Ref<RenderTarget> const target) { m_activeTarget = target; }

		Ref<RenderTarget> get_active_target() { return m_activeTarget; }

		Bool using_color_attachment() const { return m_useColorAttachment; }

		Bool using_depth_attachment() const { return m_useDepthAttachment; }

		virtual void* get_native() const = 0;

	public:
		AssetType get_asset_type() const override { return AssetType::RenderPass; }

	public:
		static Owner<RenderPass> create(RenderPassBuilder const& builder);
	};
}