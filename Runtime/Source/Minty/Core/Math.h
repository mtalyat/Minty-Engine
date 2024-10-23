#pragma once
#include "Minty/Core/Types.h"
#include "Minty/Core/String.h"
#include "Minty/Library/GLM.h"

namespace Minty::Math
{
	static Float const PI = 3.14159265359f;
	static Float const RAD2DEG = 180.0f / PI;
	static Float const DEG2RAD = PI / 180.0f;

	/// <summary>
	/// Clamps the given value between two other values.
	/// </summary>
	/// <param name="value">The value to clamp.</param>
	/// <param name="minimum">The minimum value.</param>
	/// <param name="maximum">The maximum value.</param>
	/// <returns>A value clamped within the range of [minimum, maximum].</returns>
	inline Float clamp(Float const value, Float const minimum, Float const maximum)
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
	inline Int clamp(Int const value, Int const minimum, Int const maximum)
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
	inline UInt clamp(UInt const value, UInt const minimum, UInt const maximum)
	{
		return std::max(minimum, std::min(maximum, value));
	}

	template<typename T>
	inline T min(T const left, T const right)
	{
		return left < right ? left : right;
	}

	template<typename T>
	inline T max(T const left, T const right)
	{
		return left > right ? left : right;
	}

	/// <summary>
	/// Gets the position between left and right, given the percentage t.
	/// </summary>
	/// <param name="left">The left positional value.</param>
	/// <param name="right">The right positional value.</param>
	/// <param name="t">The percentage between them.</param>
	/// <returns>The lerped value.</returns>
	inline Float lerp(Float const left, Float const right, Float const t)
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
	inline Int lerp(Int const left, Int const right, Float const t)
	{
		return static_cast<Int>(roundf((right - left) * t + left));
	}

	/// <summary>
	/// Gets the position between left and right, given the percentage t, clamped between 0.0 and 1.0.
	/// </summary>
	/// <param name="left">The left positional value.</param>
	/// <param name="right">The right positional value.</param>
	/// <param name="t">The percentage between them.</param>
	/// <returns>The lerped value.</returns>
	inline Float lerp_clamped(Float const left, Float const right, Float const t)
	{
		return ((right - left) * clamp(t, 0.0f, 1.0f) + left);
	}

	/// <summary>
	/// Rounds the value to the nearest integer.
	/// </summary>
	/// <param name="value">The value to round.</param>
	/// <returns>The rounded value.</returns>
	inline Float round(Float const value)
	{
		return roundf(value);
	}

	/// <summary>
	/// Rounds the value to the nearest value, using the given precision.
	/// </summary>
	/// <param name="value">The value to round.</param>
	/// <param name="precision">The precision to round to.</param>
	/// <returns>The rounded value.</returns>
	inline Float round(Float const value, Float const precision)
	{
		return roundf(value / precision) * precision;
	}

	/// <summary>
	/// Rounds the value to the nearest integer.
	/// </summary>
	/// <param name="value">The value to round.</param>
	/// <returns>The rounded value.</returns>
	inline Int round_to_int(Float const value)
	{
		return static_cast<Int>(roundf(value));
	}

	/// <summary>
	/// Rounds the value to the nearest value, using the given precision.
	/// </summary>
	/// <param name="value">The value to round.</param>
	/// <param name="precision">The precision to round to.</param>
	/// <returns>The rounded value.</returns>
	inline Int round_to_int(Float const value, Float const precision)
	{
		return static_cast<Int>(roundf(value / precision) * precision);
	}

	/// <summary>
	/// Rounds down to the nearest integer.
	/// </summary>
	/// <param name="value">The value to floor.</param>
	/// <returns>The floored value.</returns>
	inline Float floor(Float const value)
	{
		return floorf(value);
	}

	/// <summary>
	/// Rounds down to the nearest integer.
	/// </summary>
	/// <param name="value">The value to floor.</param>
	/// <returns>The floored value.</returns>
	inline Int floor_to_int(Float const value)
	{
		return static_cast<Int>(floorf(value));
	}

	/// <summary>
	/// Rounds up to the nearest integer.
	/// </summary>
	/// <param name="value">The value to ceil.</param>
	/// <returns>The ceiled value.</returns>
	inline Float ceil(Float const value)
	{
		return ceilf(value);
	}

	/// <summary>
	/// Rounds up to the nearest integer.
	/// </summary>
	/// <param name="value">The value to ceil.</param>
	/// <returns>The ceiled value.</returns>
	inline Int ceil_to_int(Float const value)
	{
		return static_cast<Int>(ceilf(value));
	}

	/// <summary>
	/// Gets the positive value of the given value.
	/// </summary>
	/// <param name="value">The value to get the absolute value of.</param>
	/// <returns>The absolute value.</returns>
	inline Float abs(Float const value)
	{
		return fabsf(value);
	}

	/// <summary>
	/// Gets the remainder of the given value, using mod as a dividend.
	/// </summary>
	/// <param name="value">The value to mod.</param>
	/// <param name="mod">The value to mod by.</param>
	/// <returns>The remainder.</returns>
	inline Float mod(Float const value, Float const mod)
	{
		return value - floorf(value / mod) * mod;
	}

	/// <summary>
	/// Converts the given degrees into radians.
	/// </summary>
	/// <param name="degree">The degrees to convert.</param>
	/// <returns>The degrees as radians.</returns>
	inline Float deg2rad(Float const degree)
	{
		return degree * DEG2RAD;
	}

	/// <summary>
	/// Converts the given radians into degrees.
	/// </summary>
	/// <param name="rad">The radians to convert.</param>
	/// <returns>The radians as degrees.</returns>
	inline Float rad2deg(Float const rad)
	{
		return rad * RAD2DEG;
	}

	/// <summary>
	/// Gets the sin value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline Float sin(Float const rad)
	{
		return sinf(rad);
	}

	/// <summary>
	/// Gets the cos value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline Float cos(Float const rad)
	{
		return cosf(rad);
	}

	/// <summary>
	/// Gets the tan value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline Float tan(Float const rad)
	{
		return tanf(rad);
	}

	/// <summary>
	/// Gets the asin value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline Float asin(Float const rad)
	{
		return asinf(rad);
	}

	/// <summary>
	/// Gets the acos value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline Float acos(Float const rad)
	{
		return acosf(rad);
	}

	/// <summary>
	/// Gets the atan value using the given radians.
	/// </summary>
	/// <param name="rad">The angle to use.</param>
	/// <returns>The value based on the given angle.</returns>
	inline Float atan(Float const rad)
	{
		return atanf(rad);
	}

	/// <summary>
	/// Gets the atan2 value using the given change in x and y.
	/// </summary>
	/// <param name="dy">The change in y.</param>
	/// <param name="dx">The change in x.</param>
	/// <returns>The angle based on the given change in x and y.</returns>
	inline Float atan2(Float const dy, Float const dx)
	{
		return atan2f(dy, dx);
	}

	/// <summary>
	/// Gets the angle to the dx and dy values from the origin.
	/// </summary>
	/// <param name="dx">The x position.</param>
	/// <param name="dy">The y position.</param>
	/// <returns>The angle based on the given x and y position.</returns>
	inline Float angle(Float const dx, Float const dy)
	{
		return atan2f(dy, dx);
	}

	/// <summary>
	/// Gets the square root of the given value.
	/// </summary>
	/// <param name="value">The value to square root.</param>
	/// <returns>The square rooted value.</returns>
	inline Float sqrt(Float const value)
	{
		return sqrtf(value);
	}

	/// <summary>
	/// Gets the power of the given value to the power of the given power.
	/// </summary>
	/// <param name="value">The value to raise.</param>
	/// <param name="power">The value to raise by.</param>
	/// <returns>The value raised to the power.</returns>
	inline Float pow(Float const value, Float const power)
	{
		return powf(value, power);
	}

	/// <summary>
	/// Gets the distance from the origin to the x and y position.
	/// </summary>
	/// <param name="x">The x position.</param>
	/// <param name="y">The y position.</param>
	/// <returns>The distance.</returns>
	inline Float magnitude(Float const x, Float const y)
	{
		return sqrtf(x * x + y * y);
	}

	/// <summary>
	/// Gets the sign of the given value. 1 for positive or zero, and -1 for negative.
	/// </summary>
	/// <param name="value">The value to get the sign of.</param>
	/// <returns>The sign of the value.</returns>
	inline Float sign(Float const value)
	{
		return value < 0.0f ? -1.0f : 1.0f;
	}

	/// <summary>
	/// Gets the sign of the given value. 1 for positive, 0 for zero, and -1 for negative.
	/// </summary>
	/// <param name="value">The value to get the sign of.</param>
	/// <returns>The sign of the value.</returns>
	inline Float signz(Float const value)
	{
		return value < 0.0f ? -1.0f : (value > 0.0f ? 1.0f : 0.0f);
	}

	/// <summary>
	/// Given a string expression, it is parsed and evaluated into a Float value.
	/// </summary>
	/// <param name="expression">The expression to parse.</param>
	/// <returns>The evaluated value.</returns>
	Float evaluate(String const& expression);
}

namespace Minty
{
	using Bool2 = glm::bvec2;
	using Bool3 = glm::bvec3;
	using Bool4 = glm::bvec4;

	using Float2 = glm::vec2;
	using Float3 = glm::vec3;
	using Float4 = glm::vec4;

	using Int2 = glm::ivec2;
	using Int3 = glm::ivec3;
	using Int4 = glm::ivec4;

	using UInt2 = glm::uvec2;
	using UInt3 = glm::uvec3;
	using UInt4 = glm::uvec4;

	using Quaternion = glm::quat;

	Float3 forward(Quaternion const& value);
	Float3 backward(Quaternion const& value);
	Float3 right(Quaternion const& value);
	Float3 left(Quaternion const& value);
	Float3 up(Quaternion const& value);
	Float3 down(Quaternion const& value);

	Float3 to_euler(Quaternion const& value);
	Quaternion from_euler(Float3 const& value);

	typedef glm::mat2 Matrix2;
	typedef glm::mat3 Matrix3;
	typedef glm::mat4 Matrix4;

	Float3 matrix4_get_position(Matrix4 const& value);
	Quaternion matrix4_get_rotation(Matrix4 const& value);
	Quaternion matrix4_get_rotation(Matrix4 const& value, Float3 const scale);
	Float3 matrix4_get_scale(Matrix4 const& value);
	void matrix4_set_position(Matrix4& value, Float3 const position);
	void matrix4_set_rotation(Matrix4& value, Quaternion const position);
	void matrix4_set_scale(Matrix4& value, Float3 const position);
}
