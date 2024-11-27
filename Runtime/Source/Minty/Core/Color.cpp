#include "pch.h"
#include "Color.h"

using namespace Minty;

Minty::Color::Color(Color_t const color)
	: r((color >> COLOR_SHAMT_R) & 0xff)
	, g((color >> COLOR_SHAMT_G) & 0xff)
	, b((color >> COLOR_SHAMT_B) & 0xff)
	, a((color >> COLOR_SHAMT_A) & 0xff)
{}

Minty::Color::Color(Color const& color)
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

Color& Color::operator=(Color const& color)
{
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;

	return *this;
}

std::ostream& Minty::operator<<(std::ostream& stream, Color const& color)
{
	// hex code
	uint32_t hex = ((color.r << 24) | (color.g << 16) | (color.b << 8) | color.a);
	stream << '#' << std::hex << std::setfill('0') << std::setw(8) << hex << std::dec;
	return stream;
}

std::istream& Minty::operator>>(std::istream& stream, Color& color)
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

Color Color::darken(float const percent) const
{
	return Color(rf() * percent, gf() * percent, bf() * percent, af());
}

Color Color::lighten(float const percent) const
{
	float r = rf();
	float g = gf();
	float b = bf();
	return Color(r + (1.0f - r) * percent, g + (1.0f - g) * percent, b + (1.0f - b) * percent, af());
}

Float4 Minty::Color::toFloat4() const
{
	return Float4(rf(), gf(), bf(), af());
}

String Minty::Color::toHex() const
{
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

Color Minty::Color::fromHex(String const& string)
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