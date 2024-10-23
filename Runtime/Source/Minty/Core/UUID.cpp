#include "pch.h"
#include "UUID.h"

#include "Minty/Core/Encoding.h"
#include <random>

using namespace Minty;

static std::random_device randomDevice;
static std::mt19937_64 randomEngine(randomDevice());
static std::uniform_int_distribution<uint64_t> uniformDistribution;

Bool Minty::UUID::operator==(UUID const other) const
{
	return _uuid == other._uuid;
}

Bool Minty::UUID::operator!=(UUID const other) const
{
	return _uuid != other._uuid;
}

Bool Minty::UUID::operator==(uint64_t const other) const
{
	return _uuid == other;
}

Bool Minty::UUID::operator!=(uint64_t const other) const
{
	return _uuid != other;
}

UUID Minty::UUID::create()
{
	return UUID(uniformDistribution(randomEngine));
}

std::ostream& Minty::operator<<(std::ostream& stream, UUID const& object)
{
	stream << std::hex << object._uuid << std::dec;
	return stream;
}

std::istream& Minty::operator>>(std::istream& stream, UUID& object)
{
	stream >> std::hex >> object._uuid >> std::dec;
	return stream;
}
