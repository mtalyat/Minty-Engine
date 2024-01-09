#pragma once

#include <climits>
#include <string>
#include <filesystem>

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
}