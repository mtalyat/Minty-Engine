#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Core/Math.h"

namespace Minty
{
	struct CanvasComponent
		: public Component
	{
		Int2 referenceResolution;

		virtual void serialize(Writer& writer) const override
		{
			writer.write("resolution", referenceResolution);
		}

		virtual void deserialize(Reader& reader) override
		{
			reader.read("resolution", referenceResolution);
		}
	};
}
