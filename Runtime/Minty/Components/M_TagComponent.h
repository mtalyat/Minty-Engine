#pragma once
#include "Minty/Components/M_Component.h"

namespace Minty
{
	/// <summary>
	/// Holds the Entity's tag.
	/// </summary>
	struct TagComponent
		: public Component
	{
		String tag;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}