#include "pch.h"
#include "M_Color.h"

#include "M_Writer.h"
#include "M_Reader.h"
#include "M_Math.h"

using namespace minty;

/// <summary>
/// The bit shift amount to get the alpha value in a color.
/// </summary>
constexpr int COLOR_SHAMT_A = 24;
/// <summary>
/// The bit shift amount to get the red value in a color.
/// </summary>
constexpr int COLOR_SHAMT_R = 0;
/// <summary>
/// The bit shift amount to get the green value in a color.
/// </summary>
constexpr int COLOR_SHAMT_G = 8;
/// <summary>
/// The bit shift amount to get the blue value in a color.
/// </summary>
constexpr int COLOR_SHAMT_B = 16;

Color::Color()
	: r(0)
	, g(0)
	, b(0)
	, a(0)
{}

Color::Color(Byte const r, Byte const g, Byte const b)
	: r(r)
	, g(g)
	, b(b)
	, a(255)
{}

Color::Color(Byte const r, Byte const g, Byte const b, Byte const a)
	: r(r)
	, g(g)
	, b(b)
	, a(a)
{}

minty::Color::Color(int const r, int const g, int const b)
	: Color(static_cast<Byte>(r), static_cast<Byte>(g), static_cast<Byte>(b))
{}

minty::Color::Color(int const r, int const g, int const b, int const a)
	: Color(static_cast<Byte>(r), static_cast<Byte>(g), static_cast<Byte>(b), static_cast<Byte>(a))
{}

minty::Color::Color(float const rf, float const gf, float const bf)
	: Color(static_cast<Byte>(Math::floor_to_int(rf * 255.0f)), static_cast<Byte>(Math::floor_to_int(gf * 255.0f)), static_cast<Byte>(Math::floor_to_int(bf * 255.0f)))
{}

minty::Color::Color(float const rf, float const gf, float const bf, float const af)
	: Color(static_cast<Byte>(Math::floor_to_int(rf * 255.0f)), static_cast<Byte>(Math::floor_to_int(gf * 255.0f)), static_cast<Byte>(Math::floor_to_int(bf * 255.0f)), static_cast<Byte>(Math::floor_to_int(af * 255.0f)))
{}

Color::Color(color_t const color)
	: r((color >> COLOR_SHAMT_R) & 0xff)
	, g((color >> COLOR_SHAMT_G) & 0xff)
	, b((color >> COLOR_SHAMT_B) & 0xff)
	, a((color >> COLOR_SHAMT_A) & 0xff)
{}

Color::Color(Color const& color)
	: r(color.r)
	, g(color.g)
	, b(color.b)
	, a(color.a)
{}

Color::Color(Color&& color) noexcept
	: r(color.r)
	, g(color.g)
	, b(color.b)
	, a(color.a)
{
	color.a = 0;
	color.g = 0;
	color.b = 0;
	color.a = 0;
}

Color::operator color_t() const
{
	return (a << COLOR_SHAMT_A) | (r << COLOR_SHAMT_R) | (g << COLOR_SHAMT_G) | (b << COLOR_SHAMT_B);
}

Color& Color::operator=(const Color& color)
{
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;

	return *this;
}

void minty::Color::serialize(Writer& writer) const
{
	std::stringstream ss;

	ss << *this;

	writer.get_node().set_data(ss.str());
}

void minty::Color::deserialize(Reader const& reader)
{
	std::stringstream ss(reader.get_node().get_data());

	ss >> *this;
}

float minty::Color::rf() const
{
	return static_cast<float>(r) / 255.0f;
}

float minty::Color::gf() const
{
	return static_cast<float>(g) / 255.0f;
}

float minty::Color::bf() const
{
	return static_cast<float>(b) / 255.0f;
}

float minty::Color::af() const
{
	return static_cast<float>(a) / 255.0f;
}

inline Color Color::darken(float const percent) const
{
	return Color();
}

inline Color Color::lighten(float const percent) const
{
	return Color();
}

Vector4 minty::Color::toVector() const
{
	return Vector4(rf(), gf(), bf(), af());
}

String minty::Color::toHex() const
{
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

Color minty::Color::fromHex(String const& string)
{
	Color color;
	std::stringstream ss(string);
	ss >> color;
	return color;
}

Color Color::lerp(Color const left, Color const right, float const t)
{
	return Color(
		static_cast<Byte>(Math::lerp(left.r, right.r, t)),
		static_cast<Byte>(Math::lerp(left.g, right.g, t)),
		static_cast<Byte>(Math::lerp(left.b, right.b, t)),
		left.a);
}

Color Color::lerpa(Color const left, Color const right, float const t)
{
	return Color(
		static_cast<Byte>(Math::lerp(left.r, right.r, t)),
		static_cast<Byte>(Math::lerp(left.g, right.g, t)),
		static_cast<Byte>(Math::lerp(left.b, right.b, t)),
		static_cast<Byte>(Math::lerp(left.a, right.a, t)));
}

std::ostream& minty::operator<<(std::ostream& stream, Color const& color)
{
	// hex code
	uint32_t hex = ((color.r << 24) | (color.g << 16) | (color.b << 8) | color.a);
	stream << '#' << std::hex << std::setfill('0') << std::setw(8) << hex << std::dec;
	return stream;
}

std::istream& minty::operator>>(std::istream& stream, Color& color)
{
	// if first char is hex, discard it
	if (stream.peek() == '#')
	{
		char discard;
		stream >> discard;
	}

	// get each value
	uint32_t hex;
	stream >> std::hex >> std::setw(8) >> hex >> std::dec;
	color = Color(
		static_cast<Byte>((hex >> 24) & 0xff),
		static_cast<Byte>((hex >> 16) & 0xff),
		static_cast<Byte>((hex >> 8) & 0xff),
		static_cast<Byte>(hex & 0xff)
	);
	return stream;
}

String minty::to_string(Color const color)
{
	return std::format("Color({}, {}, {}, {})", color.r, color.g, color.b, color.a);
}
