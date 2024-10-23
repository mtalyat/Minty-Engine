#pragma once

#include <chrono>
#include <typeindex>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Minty
{
	// base types:

	using Bool = bool;

	using Char = char;
	using Byte = unsigned char;

	using Short = short;
	using UShort = unsigned short;

	using Int = int;
	using UInt = unsigned int;

	using Long = long;
	using ULong = unsigned long;

	using Size = unsigned long long;

	using Float = float;
	using Double = double;

	using TimePoint = std::chrono::steady_clock::time_point;
	using TimeElapsed = long long;

	using TypeID = std::type_index;
	using TypeInfo = std::type_info;
}