#pragma once

#include "Minty/Types/M_Types.h"

namespace Minty
{
	class Writer;
	class Reader;

	class ISerializable
	{
	public:
		virtual ~ISerializable() {}

		/// <summary>
		/// Serializes this class data into a Node.
		/// </summary>
		/// <param name="writer"></param>
		virtual void serialize(Writer& writer) const = 0;

		/// <summary>
		/// Deserializes this class data from a Node.
		/// </summary>
		/// <param name="reader"></param>
		virtual void deserialize(Reader const& reader) = 0;
	};
}