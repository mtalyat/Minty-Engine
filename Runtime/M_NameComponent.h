#pragma once

#include "M_Types.h"
#include "M_Component.h"

namespace minty
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
		std::string name;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::string to_string(NameComponent const& value);
	};
}