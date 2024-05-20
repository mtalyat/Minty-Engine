#include "pch.h"
#include "types/M_UUID.h"

#include "tools/M_Encoding.h"
#include <random>

using namespace minty;

static std::random_device randomDevice;
static std::mt19937_64 randomEngine(randomDevice());
static std::uniform_int_distribution<uint64_t> uniformDistribution;

minty::UUID::UUID()
	: _uuid(uniformDistribution(randomEngine))
{}

minty::UUID::UUID(uint64_t const uuid)
	: _uuid(uuid)
{}

bool minty::UUID::valid() const
{
	return _uuid > 0;
}

uint64_t minty::UUID::data() const
{
	return _uuid;
}

bool minty::UUID::operator==(UUID const other) const
{
	return _uuid == other._uuid;
}

bool minty::UUID::operator!=(UUID const other) const
{
	return _uuid != other._uuid;
}

bool minty::UUID::operator==(uint64_t const other) const
{
	return _uuid == other;
}

bool minty::UUID::operator!=(uint64_t const other) const
{
	return _uuid != other;
}

bool minty::UUID::operator!() const
{
	return !_uuid;
}

std::ostream& minty::operator<<(std::ostream& stream, UUID const& object)
{
	stream << std::hex << object._uuid << std::dec;
	return stream;
}

std::istream& minty::operator>>(std::istream& stream, UUID& object)
{
	stream >> std::hex >> object._uuid >> std::dec;
	return stream;
}

String minty::to_string(UUID const value)
{
	std::stringstream stream;
	stream << std::hex << value._uuid;
	return stream.str();
}
