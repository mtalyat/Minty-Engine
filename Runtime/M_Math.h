#pragma once

namespace minty
{
	float const PI = 3.14159265359f;

	/// <summary>
	/// Clamps the given value between two other values.
	/// </summary>
	/// <param name="value">The value to clamp.</param>
	/// <param name="minimum">The minimum value.</param>
	/// <param name="maximum">The maximum value.</param>
	/// <returns>A value clamped between minimum and maximum.</returns>
	inline float clamp(float const value, float const minimum, float const maximum);

	/// <summary>
	/// Gets the position between left and right, given the percentage t.
	/// </summary>
	/// <param name="left">The left positional value.</param>
	/// <param name="right">The right positional value.</param>
	/// <param name="t">The percentage between them.</param>
	/// <returns>The lerped value.</returns>
	inline float lerp(float const left, float const right, float const t);

	/// <summary>
	/// Gets the position between left and right, given the percentage t.
	/// </summary>
	/// <param name="left">The left positional value.</param>
	/// <param name="right">The right positional value.</param>
	/// <param name="t">The percentage between them.</param>
	/// <returns>The lerped value.</returns>
	inline int lerp(int const left, int const right, float const t);

	/// <summary>
	/// Gets the position between left and right, given the percentage t, clamped between 0.0 and 1.0.
	/// </summary>
	/// <param name="left">The left positional value.</param>
	/// <param name="right">The right positional value.</param>
	/// <param name="t">The percentage between them.</param>
	/// <returns>The lerped value.</returns>
	inline float lerp_clamped(float const left, float const right, float const t);

	/// <summary>
	/// Rounds the value to the nearest integer.
	/// </summary>
	/// <param name="value">The value to round.</param>
	/// <returns>The rounded value.</returns>
	inline float round(float const value);

	/// <summary>
	/// Rounds the value to the nearest value, using the given precision.
	/// </summary>
	/// <param name="value">The value to round.</param>
	/// <param name="precision">The precision to round to.</param>
	/// <returns>The rounded value.</returns>
	inline float round(float const value, float const precision);

	/// <summary>
	/// Rounds the value to the nearest integer.
	/// </summary>
	/// <param name="value">The value to round.</param>
	/// <returns>The rounded value.</returns>
	inline int roundToInt(float const value);

	/// <summary>
	/// Rounds the value to the nearest value, using the given precision.
	/// </summary>
	/// <param name="value">The value to round.</param>
	/// <param name="precision">The precision to round to.</param>
	/// <returns>The rounded value.</returns>
	inline int roundToInt(float const value, float const precision);

	/// <summary>
	/// Rounds down to the nearest integer.
	/// </summary>
	/// <param name="value">The value to floor.</param>
	/// <returns>The floored value.</returns>
	inline float floor(float const value);

	/// <summary>
	/// Rounds down to the nearest integer.
	/// </summary>
	/// <param name="value">The value to floor.</param>
	/// <returns>The floored value.</returns>
	inline int floorToInt(float const value);

	/// <summary>
	/// Rounds up to the nearest integer.
	/// </summary>
	/// <param name="value">The value to ceil.</param>
	/// <returns>The ceiled value.</returns>
	inline float ceil(float const value);

	/// <summary>
	/// Rounds up to the nearest integer.
	/// </summary>
	/// <param name="value">The value to ceil.</param>
	/// <returns>The ceiled value.</returns>
	inline int ceilToInt(float const value);

	/// <summary>
	/// Gets the positive value of the given value.
	/// </summary>
	/// <param name="value">The value to get the absolute value of.</param>
	/// <returns>The absolute value.</returns>
	inline float abs(float const value);

	/// <summary>
	/// Gets the remainder of the given value, using mod as a dividend.
	/// </summary>
	/// <param name="value">The value to mod.</param>
	/// <param name="mod">The value to mod by.</param>
	/// <returns>The remainder.</returns>
	inline float mod(float const value, float const mod);

	/// <summary>
	/// Converts the given degrees into radians.
	/// </summary>
	/// <param name="degree">The degrees to convert.</param>
	/// <returns>The degrees as radians.</returns>
	inline float deg2rad(float const degree);

	/// <summary>
	/// Converts the given radians into degrees.
	/// </summary>
	/// <param name="rad">The radians to convert.</param>
	/// <returns>The radians as degrees.</returns>
	inline float rad2deg(float const rad);

	/// <summary>
	/// Gets the sin value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline float sin(float const rad);

	/// <summary>
	/// Gets the cos value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline float cos(float const rad);

	/// <summary>
	/// Gets the tan value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline float tan(float const rad);

	/// <summary>
	/// Gets the asin value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline float asin(float const rad);

	/// <summary>
	/// Gets the acos value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline float acos(float const rad);

	/// <summary>
	/// Gets the atan value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline float atan(float const rad);

	/// <summary>
	/// Gets the atan2 value using the given change in x and y.
	/// </summary>
	/// <param name="dy">The change in y.</param>
	/// <param name="dx">The change in x.</param>
	/// <returns>The angle based on the given change in x and y.</returns>
	inline float atan2(float const dy, float const dx);

	/// <summary>
	/// Gets the angle to the dx and dy values from the origin.
	/// </summary>
	/// <param name="dx">The x position.</param>
	/// <param name="dy">The y position.</param>
	/// <returns>The angle based on the given x and y position.</returns>
	inline float angle(float const dx, float const dy);

	/// <summary>
	/// Gets the square root of the given value.
	/// </summary>
	/// <param name="value">The value to square root.</param>
	/// <returns>The square rooted value.</returns>
	inline float sqrt(float const value);

	/// <summary>
	/// Gets the power of the given value to the power of the given power.
	/// </summary>
	/// <param name="value">The value to raise.</param>
	/// <param name="power">The value to raise by.</param>
	/// <returns>The value raised to the power.</returns>
	inline float pow(float const value, float const power);

	/// <summary>
	/// Gets the distance from the origin to the x and y position.
	/// </summary>
	/// <param name="x">The x position.</param>
	/// <param name="y">The y position.</param>
	/// <returns>The distance.</returns>
	inline float magnitude(float const x, float const y);

	/// <summary>
	/// Gets the sign of the given value. 1 for positive or zero, and -1 for negative.
	/// </summary>
	/// <param name="value">The value to get the sign of.</param>
	/// <returns>The sign of the value.</returns>
	inline float sign(float const value);

	/// <summary>
	/// Gets the sign of the given value. 1 for positive, 0 for zero, and -1 for negative.
	/// </summary>
	/// <param name="value">The value to get the sign of.</param>
	/// <returns>The sign of the value.</returns>
	inline float signz(float const value);
}
