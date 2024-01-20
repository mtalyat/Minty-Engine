#pragma once

#include <climits>
#include <string>
#include <filesystem>
#include <type_traits>

namespace minty
{
	// basic types

	/// <summary>
	/// Shorthand for a unsigned char.
	/// </summary>
	typedef unsigned char Byte;
	/// <summary>
	/// Shorthand for a String.
	/// </summary>
	typedef std::string String;
	/// <summary>
	/// Shorthand for a Path.
	/// </summary>
	typedef std::filesystem::path Path;

	// ID types

	/// <summary>
	/// An identification number.
	/// </summary>
	typedef int ID;
	/// <summary>
	/// An invalid identification number.
	/// </summary>
	constexpr ID ERROR_ID = -1;
	/// <summary>
	/// The highest possible identification number.
	/// </summary>
	constexpr ID MAX_ID = INT_MAX;

	template<typename DerivedClass, typename BaseClass>
	bool is_type()
	{
		return static_cast<bool>(std::is_base_of<BaseClass, DerivedClass>::value);
	}

	template<typename Old, typename New>
	New* try_cast(Old* value)
	{
		if (!is_type<Old, New>()) return nullptr;

		return static_cast<New*>(static_cast<void*>(value));
	}

	template<typename Old, typename New>
	New const* try_cast(Old const* value)
	{
		if (!is_type<Old, New>()) return nullptr;

		return static_cast<New const*>(static_cast<void const*>(value));
	}
}