#pragma once
#include "Minty/Assets/M_Asset.h"

#include "Minty/Types/M_Types.h"
#include <vector>

namespace Minty
{
	constexpr char const* const EMPTY = "";

	struct GenericAssetBuilder
	{
		UUID id;
		Path path;
		String text;
	};

	/// <summary>
	/// A generic text asset.
	/// </summary>
	class GenericAsset
		: public Asset
	{
	private:
		String _text;

	public:
		GenericAsset(GenericAssetBuilder const& builder)
			: Asset(builder.id, builder.path), _text(builder.text) {}

		String const& get_text() const { return _text; }
	};
}