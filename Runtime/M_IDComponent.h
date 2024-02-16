#pragma once
#include "M_Component.h"

#include "M_UUID.h"

namespace minty
{
	struct IDComponent
		: Component
	{
		UUID id;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}