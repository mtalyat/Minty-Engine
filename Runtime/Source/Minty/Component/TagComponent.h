#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Core/String.h"

namespace Minty
{
	/// <summary>
	/// Holds the tag data for an Entity.
	/// </summary>
	struct TagComponent
		: public Component
	{
		/// <summary>
		/// The tag on the Entity.
		/// </summary>
		String tag = "";

		void serialize(Writer& writer) const override
		{
			writer.write("tag", tag);
		}

		void deserialize(Reader& reader) override
		{
			reader.read("tag", tag);
		}
	};
}
