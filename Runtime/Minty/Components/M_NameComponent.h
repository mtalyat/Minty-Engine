#pragma once
#include "Minty/Components/M_Component.h"

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
		void deserialize(Reader const& reader) override;
	};
}