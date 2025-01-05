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

		void serialize(Writer& writer) const override;

		void deserialize(Reader& reader) override;
	};
}
