#pragma once

#include "Minty/Core/M_Base.h"
#include "Minty/Serialization/M_ISerializable.h"

namespace Minty
{
	/// <summary>
	/// The base type for most Minty classes.
	/// </summary>
	class Object
		: public ISerializable
	{
	public:
		virtual ~Object() = default;

		virtual void serialize(Writer& writer) const override;
		virtual void deserialize(Reader const& reader) override;

		friend std::ostream& operator<<(std::ostream& stream, Object const& object);
		friend std::istream& operator>>(std::istream& stream, Object& object);
	};
}

