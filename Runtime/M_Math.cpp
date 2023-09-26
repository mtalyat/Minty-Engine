#include "pch.h"
#include "M_Math.h"

#include <cmath>

float minty::math::clamp(float const value, float const minimum, float const maximum)
{
    return max(minimum, min(maximum, value));
}

float minty::math::lerp(float const left, float const right, float const t)
{
	return (right - left) * t + left;
}

int minty::math::lerp(int const left, int const right, float const t)
{
	return static_cast<int>(roundf((right - left) * t + left));
}

float minty::math::lerp_clamped(float const left, float const right, float const t)
{
	return ((right - left) * clamp(t, 0.0f, 1.0f) + left);
}

float minty::math::round(float const value)
{
	return roundf(value);
}

float minty::math::round(float const value, float const precision)
{
	return roundf(value / precision) * precision;
}

int minty::math::round_to_int(float const value)
{
	return static_cast<int>(roundf(value));
}

int minty::math::round_to_int(float const value, float const precision)
{
	return static_cast<int>(roundf(value / precision) * precision);
}

float minty::math::floor(float const value)
{
	return floorf(value);
}

int minty::math::floor_to_int(float const value)
{
	return static_cast<int>(floorf(value));
}

float minty::math::ceil(float const value)
{
	return ceilf(value);
}

int minty::math::ceil_to_int(float const value)
{
	return static_cast<int>(ceilf(value));
}

float minty::math::abs(float const value)
{
	return fabsf(value);
}

float minty::math::mod(float const value, float const mod)
{
	return value - floorf(value / mod) * mod;
}

float minty::math::deg2rad(float const degree)
{
	return degree * PI / 180.0f;
}

float minty::math::rad2deg(float const rad)
{
	return rad * 180.0f / PI;
}

float minty::math::sin(float const rad)
{
	return sinf(rad);
}

float minty::math::cos(float const rad)
{
	return cosf(rad);
}

float minty::math::tan(float const rad)
{
	return tanf(rad);
}

float minty::math::asin(float const rad)
{
	return asinf(rad);
}

float minty::math::acos(float const rad)
{
	return acosf(rad);
}

float minty::math::atan(float const rad)
{
	return atanf(rad);
}

float minty::math::atan2(float const dy, float const dx)
{
	return atan2f(dy, dx);
}

float minty::math::angle(float const dx, float const dy)
{
	return atan2f(dy, dx);
}

float minty::math::sqrt(float const value)
{
	return sqrtf(value);
}

float minty::math::pow(float const value, float const power)
{
	return powf(value, power);
}

float minty::math::magnitude(float const x, float const y)
{
	return sqrtf(x * x + y * y);
}

float minty::math::sign(float const value)
{
	return value < 0.0f ? -1.0f : 1.0f;
}

float minty::math::signz(float const value)
{
	return value < 0.0f ? -1.0f : (value > 0.0f ? 1.0f : 0.0f);
}
