#pragma once
#include "Minty/Types/M_Object.h"

namespace Minty
{
	/// <summary>
	/// The base class for all Components.
	/// </summary>
	struct Component
		: public Object
	{
		virtual void serialize(Writer& writer) const override {}
		virtual void deserialize(Reader const& reader) override {}
	};
}