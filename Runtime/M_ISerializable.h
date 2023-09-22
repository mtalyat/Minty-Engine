#pragma once

#include "M_Types.h"
#include "M_Serializer.h"

namespace minty
{
	class ISerializable
	{
	public:
		virtual void serialize(Serializer& serializer) const = 0;

		virtual void deserialize(Serializer const& serializer) = 0;
	};
}