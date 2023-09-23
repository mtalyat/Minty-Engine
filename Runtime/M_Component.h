#pragma once

#include "M_Object.h"
#include "M_ISerializable.h"

namespace minty
{
	struct Component
		: public Object, public ISerializable
	{
		/// <summary>
		/// Disposes any resources connected to this Component.
		/// </summary>
		virtual void dispose() {}

		virtual void serialize(Writer& writer) const override {}

		virtual void deserialize(Reader const& reader) override {}
	};
}