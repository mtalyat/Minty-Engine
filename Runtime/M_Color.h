#pragma once

#include "M_Types.h"
#include "M_Object.h"

namespace minty
{
	typedef int color_t;

	struct Color
		: public Object
	{
	public:
		/// <summary>
		/// The red component.
		/// </summary>
		byte r;
		
		/// <summary>
		/// The green component.
		/// </summary>
		byte g;

		/// <summary>
		/// The blue component.
		/// </summary>
		byte b;

		/// <summary>
		/// The alpha component.
		/// </summary>
		byte a;

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
		Color(byte const r, byte const g, byte const b);

		/// <summary>
		/// Creates a color using the given red, green, blue and alpha values.
		/// </summary>
		/// <param name="r"></param>
		/// <param name="g"></param>
		/// <param name="b"></param>
		/// <param name="a"></param>
		Color(byte const r, byte const g, byte const b, byte const a);

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
		Color& operator=(const Color& color);

		/// <summary>
		/// Darkens the color by the given percentage.
		/// </summary>
		/// <param name="percent">The percent to darken the color by.</param>
		/// <returns>A new darkened color.</returns>
		inline Color darken(float const percent) const;

		/// <summary>
		/// Lightens the color by the given percentage.
		/// </summary>
		/// <param name="percent">The percentage to lighten the color by.</param>
		/// <returns>A new lightened color.</returns>
		inline Color lighten(float const percent) const;

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
	};
}

