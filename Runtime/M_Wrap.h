#pragma once

#include "M_Object.h"
#include "M_Register.h"
#include "M_Compression.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace minty
{
	class VirtualFile;
	class PhysicalFile;

	constexpr char const* const WRAP_MAGIC = "WRAP";
	constexpr int const WRAP_MAGIC_SIZE = 4;

	constexpr int const WRAP_HEADER_PATH_SIZE = 100;
	constexpr int const WRAP_HEADER_NAME_SIZE = 50;
	constexpr int const WRAP_ENTRY_PATH_SIZE = 255;
	constexpr uint16_t const WRAP_VERSION = 0;

	/// <summary>
	/// Handles dealing with one .wrap file.
	/// </summary>
	class Wrap
		: public Object
	{
	public:
		/// <summary>
		/// The type of the Wrap file.
		/// </summary>
		enum class Type : uint16_t
		{
			/// <summary>
			/// This file has no type. Ignore it.
			/// </summary>
			None = 0,
			/// <summary>
			/// This file is a base file.
			/// </summary>
			File = 1,
			/// <summary>
			/// This file should override some data.
			/// </summary>
			Update = 2,
		};

		/// <summary>
		/// The header information for a .wrap file. Holds meta data about the file itself.
		/// </summary>
		struct Header
		{
			/// <summary>
			/// The ID to ensure that this is in fact a Wrap file.
			/// </summary>
			char id[WRAP_MAGIC_SIZE] = { 'W', 'R', 'A', 'P' };

			/// <summary>
			/// The type of Wrap file.
			/// </summary>
			Type type = Type::None;

			/// <summary>
			/// The Wrap version.
			/// </summary>
			uint16_t wrapVersion = 0;

			/// <summary>
			/// The content version.
			/// </summary>
			uint32_t contentVersion = 0;

			/// <summary>
			/// The base path of every virtual file Entry within this Wrap.
			/// </summary>
			char basePath[WRAP_HEADER_PATH_SIZE] = "";

			/// <summary>
			/// The name of this Wrap.
			/// </summary>
			char name[WRAP_HEADER_NAME_SIZE] = "";

			/// <summary>
			/// The number of Entries within this Wrap.
			/// </summary>
			uint32_t entryCount = 0;

			Header();

			Header(Header const& other);

			Header& operator=(Header const& other);
		};

		/// <summary>
		/// The entry information for a virtual file that is stored within the .wrap file.
		/// </summary>
		struct Entry
		{
			/// <summary>
			/// The path to this Entry within the Wrap file.
			/// </summary>
			char path[WRAP_ENTRY_PATH_SIZE] = "";

			/// <summary>
			/// The compression level of this data in the Wrap file.
			/// </summary>
			Byte compressionLevel = 0;

			/// <summary>
			/// The reserved size of the data within the Wrap file.
			/// </summary>
			uint32_t reservedSize = 0;

			/// <summary>
			/// The size of the data after compression, if any.
			/// </summary>
			uint32_t compressedSize = 0;

			/// <summary>
			/// The size of the data before compression, if any.
			/// </summary>
			uint32_t uncompressedSize = 0;

			/// <summary>
			/// The offset to the data within the Wrap file.
			/// </summary>
			uint32_t offset = 0;

			Entry();

			Entry(Entry const& other);

			Entry& operator=(Entry const& other);

			bool empty() const;
		};

	private:
		// path to the wrap file on the disk
		Path _path;

		// header in the wrap file
		Header _header;

		// list of entries in the wrap file
		std::vector<Entry> _entries;
		// list of empty gaps in the wrap file, where files used to be
		std::unordered_set<uint32_t> _empties;
		// virtual paths indexed to entry indices
		std::unordered_map<Path, size_t> _indexed;

	public:
		/// <summary>
		/// Creates an empty Wrap.
		/// </summary>
		/// <param name="path">The path to the Wrap file on the disk.</param>
		Wrap();

		/// <summary>
		/// Creates and loads a Wrap file at the given path.
		/// </summary>
		/// <param name="path">The path to the Wrap file on the disk.</param>
		Wrap(Path const& path);

		/// <summary>
		/// Creates a new Wrap file with the given name, base path, and content version.
		/// </summary>
		/// <param name="path">The path to the Wrap file on the disk.</param>
		/// <param name="name">The name of the Wrap file.</param>
		/// <param name="base">The base path all files within the Wrap file.</param>
		/// <param name="contentVersion">The version of the content within the Wrap file.</param>
		Wrap(Path const& path, String const& name, uint32_t const entryCount, Path const& base = "", uint32_t const contentVersion = 0);

	public:
		// loads the Wrap file using the _path
		void load(Path const& path);

	private:
		// writes the header to the file
		void write_header(PhysicalFile& wrapFile) const;

		// writes the entry at the given index to the file
		void write_entry(PhysicalFile& wrapFile, size_t const index) const;

		// adds the given entry to _wraps, then returns the index of it
		uint32_t emplace_entry(Entry& entry);

#pragma region Files

	private:
		// forces the given path to include the base path if it doesn't already
		Path fix_path(Path const& path) const;

		// forces the given path to be relative to the base path
		Path relative_path(Path const& path) const;

	public:
		/// <summary>
		/// Adds the file at the given physicalPath to the Wrap at the given virtualPath.
		/// </summary>
		/// <param name="physicalPath">The path to the file to read and add to the Wrap file.</param>
		/// <param name="virtualPath">The path of the file within the Wrap file.</param>
		/// <param name="compression">The level of compression for the given file.</param>
		/// <param name="reservedSize">The reserved size of the chunk to store the file within. If the reservedSize is 0, it will default to the size of the file at the physicalPath.</param>
		void emplace(Path const& physicalPath, Path const& virtualPath, CompressionLevel const compression = CompressionLevel::Default, uint32_t const reservedSize = 0);

		/// <summary>
		/// Checks if the Wrap contains a file with the given path.
		/// </summary>
		/// <param name="path">The path to check.</param>
		/// <returns></returns>
		bool contains(Path const& path) const;

		/// <summary>
		/// Opens a file at the given path using the given VirtualFile.
		/// </summary>
		/// <param name="path">The path at which to open a file.</param>
		/// <param name="file">The file object to use to open with.</param>
		/// <returns></returns>
		bool open(Path const& path, VirtualFile& file) const;

		/// <summary>
		/// Opens, reads, uncompresses (if needed) and returns the data from the file at the given path.
		/// </summary>
		/// <param name="path">The path at which to open a file.</param>
		/// <returns>The uncompressed data from the file, or an empty vector if no file found.</returns>
		std::vector<char> read(Path const& path) const;

		/// <summary>
		/// Gets the Entry at the given index.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		Entry const& get_entry(size_t const index) const;

		/// <summary>
		/// Gets the Entry at the given path.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		Entry const& get_entry(Path const& path) const;
#pragma endregion

#pragma region Get Set

	public:
		/// <summary>
		/// Gets the base path of each file within this Wrap.
		/// </summary>
		/// <returns></returns>
		char const* get_base_path() const;

		/// <summary>
		/// Sets the base path of each file within this Wrap.
		/// </summary>
		/// <param name="path"></param>
		void set_base_path(Path const& path);

		/// <summary>
		/// Gets the Path that this Wrap file is located at.
		/// </summary>
		/// <returns></returns>
		Path const& get_path() const;

		/// <summary>
		/// Gets the name of this Wrap file.
		/// </summary>
		/// <returns></returns>
		char const* get_name() const;

		/// <summary>
		/// Sets the name of this Wrap file.
		/// </summary>
		/// <param name="name"></param>
		void set_name(String const& name);

		/// <summary>
		/// Gets the Wrap version of this Wrap file.
		/// </summary>
		/// <returns></returns>
		uint16_t get_wrap_version() const;

		/// <summary>
		/// Gets the content version of this Wrap file.
		/// </summary>
		/// <returns></returns>
		uint32_t get_content_version() const;

		/// <summary>
		/// Gets the type of this Wrap file.
		/// </summary>
		/// <returns></returns>
		Type get_type() const;

		/// <summary>
		/// Sets the Type for this Wrap file.
		/// </summary>
		/// <param name="type"></param>
		void set_type(Type const type);

#pragma endregion

		/// <summary>
		/// Loads or creates a new Wrap if none exists.
		/// </summary>
		/// <param name="path"></param>
		/// <param name="name"></param>
		/// <param name="entryCount"></param>
		/// <param name="base"></param>
		/// <param name="contentVersion"></param>
		/// <returns></returns>
		static Wrap load_or_create(Path const& path, String const& name, uint32_t const entryCount, Path const& base = "", uint32_t const contentVersion = 0);
	};
}