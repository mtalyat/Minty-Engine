#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/Types.h"

namespace Minty
{
	struct ScissorBuilder
	{
		UUID id = {};

		Int x = 0;
		Int y = 0;
		UInt width = 0;
		UInt height = 0;
	};

	/// <summary>
	/// Controls the mask/cropping of the contents being rendered to a Surface.
	/// </summary>
	class Scissor
		: public Asset
	{
	protected:
		Scissor(ScissorBuilder const& builder)
			: Asset(builder.id)
		{}

	public:
		virtual ~Scissor() = default;

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
		AssetType get_type() const override { return AssetType::Scissor; }

	public:
		static Owner<Scissor> create(ScissorBuilder const& builder = {});
	};
}