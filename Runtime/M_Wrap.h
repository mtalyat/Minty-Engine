#pragma once

#include "M_Object.h"
#include <unordered_map>

namespace minty
{
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
		enum class Type
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
			char id[4] = { 'W', 'R', 'A', 'P' };
			// type of wrap
			Type type = Type::None;
			// version of wrap
			uint16_t wrapVersion = 0;
			// version of the data
			uint32_t contentVersion = 0;
			// base path of the physical folder that contains all of the entries
			char basePath[100] = "";
			// the name of the folder?
			char name[50] = "";
			// number of entries
			uint32_t entryCount = 0;
		};

		/// <summary>
		/// The entry information for a physical file that is stored within the .wrap file.
		/// </summary>
		struct Entry
		{
			// old physical file path
			char path[255] = "";
			// is the data compressed?
			Byte compressed = 0;
			// size of data
			unsigned int size = 0;
			// offset to from beginning
			unsigned int offset = 0;
		};

	private:
		Header _header;

		std::unordered_map<Path, Entry> _entries;

	public:
		/// <summary>
		/// Creates a wrap file in memory.
		/// </summary>
		/// <param name="path">The path to the location of the base path for a new wrap file, or the location to a wrap file that is to be loaded.</param>
		Wrap(Path const& path);

		uint32_t get_version() const;

		char const* get_base_path() const;

		char const* get_name() const;

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
		Wrapper(Path const& path);

		void load(Path const& path, bool const recursive = false);
	};
}