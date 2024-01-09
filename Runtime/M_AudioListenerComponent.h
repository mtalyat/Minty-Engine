#pragma once

#include "M_Component.h"

namespace minty
{
	/// <summary>
	/// Holds data for an Entity that can hear audio in the Scene.
	/// </summary>
	struct AudioListenerComponent
		: public Component
	{
		void serialize(Writer& writer) const override {}
		void deserialize(Reader const& reader) override {}

		friend String to_string(AudioListenerComponent const& value);
	};
}