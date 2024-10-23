#pragma once
#include "Minty/Core/Math.h"
#include "Minty/Core/Types.h"

namespace Minty
{
	/// <summary>
	/// The bit shift amount to get the alpha value in a color.
	/// </summary>
	constexpr Int COLOR_SHAMT_A = 24;
	/// <summary>
	/// The bit shift amount to get the red value in a color.
	/// </summary>
	constexpr Int COLOR_SHAMT_R = 0;
	/// <summary>
	/// The bit shift amount to get the green value in a color.
	/// </summary>
	constexpr Int COLOR_SHAMT_G = 8;
	/// <summary>
	/// The bit shift amount to get the blue value in a color.
	/// </summary>
	constexpr Int COLOR_SHAMT_B = 16;

	/// <summary>
	/// Holds data for a RGBA Color.
	/// </summary>
	struct Color
	{
	public:
		typedef Int Color_t;
		constexpr static Byte MAX_CHANNEL = 255;
		constexpr static Byte MIN_CHANNEL = 0;

	public:
		/// <summary>
		/// The red component.
		/// </summary>
		Byte r;

		/// <summary>
		/// The green component.
		/// </summary>
		Byte g;

		/// <summary>
		/// The blue component.
		/// </summary>
		Byte b;

		/// <summary>
		/// The alpha component.
		/// </summary>
		Byte a;

	public:
		/// <summary>
		/// Creates an empty color.
		/// </summary>
		Color() = default;

		/// <summary>
		/// Creates a color using the given red, green, and blue values. [0, 256)
		/// </summary>
		/// <param name="r"></param>
		/// <param name="g"></param>
		/// <param name="b"></param>
		Color(Byte const r, Byte const g, Byte const b)
			: r(r)
			, g(g)
			, b(b)
			, a(255)
		{}

		/// <summary>
		/// Creates a color using the given red, green, blue and alpha values. [0, 256)
		/// </summary>
		/// <param name="r"></param>
		/// <param name="g"></param>
		/// <param name="b"></param>
		/// <param name="a"></param>
		Color(Byte const r, Byte const g, Byte const b, Byte const a)
			: r(r)
			, g(g)
			, b(b)
			, a(a)
		{}

		/// <summary>
		/// Creates a color using the given red, green, and blue values. [0, 256)
		/// </summary>
		/// <param name="r"></param>
		/// <param name="g"></param>
		/// <param name="b"></param>
		Color(Int const r, Int const g, Int const b)
			: Color(static_cast<Byte>(r), static_cast<Byte>(g), static_cast<Byte>(b))
		{}

		/// <summary>
		/// Creates a color using the given red, green, blue and alpha values. [0, 256)
		/// </summary>
		/// <param name="r"></param>
		/// <param name="g"></param>
		/// <param name="b"></param>
		/// <param name="a"></param>
		Color(Int const r, Int const g, Int const b, Int const a)
			: Color(static_cast<Byte>(r), static_cast<Byte>(g), static_cast<Byte>(b), static_cast<Byte>(a))
		{}

		/// <summary>
		/// Creates a color using the given red, green and blue float values. [0.0, 1.0]
		/// </summary>
		/// <param name="rf"></param>
		/// <param name="gf"></param>
		/// <param name="bf"></param>
		Color(float const rf, float const gf, float const bf)
			: Color(static_cast<Byte>(Math::floor_to_int(rf * 255.0f)), static_cast<Byte>(Math::floor_to_int(gf * 255.0f)), static_cast<Byte>(Math::floor_to_int(bf * 255.0f)))
		{}

		/// <summary>
		/// Creates a color using the given red, green, blue and alpha float values. [0.0, 1.0]
		/// </summary>
		/// <param name="rf"></param>
		/// <param name="gf"></param>
		/// <param name="bf"></param>
		/// <param name="af"></param>
		Color(float const rf, float const gf, float const bf, float const af)
			: Color(static_cast<Byte>(Math::floor_to_int(rf * 255.0f)), static_cast<Byte>(Math::floor_to_int(gf * 255.0f)), static_cast<Byte>(Math::floor_to_int(bf * 255.0f)), static_cast<Byte>(Math::floor_to_int(af * 255.0f)))
		{}

		/// <summary>
		/// Creates a color using the given packed color value.
		/// </summary>
		/// <param name="color">The color value packed Into a Color_t.</param>
		Color(Color_t const color);

		// copy constructor
		Color(Color const& color);

		// move constructor
		Color(Color&& color) noexcept;

		// cast to Int
		operator Color_t() const
		{
			return (a << COLOR_SHAMT_A) | (r << COLOR_SHAMT_R) | (g << COLOR_SHAMT_G) | (b << COLOR_SHAMT_B);
		}

		// assignment operator
		Color& operator=(Color const& color);

		friend std::ostream& operator<<(std::ostream& stream, Color const& color);
		friend std::istream& operator>>(std::istream& stream, Color& color);

		/// <summary>
		/// Gets the red value as a scale from 0.0f to 1.0f.
		/// </summary>
		/// <returns>A float representing the value.</returns>
		float rf() const
		{
			return static_cast<float>(r) / 255.0f;
		}

		/// <summary>
		/// Gets the green value as a scale from 0.0f to 1.0f.
		/// </summary>
		/// <returns>A float representing the value.</returns>
		float gf() const
		{
			return static_cast<float>(g) / 255.0f;
		}

		/// <summary>
		/// Gets the blue value as a scale from 0.0f to 1.0f.
		/// </summary>
		/// <returns>A float representing the value.</returns>
		float bf() const
		{
			return static_cast<float>(b) / 255.0f;
		}

		/// <summary>
		/// Gets the alpha value as a scale from 0.0f to 1.0f.
		/// </summary>
		/// <returns>A float representing the value.</returns>
		float af() const
		{
			return static_cast<float>(a) / 255.0f;
		}

		/// <summary>
		/// Darkens the color by the given percentage.
		/// </summary>
		/// <param name="percent">The percent to darken the color by.</param>
		/// <returns>A new darkened color.</returns>
		Color darken(float const percent) const;

		/// <summary>
		/// Lightens the color by the given percentage.
		/// </summary>
		/// <param name="percent">The percentage to lighten the color by.</param>
		/// <returns>A new lightened color.</returns>
		Color lighten(float const percent) const;

		Float4 toFloat4() const;

		String toHex() const;

		static Color fromHex(String const& string);

		/// <summary>
		/// Lerps two colors using the red, green and blue. The alpha value is set to the left argument alpha.
		/// </summary>
		/// <param name="left">The left color.</param>
		/// <param name="right">The right color.</param>
		/// <param name="t">The percentage poInt between the colors.</param>
		/// <returns>A new lerped color.</returns>
		static Color lerp(Color const left, Color const right, float const t);

		/// <summary>
		/// Lerps two colors using the red, green, blue and alpha.
		/// </summary>
		/// <param name="left">The left color.</param>
		/// <param name="right">The right color.</param>
		/// <param name="t">The percentage poInt between the colors.</param>
		/// <returns>A new lerped color.</returns>
		static Color lerpa(Color const left, Color const right, float const t);

		/// <summary>
		/// Creates a Color that represents the color white.
		/// </summary>
		/// <returns>The Color.</returns>
		inline static Color white() { return Color(255, 255, 255, 255); }

		/// <summary>
		/// Creates a Color that represents the color black.
		/// </summary>
		/// <returns>The Color.</returns>
		inline static Color black() { return Color(0, 0, 0, 255); }

		/// <summary>
		/// Creates a Color that represents the color red.
		/// </summary>
		/// <returns>The Color.</returns>
		inline static Color red() { return Color(255, 0, 0, 255); }

		/// <summary>
		/// Creates a Color that represents the color green.
		/// </summary>
		/// <returns>The Color.</returns>
		inline static Color green() { return Color(0, 255, 0, 255); }

		/// <summary>
		/// Creates a Color that represents the color blue.
		/// </summary>
		/// <returns>The Color.</returns>
		inline static Color blue() { return Color(0, 0, 255, 255); }

		/// <summary>
		/// Creates a Color that represents the color yellow.
		/// </summary>
		/// <returns>The Color.</returns>
		inline static Color yellow() { return Color(255, 255, 0, 255); }

		/// <summary>
		/// Creates a Color that represents the color cyan.
		/// </summary>
		/// <returns>The Color.</returns>
		inline static Color cyan() { return Color(0, 255, 255, 255); }

		/// <summary>
		/// Creates a Color that represents the color magenta.
		/// </summary>
		/// <returns>The Color.</returns>
		inline static Color magenta() { return Color(255, 0, 255, 255); }

		/// <summary>
		/// Creates a Color that represents an error.
		/// </summary>
		/// <returns>The Color.</returns>
		inline static Color error() { return Color(255, 0, 255, 255); }
	};
}

