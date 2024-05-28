#include "pch.h"
#include "Minty/Types/M_UUID.h"

#include "Minty/Tools/M_Encoding.h"
#include <random>

using namespace Minty;

static std::random_device randomDevice;
static std::mt19937_64 randomEngine(randomDevice());
static std::uniform_int_distribution<uint64_t> uniformDistribution;

bool Minty::UUID::operator==(UUID const other) const
{
	return _uuid == other._uuid;
}

bool Minty::UUID::operator!=(UUID const other) const
{
	return _uuid != other._uuid;
}

bool Minty::UUID::operator==(uint64_t const other) const
{
	return _uuid == other;
}

bool Minty::UUID::operator!=(uint64_t const other) const
{
	return _uuid != other;
}

bool Minty::UUID::operator!() const
{
	return !_uuid;
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

String Minty::to_string(UUID const value)
{
	std::stringstream stream;
	stream << std::hex << value.data();
	return stream.str();
}
