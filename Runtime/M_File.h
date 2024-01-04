#pragma once

#include "M_Object.h"
#include "M_Node.h"
#include <fstream>
#include <unordered_map>

namespace minty
{
	/// <summary>
	/// Handles reading/writing to a file.
	/// </summary>
	class File
		: public Object
	{
	public:
		typedef std::streampos Position;
		typedef std::streamsize Size;
		enum class Direction
		{
			Begin = std::ios_base::beg,
			Current = std::ios_base::cur,
			End = std::ios_base::end,
		};
		enum class Flags : int
		{
			None = 0,
			Read = std::ios_base::in,
			Write = std::ios_base::out,
			ReadWrite = std::ios_base::in | std::ios_base::out,
			End = std::ios_base::end,
			Append = std::ios_base::app,
			Truncate = std::ios_base::trunc,
			Binary = std::ios_base::binary,
		};
		friend inline Flags operator|(Flags const left, Flags const right)
		{
			return static_cast<Flags>(static_cast<int>(left) | static_cast<int>(right));
		}
		friend inline Flags operator&(Flags const left, Flags const right)
		{
			return static_cast<Flags>(static_cast<int>(left) & static_cast<int>(right));
		}
	protected:
		Path _path;

		// file stream, could be in or out
		std::fstream _stream;

		// the file mode
		Flags _flags;
	public:
		/// <summary>
		/// Creates an empty File object.
		/// </summary>
		File();

		/// <summary>
		/// Creates a File object and opens the file at the given path.
		/// </summary>
		/// <param name="path">The path to the file.</param>
		File(Path const& path, Flags const flags);

		virtual ~File();

		/// <summary>
		/// Returns true if this File is open.
		/// </summary>
		/// <returns></returns>
		bool is_open();

		/// <summary>
		/// Opens a file at the given path with the given flags. If a file is already open, it will be closed.
		/// </summary>
		/// <param name="path">The path of the file to load.</param>
		/// <param name="flags">The flags to determine how to open the file.</param>
		void open(Path const& path, Flags const flags);

		/// <summary>
		/// Closes the open file, if a file is open.
		/// </summary>
		void close();

		/// <summary>
		/// Moves the cursor to a new location within the file.
		/// </summary>
		/// <param name="offset">The offset at which the file is relative to the direction.</param>
		/// <param name="dir">The anchor point of the offset.</param>
		virtual void seek_read(Position const offset, Direction const dir = Direction::Begin);

		virtual void seek_write(Position const offset, Direction const dir = Direction::Begin);

		/// <summary>
		/// Checks if the cursor is at or past the end of the file.
		/// </summary>
		/// <returns></returns>
		virtual bool eof();

		/// <summary>
		/// Gets the current position of the cursor.
		/// </summary>
		/// <returns></returns>
		virtual Position tell_read();

		virtual Position tell_write();

		/// <summary>
		/// Gets the size of the file.
		/// </summary>
		/// <returns></returns>
		virtual Size size() const;

		/// <summary>
		/// Checks the next character after the cursor.
		/// </summary>
		/// <returns></returns>
		virtual char peek();

		/// <summary>
		/// Gets the next character after the cursor, and moves the cursor to that position.
		/// </summary>
		/// <returns></returns>
		virtual char get();

		/// <summary>
		/// Reads the given size of data into the given buffer, and moves the cursor size bytes.
		/// </summary>
		/// <param name="buffer">The location to read the data to.</param>
		/// <param name="size">The number of bytes to read.</param>
		virtual void read(void* const buffer, Size const size);

		/// <summary>
		/// Reads the next bytes into the given data type T, and returns it.
		/// </summary>
		/// <typeparam name="T">The type of object to read and return.</typeparam>
		/// <returns></returns>
		template<typename T>
		T read();

		/// <summary>
		/// Reads the next line of text, and moves the cursor the appropriate amount of bytes.
		/// </summary>
		/// <returns></returns>
		virtual String read_line();

		/// <summary>
		/// Reads the next line of text, and moves the cursor the appropriate amount of bytes.
		/// </summary>
		/// <param name="delimiter">The separating character.</param>
		/// <returns></returns>
		virtual String read_line(char const delimiter);

		/// <summary>
		/// Reads all chars within the file at the given path.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		/// <returns>The chars from the file.</returns>
		static std::vector<char> read_all_chars(Path const& path);

		/// <summary>
		/// Reads all of the text from the file at the given path.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		/// <returns>The text from the file.</returns>
		static String read_all_text(Path const& path);

		/// <summary>
		/// Reads all of the lines from the file at the given path.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		/// <returns>The lines from the file.</returns>
		static std::vector<String> read_all_lines(Path const& path);

		/// <summary>
		/// Parses a Node from the file at the given path.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		/// <returns>A Node with the parsed data.</returns>
		static Node read_node(Path const& path);

		/// <summary>
		/// Writes the given size of data to the file, and moves the cursor size number of bytes.
		/// </summary>
		/// <param name="buffer">The location to write the data from.</param>
		/// <param name="size">The number of bytes to write.</param>
		virtual void write(void const* const buffer, Size const size);

		/// <summary>
		/// Writes the given object to the file, and moves the cursor sizeof(T) number of bytes.
		/// </summary>
		/// <typeparam name="T">The type of object to be written.</typeparam>
		/// <param name="obj">The object to write to the file.</param>
		template<typename T>
		void write(T const& obj);

		/// <summary>
		/// Writes all of the given text to the file at the given path.
		/// </summary>
		/// <param name="path">The location of the file.</param>
		/// <param name="text">The text to write.</param>
		/// <returns>True if the text was successfully written.</returns>
		static bool write_all_text(Path const& path, String const& text);

		/// <summary>
		/// Writes all of the given lines to the file at the given path.
		/// </summary>
		/// <param name="path">The location of the file.</param>
		/// <param name="lines">The text to write.</param>
		/// <returns>True if the lines were successfully written.</returns>
		static bool write_all_lines(Path const& path, std::vector<String> const& lines);
	};

	template<typename T>
	T File::read()
	{
		T t;
		read(&t, sizeof(T));
		return t;
	}

	template<typename T>
	void File::write(T const& obj)
	{
		write(&obj, sizeof(T));
	}

	/// <summary>
	/// Handles reading/writing to a physical file on the disk.
	/// </summary>
	class PhysicalFile
		: public File
	{
	public:
		PhysicalFile();

		PhysicalFile(Path const& path, Flags const flags);
	};

	/// <summary>
	/// Handles reading/writing to a virtual file.
	/// </summary>
	class VirtualFile
		: public File
	{
	protected:
		// offset within the physical file
		Position _virtualOffset;
		// size within the physical file
		Size _virtualSize;

	public:
		VirtualFile(Path const& path, Flags const flags, Position const offset, Size const size);

		void seek_read(Position const offset, Direction dir = Direction::Begin) override;

		bool eof() override;

		Position tell_read() override;

		Size size() const override;

		void read(void* const buffer, Size const size) override;

		void write(void const* const buffer, Size const size) override;
	};

}