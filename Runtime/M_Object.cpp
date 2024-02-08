#include "pch.h"
#include "M_Object.h"

#include "M_Console.h"

#include <typeinfo>

using namespace minty;

std::ostream& minty::operator<<(std::ostream& stream, Object const& object)
{
	return stream;
}

std::istream& minty::operator>>(std::istream& stream, Object& object)
{
	return stream;
}

String minty::to_string(Object const& value)
{
	return std::format("Object({})", typeid(value).name());
}

void minty::Object::serialize(Writer& writer) const
{
	Console::todo(std::format("serialize() for {}.", to_string(*this)));
}

void minty::Object::deserialize(Reader const& reader)
{
	Console::todo(std::format("deserialize() for {}.", to_string(*this)));
}
