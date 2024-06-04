#include "pch.h"
#include "Minty/Types/M_Time.h"

using namespace Minty;

TimePoint Minty::Time::now()
{
	return std::chrono::steady_clock::now();
}

float Minty::Time::calculate_duration_seconds(TimePoint const start, TimePoint const end)
{
	size_t frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

	// change from nanoseconds to seconds
	return frameTime / 1000000000.0f;
}

String Minty::Time::timestamp()
{
	auto now = std::chrono::system_clock::now();
	auto current_time = std::chrono::zoned_time(std::chrono::current_zone(), now);
	return std::format("{:%Y-%m-%d %H:%M:%S}", current_time);
}
