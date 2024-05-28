#pragma once
#include "Minty/Types/M_Types.h"

namespace Minty::Math
{
	static float const PI = 3.14159265359f;
	static float const RAD2DEG = 180.0f / PI;
	static float const DEG2RAD = PI / 180.0f;

	/// <summary>
	/// Clamps the given value between two other values.
	/// </summary>
	/// <param name="value">The value to clamp.</param>
	/// <param name="minimum">The minimum value.</param>
	/// <param name="maximum">The maximum value.</param>
	/// <returns>A value clamped within the range of [minimum, maximum].</returns>
	inline float clamp(float const value, float const minimum, float const maximum)
	{
		return std::max(minimum, std::min(maximum, value));
	}

	/// <summary>
	/// Clamps the given value between the other two values.
	/// </summary>
	/// <param name="value">The value to clamp.</param>
	/// <param name="minimum">The minimum value.</param>
	/// <param name="maximum">The maximum value.</param>
	/// <returns>A value clamped within the range of [minimum, maximum].</returns>
	inline int clamp(int const value, int const minimum, int const maximum)
	{
		return std::max(minimum, std::min(maximum, value));
	}

	/// <summary>
	/// Gets the position between left and right, given the percentage t.
	/// </summary>
	/// <param name="left">The left positional value.</param>
	/// <param name="right">The right positional value.</param>
	/// <param name="t">The percentage between them.</param>
	/// <returns>The lerped value.</returns>
	inline float lerp(float const left, float const right, float const t)
	{
		return (right - left) * t + left;
	}

	/// <summary>
	/// Gets the position between left and right, given the percentage t.
	/// </summary>
	/// <param name="left">The left positional value.</param>
	/// <param name="right">The right positional value.</param>
	/// <param name="t">The percentage between them.</param>
	/// <returns>The lerped value.</returns>
	inline int lerp(int const left, int const right, float const t)
	{
		return static_cast<int>(roundf((right - left) * t + left));
	}

	/// <summary>
	/// Gets the position between left and right, given the percentage t, clamped between 0.0 and 1.0.
	/// </summary>
	/// <param name="left">The left positional value.</param>
	/// <param name="right">The right positional value.</param>
	/// <param name="t">The percentage between them.</param>
	/// <returns>The lerped value.</returns>
	inline float lerp_clamped(float const left, float const right, float const t)
	{
		return ((right - left) * clamp(t, 0.0f, 1.0f) + left);
	}

	/// <summary>
	/// Rounds the value to the nearest integer.
	/// </summary>
	/// <param name="value">The value to round.</param>
	/// <returns>The rounded value.</returns>
	inline float round(float const value)
	{
		return roundf(value);
	}

	/// <summary>
	/// Rounds the value to the nearest value, using the given precision.
	/// </summary>
	/// <param name="value">The value to round.</param>
	/// <param name="precision">The precision to round to.</param>
	/// <returns>The rounded value.</returns>
	inline float round(float const value, float const precision)
	{
		return roundf(value / precision) * precision;
	}

	/// <summary>
	/// Rounds the value to the nearest integer.
	/// </summary>
	/// <param name="value">The value to round.</param>
	/// <returns>The rounded value.</returns>
	inline int round_to_int(float const value)
	{
		return static_cast<int>(roundf(value));
	}

	/// <summary>
	/// Rounds the value to the nearest value, using the given precision.
	/// </summary>
	/// <param name="value">The value to round.</param>
	/// <param name="precision">The precision to round to.</param>
	/// <returns>The rounded value.</returns>
	inline int round_to_int(float const value, float const precision)
	{
		return static_cast<int>(roundf(value / precision) * precision);
	}

	/// <summary>
	/// Rounds down to the nearest integer.
	/// </summary>
	/// <param name="value">The value to floor.</param>
	/// <returns>The floored value.</returns>
	inline float floor(float const value)
	{
		return floorf(value);
	}

	/// <summary>
	/// Rounds down to the nearest integer.
	/// </summary>
	/// <param name="value">The value to floor.</param>
	/// <returns>The floored value.</returns>
	inline int floor_to_int(float const value)
	{
		return static_cast<int>(floorf(value));
	}

	/// <summary>
	/// Rounds up to the nearest integer.
	/// </summary>
	/// <param name="value">The value to ceil.</param>
	/// <returns>The ceiled value.</returns>
	inline float ceil(float const value)
	{
		return ceilf(value);
	}

	/// <summary>
	/// Rounds up to the nearest integer.
	/// </summary>
	/// <param name="value">The value to ceil.</param>
	/// <returns>The ceiled value.</returns>
	inline int ceil_to_int(float const value)
	{
		return static_cast<int>(ceilf(value));
	}

	/// <summary>
	/// Gets the positive value of the given value.
	/// </summary>
	/// <param name="value">The value to get the absolute value of.</param>
	/// <returns>The absolute value.</returns>
	inline float abs(float const value)
	{
		return fabsf(value);
	}

	/// <summary>
	/// Gets the remainder of the given value, using mod as a dividend.
	/// </summary>
	/// <param name="value">The value to mod.</param>
	/// <param name="mod">The value to mod by.</param>
	/// <returns>The remainder.</returns>
	inline float mod(float const value, float const mod)
	{
		return value - floorf(value / mod) * mod;
	}

	/// <summary>
	/// Converts the given degrees into radians.
	/// </summary>
	/// <param name="degree">The degrees to convert.</param>
	/// <returns>The degrees as radians.</returns>
	inline float deg2rad(float const degree)
	{
		return degree * DEG2RAD;
	}

	/// <summary>
	/// Converts the given radians into degrees.
	/// </summary>
	/// <param name="rad">The radians to convert.</param>
	/// <returns>The radians as degrees.</returns>
	inline float rad2deg(float const rad)
	{
		return rad * RAD2DEG;
	}

	/// <summary>
	/// Gets the sin value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline float sin(float const rad)
	{
		return sinf(rad);
	}

	/// <summary>
	/// Gets the cos value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline float cos(float const rad)
	{
		return cosf(rad);
	}

	/// <summary>
	/// Gets the tan value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline float tan(float const rad)
	{
		return tanf(rad);
	}

	/// <summary>
	/// Gets the asin value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline float asin(float const rad)
	{
		return asinf(rad);
	}

	/// <summary>
	/// Gets the acos value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline float acos(float const rad)
	{
		return acosf(rad);
	}

	/// <summary>
	/// Gets the atan value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline float atan(float const rad)
	{
		return atanf(rad);
	}

	/// <summary>
	/// Gets the atan2 value using the given change in x and y.
	/// </summary>
	/// <param name="dy">The change in y.</param>
	/// <param name="dx">The change in x.</param>
	/// <returns>The angle based on the given change in x and y.</returns>
	inline float atan2(float const dy, float const dx)
	{
		return atan2f(dy, dx);
	}

	/// <summary>
	/// Gets the angle to the dx and dy values from the origin.
	/// </summary>
	/// <param name="dx">The x position.</param>
	/// <param name="dy">The y position.</param>
	/// <returns>The angle based on the given x and y position.</returns>
	inline float angle(float const dx, float const dy)
	{
		return atan2f(dy, dx);
	}

	/// <summary>
	/// Gets the square root of the given value.
	/// </summary>
	/// <param name="value">The value to square root.</param>
	/// <returns>The square rooted value.</returns>
	inline float sqrt(float const value)
	{
		return sqrtf(value);
	}

	/// <summary>
	/// Gets the power of the given value to the power of the given power.
	/// </summary>
	/// <param name="value">The value to raise.</param>
	/// <param name="power">The value to raise by.</param>
	/// <returns>The value raised to the power.</returns>
	inline float pow(float const value, float const power)
	{
		return powf(value, power);
	}

	/// <summary>
	/// Gets the distance from the origin to the x and y position.
	/// </summary>
	/// <param name="x">The x position.</param>
	/// <param name="y">The y position.</param>
	/// <returns>The distance.</returns>
	inline float magnitude(float const x, float const y)
	{
		return sqrtf(x * x + y * y);
	}

	/// <summary>
	/// Gets the sign of the given value. 1 for positive or zero, and -1 for negative.
	/// </summary>
	/// <param name="value">The value to get the sign of.</param>
	/// <returns>The sign of the value.</returns>
	inline float sign(float const value)
	{
		return value < 0.0f ? -1.0f : 1.0f;
	}

	/// <summary>
	/// Gets the sign of the given value. 1 for positive, 0 for zero, and -1 for negative.
	/// </summary>
	/// <param name="value">The value to get the sign of.</param>
	/// <returns>The sign of the value.</returns>
	inline float signz(float const value)
	{
		return value < 0.0f ? -1.0f : (value > 0.0f ? 1.0f : 0.0f);
	}

	/// <summary>
	/// Given a string expression, it is parsed and evaluated into a float value.
	/// </summary>
	/// <param name="expression">The expression to parse.</param>
	/// <returns>The evaluated value.</returns>
	float evaluate(String const& expression);
}
