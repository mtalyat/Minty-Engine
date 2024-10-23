#pragma once
#include "Minty/Component/Component.h"

namespace Minty
{
	/// <summary>
	/// Holds the name of an Entity.
	/// </summary>
	struct NameComponent
		: public Component
	{
		/// <summary>
		/// The name of the Entity.
		/// </summary>
		String name = "";

		void serialize(Writer& writer) const override
		{
			writer.write("name", name);
		}

		void deserialize(Reader& reader) override
		{
			reader.read("name", name);
		}
	};
}
