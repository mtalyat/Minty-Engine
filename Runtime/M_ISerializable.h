#pragma once

#include "M_Types.h"
#include "M_Writer.h"
#include "M_Reader.h"

namespace minty
{
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

		/*
		virtual void serialize(Writer& writer) const override;
		virtual void deserialize(Reader const& reader) override;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
		*/
	};
}