#pragma once
#include "types/M_Object.h"

namespace minty
{
	/// <summary>
	/// The base class for all Components.
	/// </summary>
	struct Component
		: public Object
	{
		UUID id;

	public:
		Component() {}
		Component(UUID const id)
			: id(id)
		{}

		virtual ~Component() {}

		virtual void serialize(Writer& writer) const override {}
		virtual void deserialize(Reader const& reader) override {}
	};
}