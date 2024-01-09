#pragma once

#include "M_Base.h"
#include "M_Node.h"
#include "M_Dynamic.h"

#include <filesystem>
#include <array>

namespace minty::Asset
{
	constexpr char const* META_EXTENSION = ".meta";

#ifdef N_DEBUG
	Path const BASE_PATH = Path("");
#else
	Path const BASE_PATH = Path("Assets/");
#endif

	/// <summary>
	/// Checks if the asset at the given path exists.
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool exists(Path const& path);

	/// <summary>
	/// Checks if the meta file for the asset at the given path exists.
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool exists_meta(Path const& path);

	/// <summary>
	/// Returns the absolute path of the given asset path.
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	Path absolute(Path const& path);

	/// <summary>
	/// Loads the given asset file into a Node.
	/// </summary>
	/// <param name="path">The path to the asset, relative to the project's Assets folder.</param>
	/// <returns>A node with the file contents.</returns>
	Node load_node(Path const& path);

	/// <summary>
	/// Loads the given asset file's meta into a Node.
	/// </summary>
	/// <param name="path">The path to the asset, relative to the project's Assets folder.</param>
	/// <returns>A node with the meta file contents.</returns>
	Node load_meta(Path const& path);

	/// <summary>
	/// Loads the given asset file into a vector of chars.
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	std::vector<char> load_chars(Path const& path);

	/// <summary>
	/// Loads the given asset file into a string.
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	String load_text(Path const& path);

	/// <summary>
	/// Loads the given asset file into a vector of line strings.
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	std::vector<String> load_lines(Path const& path);
}