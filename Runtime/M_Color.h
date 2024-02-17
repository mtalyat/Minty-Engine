#pragma once
#include "M_Object.h"

namespace minty
{
	/// <summary>
	/// Holds data for a RGBA Color.
	/// </summary>
	struct Color
		: public Object
	{
	public:
		typedef int color_t;
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
		Color();

		/// <summary>
		/// Creates a color using the given red, green, and blue values.
		/// </summary>
		/// <param name="r"></param>
		/// <param name="g"></param>
		/// <param name="b"></param>
		Color(Byte const r, Byte const g, Byte const b);

		/// <summary>
		/// Creates a color using the given red, green, blue and alpha values.
		/// </summary>
		/// <param name="r"></param>
		/// <param name="g"></param>
		/// <param name="b"></param>
		/// <param name="a"></param>
		Color(Byte const r, Byte const g, Byte const b, Byte const a);

		/// <summary>
		/// Creates a color using the given packed color value.
		/// </summary>
		/// <param name="color">The color value packed into a color_t.</param>
		Color(color_t const color);
		
		// copy constructor
		Color(Color const& color);

		// move constructor
		Color(Color&& color) noexcept;

		// cast to int
		operator color_t() const;

		// assignment operator
		Color& operator=(Color const& color);

		friend std::ostream& operator<<(std::ostream& stream, Color const& color);
		friend std::istream& operator>>(std::istream& stream, Color const& color);

		/// <summary>
		/// Gets the red value as a scale from 0.0f to 1.0f.
		/// </summary>
		/// <returns>A float representing the value.</returns>
		float rf() const;

		/// <summary>
		/// Gets the green value as a scale from 0.0f to 1.0f.
		/// </summary>
		/// <returns>A float representing the value.</returns>
		float gf() const;

		/// <summary>
		/// Gets the blue value as a scale from 0.0f to 1.0f.
		/// </summary>
		/// <returns>A float representing the value.</returns>
		float bf() const;

		/// <summary>
		/// Gets the alpha value as a scale from 0.0f to 1.0f.
		/// </summary>
		/// <returns>A float representing the value.</returns>
		float af() const;

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

		String toHex() const;

		static Color fromHex(String const& string);

		/// <summary>
		/// Lerps two colors using the red, green and blue. The alpha value is set to the left argument alpha.
		/// </summary>
		/// <param name="left">The left color.</param>
		/// <param name="right">The right color.</param>
		/// <param name="t">The percentage point between the colors.</param>
		/// <returns>A new lerped color.</returns>
		static Color lerp(Color const left, Color const right, float const t);

		/// <summary>
		/// Lerps two colors using the red, green, blue and alpha.
		/// </summary>
		/// <param name="left">The left color.</param>
		/// <param name="right">The right color.</param>
		/// <param name="t">The percentage point between the colors.</param>
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

	public:
		friend String to_string(Color const color);
	};
}

