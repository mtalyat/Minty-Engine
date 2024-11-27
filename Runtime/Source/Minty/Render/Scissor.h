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
		AssetType get_type() const override { return AssetType::Scissor; }

	public:
		static Owner<Scissor> create(ScissorBuilder const& builder = {});
	};
}