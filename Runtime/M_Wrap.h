#pragma once

#include "M_Object.h"
#include "M_Register.h"
#include "M_File.h"
#include <unordered_map>

namespace minty
{
	constexpr char const* const WRAP_MAGIC = "WRAP";
	constexpr int const WRAP_HEADER_ID_SIZE = 4;
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
		/// Determines when to compress data being stored in a wrap file.
		/// </summary>
		enum class Compression : Byte
		{
			Level0 = 0,
			Level1 = 1,
			Level2 = 2,
			Level3 = 3,
			Level4 = 4,
			Level5 = 5,
			Level6 = 6,
			Level7 = 7,
			Level8 = 8,
			Level9 = 9,

			/// <summary>
			/// Do not compress the data.
			/// </summary>
			None = Level0,

			/// <summary>
			/// Lightly compresses the data, quickly.
			/// </summary>
			Fast = Level1,
			/// <summary>
			/// Heavily compresses the data, slowly.
			/// </summary>
			Slow = Level9,

			/// <summary>
			/// Lightly compress the data.
			/// </summary>
			Low = Level1,
			/// <summary>
			/// Heavily compress the data.
			/// </summary>
			High = Level9,

			/// <summary>
			/// Default compression.
			/// </summary>
			Default = Level6,
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
		/// Creates an empty Wrap file.
		/// </summary>
		Wrap();

		/// <summary>
		/// Creates a Wrap file.
		/// </summary>
		/// <param name="name">The base path.</param>
		/// <param name="path">The base path.</param>
		Wrap(String const& name, Path const& path, uint32_t const contentVersion = 0);

		/// <summary>
		/// Creates and loads a Wrap file at the given path.
		/// </summary>
		/// <param name="path">The path to the Wrap file on the disk.</param>
		Wrap(Path const& path);

#pragma region Get

		uint32_t get_version() const;

		char const* get_base_path() const;

		char const* get_name() const;

		uint16_t get_wrap_version() const;

		uint32_t get_content_version() const;

		Type get_type() const;

#pragma endregion

#pragma region Set

		void set_type(Type const type);

#pragma endregion

		bool contains(Path const& path) const;

		Entry& at(Path const& path);

		Entry const& at(Path const& path) const;

		/// <summary>
		/// Adds the file at the given path to this Wrap file.
		/// </summary>
		/// <param name="path"></param>
		/// <param name="recursive"></param>
		/// <param name="compression"></param>
		void emplace(Path const& path, bool const recursive = true, Compression const compression = Compression::Default);

#pragma region Saving and Loading

		/// <summary>
		/// Saves the Wrap file to the disk.
		/// </summary>
		/// <param name="path">The path to save to.</param>
		void save(Path const& path) const;

		/// <summary>
		/// Loads the Wrap file from the disk.
		/// </summary>
		/// <param name="path">The path to load from.</param>
		void load(Path const& path);

#pragma endregion		
	};

	/// <summary>
	/// Handles dealing with .wrap files.
	/// </summary>
	class Wrapper
		: public Object
	{
	private:
		std::unordered_map<String, Wrap> _wraps;

		Register<File*> _files;

	public:
		/// <summary>
		/// Creates an empty Wrapper.
		/// </summary>
		Wrapper();

		~Wrapper();

		void emplace(Wrap const& wrap);

		void emplace(Path const& path, bool const recursive = true);

		Wrap const& get(String const& name) const;

		ID open(Path const& path);

		File* at(ID const id) const;

		File* at(Path const& path) const;

		void close(ID const id);

		void close(Path const& path);
	};
}