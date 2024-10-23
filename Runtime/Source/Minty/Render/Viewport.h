#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/Types.h"

namespace Minty
{
	struct ViewportBuilder
	{
		UUID id = {};

		Int x = 0;
		Int y = 0;
		UInt width = 0;
		UInt height = 0;
		Float minDepth = 0.0f;
		Float maxDepth = 1.0f;
	};

	/// <summary>
	/// Controls the location and stretch of the contents being rendered to a Surface.
	/// </summary>
	class Viewport
		: public Asset
	{
	protected:
		Viewport(ViewportBuilder const& builder)
			: Asset(builder.id)
		{}

	public:
		virtual ~Viewport() = default;

	public:
		AssetType get_type() const override { return AssetType::Viewport; }

	public:
		static Owner<Viewport> create(const ViewportBuilder& builder = {});
	};
}