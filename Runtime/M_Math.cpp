#include "pch.h"
#include "M_Math.h"

#include <cmath>

float minty::clamp(float const value, float const minimum, float const maximum)
{
    return max(minimum, min(maximum, value));
}

float minty::lerp(float const left, float const right, float const t)
{
	return (right - left) * t + left;
}

int minty::lerp(int const left, int const right, float const t)
{
	return static_cast<int>(roundf((right - left) * t + left));
}

float minty::lerp_clamped(float const left, float const right, float const t)
{
	return ((right - left) * clamp(t, 0.0f, 1.0f) + left);
}

float minty::round(float const value)
{
	return roundf(value);
}

float minty::round(float const value, float const precision)
{
	return roundf(value / precision) * precision;
}

float minty::floor(float const value)
{
	return floorf(value);
}

float minty::ceil(float const value)
{
	return ceilf(value);
}

float minty::abs(float const value)
{
	return fabsf(value);
}

float minty::mod(float const value, float const mod)
{
	return value - floorf(value / mod) * mod;
}

float minty::deg2rad(float const degree)
{
	return degree * PI / 180.0f;
}

float minty::rad2deg(float const rad)
{
	return rad * 180.0f / PI;
}

float minty::sin(float const rad)
{
	return sinf(rad);
}

float minty::cos(float const rad)
{
	return cosf(rad);
}

float minty::tan(float const rad)
{
	return tanf(rad);
}

float minty::asin(float const rad)
{
	return asinf(rad);
}

float minty::acos(float const rad)
{
	return acosf(rad);
}

float minty::atan(float const rad)
{
	return atanf(rad);
}

float minty::atan2(float const dy, float const dx)
{
	return atan2f(dy, dx);
}

float minty::angle(float const dx, float const dy)
{
	return atan2f(dy, dx);
}

float minty::sqrt(float const value)
{
	return sqrtf(value);
}

float minty::pow(float const value, float const power)
{
	return powf(value, power);
}

float minty::magnitude(float const x, float const y)
{
	return sqrtf(x * x + y * y);
}

float minty::sign(float const value)
{
	return value < 0.0f ? -1.0f : 1.0f;
}

float minty::signz(float const value)
{
	return value < 0.0f ? -1.0f : (value > 0.0f ? 1.0f : 0.0f);
}
