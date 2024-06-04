#include "pch.h"
#include "Minty/Types/M_Object.h"

#include "Minty/Tools/M_Console.h"

#include <typeinfo>

using namespace Minty;

std::ostream& Minty::operator<<(std::ostream& stream, Object const& object)
{
	return stream;
}

std::istream& Minty::operator>>(std::istream& stream, Object& object)
{
	return stream;
}

static String to_string(Object const& value)
{
	return std::format("Object({})", typeid(value).name());
}

void Minty::Object::serialize(Writer& writer) const
{
	MINTY_TODO_FORMAT("serialize() for {}.", to_string(*this));
}

void Minty::Object::deserialize(Reader const& reader)
{
	MINTY_TODO_FORMAT("deserialize() for {}.", to_string(*this));
}
