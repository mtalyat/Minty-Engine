#pragma once

#include "M_Object.h"

namespace minty
{
	/// <summary>
	/// The base class for all Components.
	/// </summary>
	struct Component
		: public Object
	{
	public:
		virtual ~Component() {}

		virtual void serialize(Writer& writer) const override {}
		virtual void deserialize(Reader const& reader) override {}
	};
}