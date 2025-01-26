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

		Float x = 0;
		Float y = 0;
		Float width = 0;
		Float height = 0;
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
		virtual void set_x(Float const x) = 0;

		virtual void set_y(Float const y) = 0;

		virtual void set_width(Float const width) = 0;
		
		virtual void set_height(Float const height) = 0;

		inline void set_position(Float const x, Float const y)
		{
			set_x(x);
			set_y(y);
		}

		inline void set_size(Float const width, Float const height)
		{
			set_width(width);
			set_height(height);
		}

	public:
		AssetType get_asset_type() const override { return AssetType::Viewport; }

	public:
		static Owner<Viewport> create(ViewportBuilder const& builder = {});
	};
}