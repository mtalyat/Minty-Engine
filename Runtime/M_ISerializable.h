#pragma once

#include "M_Types.h"
#include "M_Writer.h"
#include "M_Reader.h"

namespace minty
{
	class ISerializable
	{
	public:
		virtual void serialize(Writer& writer) const = 0;

		virtual void deserialize(Reader const& reader) = 0;
	};
}