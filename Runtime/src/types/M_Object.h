#pragma once

#include "core/M_Base.h"
#include "serialization/M_ISerializable.h"

namespace minty
{
	/// <summary>
	/// The base type for most Minty classes.
	/// </summary>
	class Object
		: public ISerializable
	{
	public:
		virtual ~Object() {}

		virtual void serialize(Writer& writer) const override;
		virtual void deserialize(Reader const& reader) override;

		friend std::ostream& operator<<(std::ostream& stream, Object const& object);
		friend std::istream& operator>>(std::istream& stream, Object& object);
	public:
		friend String to_string(Object const& value);
	};
}

