#pragma once

#include "Minty/Component/Component.h"
#include "Minty/Core/Types.h"

namespace Minty
{
	/// <summary>
	/// Holds the layer data for an Entity.
	/// </summary>
	struct LayerComponent
		: public Component
	{
		Layer layer;

		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}