#pragma once

#include "M_Node.h"
#include <string>
#include <vector>

namespace minty
{
	namespace file
	{
		std::vector<char> read_all_chars(std::string const& path);

		/// <summary>
		/// Reads all of the text from the file at the given path.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		/// <returns>The text from the file.</returns>
		std::string read_all_text(std::string const& path);

		/// <summary>
		/// Reads all of the lines from the file at the given path.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		/// <returns>The lines from the file.</returns>
		std::vector<std::string> read_all_lines(std::string const& path);

		/// <summary>
		/// Parses a Node from the file at the given path.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		/// <returns>A Node with the parsed data.</returns>
		Node read_node(std::string const& path);

		/// <summary>
		/// Checks if the given path exists.
		/// </summary>
		/// <param name="path">The path to check.</param>
		/// <returns>True if the path exists.</returns>
		bool exists(std::string const& path);

		std::string name(std::string const& path);

		std::string extension(std::string const& path);

		std::string absolute(std::string const& path);

		/// <summary>
		/// Returns the name of the file, relative to the current working directory.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		std::string relative_name(std::string const& path);

		/// <summary>
		/// Writes all of the given text to the file at the given path.
		/// </summary>
		/// <param name="path">The location of the file.</param>
		/// <param name="text">The text to write.</param>
		/// <returns>True if the text was successfully written.</returns>
		bool write_all_text(std::string const& path, std::string const& text);

		/// <summary>
		/// Writes all of the given lines to the file at the given path.
		/// </summary>
		/// <param name="path">The location of the file.</param>
		/// <param name="lines">The text to write.</param>
		/// <returns>True if the lines were successfully written.</returns>
		bool write_all_lines(std::string const& path, std::vector<std::string> const& lines);
	}
}