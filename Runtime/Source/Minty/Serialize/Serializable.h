#pragma once

#include "Minty/Serialize/Reader.h"
#include "Minty/Serialize/Writer.h"

namespace Minty
{
	class Serializable
	{
	public:
		/// <summary>
		/// Emplace object's data to the writer.
		/// </summary>
		/// <param name="writer"></param>
		virtual void serialize(Writer& writer) const = 0;

		/// <summary>
		/// Take object's data from the reader.
		/// </summary>
		/// <param name="reader"></param>
		virtual void deserialize(Reader& reader) = 0;
	};
}