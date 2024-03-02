#pragma once
#include "M_RuntimeObject.h"

#include "M_Node.h"
#include "M_Dynamic.h"
#include "M_UUID.h"

#include <filesystem>
#include <array>

namespace minty
{
#ifdef MINTY_RELEASE
	static Path const BASE_PATH = Path("");
#else
	static Path const BASE_PATH = Path("Assets/");
#endif

	class Asset
		: public RuntimeObject
	{
	private:
		UUID _id;
		Path _path;

	public:
		Asset();

		Asset(UUID const id, Path const& path, Runtime& runtime);

		virtual ~Asset();

		UUID get_id() const;

		Path const& get_path() const;

		virtual String get_name() const;

		friend bool operator==(Asset const& left, Asset const& right);
		friend bool operator!=(Asset const& left, Asset const& right);

#pragma region Static

		/// <summary>
		/// Checks the given asset path, and ensures it exists.
		/// </summary>
		/// <param name="path">The path</param>
		/// <returns>0 on success, 1 when the asset at the path does not exist, 2 when requiresMeta is true and the meta file does not exist.</returns>
		static int check(Path const& path, char const* const extension, bool const requiresMeta);

		/// <summary>
		/// Checks if the asset at the given path exists.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static bool exists(Path const& path);

		/// <summary>
		/// Checks if the meta file for the asset at the given path exists.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static bool exists_meta(Path const& path);

		/// <summary>
		/// Returns the absolute path of the given asset path.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static Path absolute(Path const& path);

		/// <summary>
		/// Loads the given asset file into a Node.
		/// </summary>
		/// <param name="path">The path to the asset, relative to the project's Assets folder.</param>
		/// <returns>A node with the file contents.</returns>
		static Node load_node(Path const& path);

		/// <summary>
		/// Loads the given asset file's meta into a Node.
		/// </summary>
		/// <param name="path">The path to the asset, relative to the project's Assets folder.</param>
		/// <returns>A node with the meta file contents.</returns>
		static Node load_meta(Path const& path);

		/// <summary>
		/// Loads the given asset file into a vector of chars.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static std::vector<char> load_chars(Path const& path);

		/// <summary>
		/// Loads the given asset file into a string.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static String load_text(Path const& path);

		/// <summary>
		/// Loads the given asset file into a vector of line strings.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static std::vector<String> load_lines(Path const& path);

#pragma endregion
	};
}