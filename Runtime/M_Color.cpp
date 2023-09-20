#include "pch.h"
#include "M_Color.h"

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

Color::Color(byte const r, byte const g, byte const b)
	: r(r)
	, g(g)
	, b(b)
	, a(0)
{}

Color::Color(byte const r, byte const g, byte const b, byte const a)
	: r(r)
	, g(g)
	, b(b)
	, a(a)
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

inline Color::operator color_t() const
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

std::string const minty::Color::toString() const
{
	return std::format("Color({}, {}, {}, {})", r, g, b, a);
}

Color Color::lerp(Color const left, Color const right, float const t)
{
	return Color(
		static_cast<byte>(math::lerp(left.r, right.r, t)),
		static_cast<byte>(math::lerp(left.g, right.g, t)),
		static_cast<byte>(math::lerp(left.b, right.b, t)),
		left.a);
}

Color Color::lerpa(Color const left, Color const right, float const t)
{
	return Color(
		static_cast<byte>(math::lerp(left.r, right.r, t)),
		static_cast<byte>(math::lerp(left.g, right.g, t)),
		static_cast<byte>(math::lerp(left.b, right.b, t)),
		static_cast<byte>(math::lerp(left.a, right.a, t)));
}
