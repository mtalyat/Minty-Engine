#pragma once

#include "M_Wrap.h"

namespace minty
{
	/// <summary>
	/// Handles multiple Wrap objects.
	/// </summary>
	class Wrapper
		: public Object
	{
	private:
		// all wraps
		std::vector<Wrap> _wraps;

		// open virtual files
		std::unordered_map<ID, std::pair<Wrap*, ID>> _files;

	public:
		/// <summary>
		/// Creates an empty Wrapper.
		/// </summary>
		Wrapper();

		/// <summary>
		/// Adds the given Wrap to this Wrapper.
		/// </summary>
		/// <param name="wrap"></param>
		void emplace(Wrap const& wrap);

		/// <summary>
		/// Adds the Wrap file at the given path to this Wrapper.
		/// </summary>
		/// <param name="path"></param>
		void emplace(Path const& path);

		/// <summary>
		/// Finds the Wrap file that contains the given Path.
		/// </summary>
		/// <param name="path">A path that the target Wrap file contains.</param>
		/// <returns>A pointer to the found Wrap file, or nullptr if no file was found.</returns>
		Wrap* find_by_path(Path const& path);

		/// <summary>
		/// Finds the Wrap file that contains the given Path.
		/// </summary>
		/// <param name="path">A path that the target Wrap file contains.</param>
		/// <returns>A pointer to the found Wrap file, or nullptr if no file was found.</returns>
		Wrap const* find_by_path(Path const& path) const;

		/// <summary>
		/// Finds the Wrap file with the given name.
		/// </summary>
		/// <param name="name">The name of the Wrap file.</param>
		/// <returns>A pointer to the found Wrap file, or nullptr if no file was found.</returns>
		Wrap* find_by_name(String const& name);

		/// <summary>
		/// Finds the Wrap file with the given name.
		/// </summary>
		/// <param name="name">The name of the Wrap file.</param>
		/// <returns>A pointer to the found Wrap file, or nullptr if no file was found.</returns>
		Wrap const* find_by_name(String const& name) const;

		/// <summary>
		/// Opens the file at the given path.
		/// </summary>
		/// <param name="path">The path to the file within the Wrapper.</param>
		/// <param name="file">The file object to use to open the file at the given path.</param>
		/// <returns>True if the file was opened, otherwise false.</returns>
		bool open(Path const& path, VirtualFile& file) const;
	};
}