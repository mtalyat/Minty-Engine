#include "pch.h"
#include "M_UUID.h"

#include "M_Encoding.h"
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

bool minty::UUID::empty() const
{
	return _uuid == 0;
}

bool minty::UUID::valid() const
{
	return _uuid > 0;
}

bool minty::UUID::operator==(UUID const other) const
{
	return _uuid == other._uuid;
}

bool minty::UUID::operator!=(UUID const other) const
{
	return _uuid != other._uuid;
}

bool minty::UUID::operator!() const
{
	return !_uuid;
}

UUID minty::UUID::create_empty()
{
	return UUID(0);
}

std::ostream& minty::operator<<(std::ostream& stream, UUID const& object)
{
	stream << Encoding::encode_base64<uint64_t>(object._uuid);
	return stream;
}

std::istream& minty::operator>>(std::istream& stream, UUID& object)
{
	String text;
	stream >> text;
	object._uuid = Encoding::decode_base64<uint64_t>(text);
	return stream;
}

String minty::to_string(UUID const value)
{
	return Encoding::encode_base64(value._uuid);
}
