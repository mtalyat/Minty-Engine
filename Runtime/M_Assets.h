#pragma once

#include "M_Base.h"
#include "M_Node.h"
#include "M_Dynamic.h"

#include <filesystem>
#include <array>

namespace minty::assets
{
	constexpr char const* META_EXTENSION = ".meta";

#ifdef N_DEBUG
	std::filesystem::path const BASE_PATH = std::filesystem::path("");
#else
	std::filesystem::path const BASE_PATH = std::filesystem::path("Assets/");
#endif

	bool exists(std::string const& path);

	bool exists_meta(std::string const& path);

	std::string absolute(std::string const& path);

	/// <summary>
	/// Loads the given asset file into a Node.
	/// </summary>
	/// <param name="path">The path to the asset, relative to the project's Assets folder.</param>
	/// <returns>A node with the file contents.</returns>
	Node load_node(std::string const& path);

	/// <summary>
	/// Loads the given asset file's meta into a Node.
	/// </summary>
	/// <param name="path">The path to the asset, relative to the project's Assets folder.</param>
	/// <returns>A node with the meta file contents.</returns>
	Node load_meta(std::string const& path);

	std::vector<char> load_chars(std::string const& path);

	std::string load_text(std::string const& path);

	std::vector<std::string> load_lines(std::string const& path);
}