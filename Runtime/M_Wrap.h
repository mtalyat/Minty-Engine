#pragma once

#include "M_Object.h"
#include <unordered_map>

namespace minty
{
	constexpr char const* const WRAP_MAGIC = "WRAP";
	constexpr int const WRAP_HEADER_ID_SIZE = 4;
	constexpr int const WRAP_HEADER_PATH_SIZE = 100;
	constexpr int const WRAP_HEADER_NAME_SIZE = 50;
	constexpr int const WRAP_ENTRY_PATH_SIZE = 255;

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
			// id to ensure this is a wrap file
			char id[WRAP_HEADER_ID_SIZE] = { 'W', 'R', 'A', 'P' };
			// type of wrap
			Type type = Type::None;
			// version of wrap
			uint16_t wrapVersion = 0;
			// version of the data
			uint32_t contentVersion = 0;
			// base path of the physical folder that contains all of the entries
			char basePath[WRAP_HEADER_PATH_SIZE] = "";
			// the name of the folder?
			char name[WRAP_HEADER_NAME_SIZE] = "";
			// number of entries
			uint32_t entryCount = 0;

			Header();

			Header(Header const& other);

			Header& operator=(Header const& other);
		};

		/// <summary>
		/// The entry information for a physical file that is stored within the .wrap file.
		/// </summary>
		struct Entry
		{
			// old physical file path
			char path[WRAP_ENTRY_PATH_SIZE] = "";
			// is the data compressed?
			Byte compressed = 0;
			// size of data
			unsigned int size = 0;
			// offset to from beginning
			unsigned int offset = 0;

			Entry();

			Entry(Entry const& other);

			Entry& operator=(Entry const& other);
		};

	private:
		Header _header;

		std::unordered_map<Path, Entry> _entries;

	public:
		/// <summary>
		/// Creates an empty wrap file in memory.
		/// </summary>
		Wrap();

		/// <summary>
		/// Loads the wrap file from the given path into memory.
		/// </summary>
		/// <param name="path">The path to the location of the base path for a new wrap file, or the location to a wrap file that is to be loaded.</param>
		Wrap(Path const& path);

		uint32_t get_version() const;

		char const* get_base_path() const;

		char const* get_name() const;

		uint16_t get_wrap_version() const;

		uint32_t get_content_version() const;

		Entry const* get_entry(Path const& path) const;

		bool contains(Path const& path);

		Entry& at(Path const& path);

		Entry const& at(Path const& path) const;

		Type get_type() const;

		void set_type(Type const type);
	};

	/// <summary>
	/// Handles dealing with .wrap files.
	/// </summary>
	class Wrapper
		: public Object
	{
	public:

	private:
		std::unordered_map<String, Wrap> _wraps;

	public:
		/// <summary>
		/// Creates an empty Wrapper.
		/// </summary>
		Wrapper();

		/// <summary>
		/// Creates a Wrapper that will load the .wrap file at the given path, or all of the .wrap files within the given directory path.
		/// </summary>
		/// <param name="path"></param>
		Wrapper(Path const& path, bool const recursive = false);

		void load(Path const& path, bool const recursive = false);
	};
}