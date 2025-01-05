#pragma once
#include "Minty/Core/Base.h"
#include <fstream>
#include <vector>

namespace Minty
{
	/// <summary>
	/// Handles reading/writing to a file.
	/// </summary>
	class File
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

		enum class Flags : Int
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
			return static_cast<Flags>(static_cast<Int>(left) | static_cast<Int>(right));
		}

		friend inline Flags operator&(Flags const left, Flags const right)
		{
			return static_cast<Flags>(static_cast<Int>(left) & static_cast<Int>(right));
		}
	protected:
		Path m_path;

		// the file mode
		Flags m_flags;
	public:
		/// <summary>
		/// Creates an empty File object.
		/// </summary>
		File()
			: m_path()
			, m_flags()
		{}

		/// <summary>
		/// Creates a File object and saves the given path and open flags.
		/// </summary>
		/// <param name="path">The path to the file.</param>
		File(Path const& path, Flags const flags)
			: m_path(path), m_flags(flags) {}

		virtual ~File() = default;

		/// <summary>
		/// Returns true if this File is open.
		/// </summary>
		/// <returns></returns>
		virtual Bool is_open() = 0;

		/// <summary>
		/// Opens a file at the given path with the given flags. If a file is already open, it will be closed.
		/// </summary>
		/// <param name="path">The path of the file to load.</param>
		/// <param name="flags">The flags to determine how to open the file.</param>
		virtual void open(Path const& path, Flags const flags) = 0;

		/// <summary>
		/// Pushes any cached changes to the disk.
		/// </summary>
		virtual void flush() = 0;

		/// <summary>
		/// Flushes and closes the open file, if a file is open.
		/// </summary>
		virtual void close() = 0;

		/// <summary>
		/// Moves the cursor(s) to a new location within the file.
		/// </summary>
		/// <param name="offset">The offset at which the file is relative to the direction.</param>
		/// <param name="dir">The anchor point of the offset.</param>
		virtual void seek(Position const offset, Direction const dir = Direction::Begin) = 0;

		/// <summary>
		/// Moves the read cursor to a new location within the file.
		/// </summary>
		/// <param name="offset">The offset at which the file is relative to the direction.</param>
		/// <param name="dir">The anchor point of the offset.</param>
		virtual void seek_read(Position const offset, Direction const dir = Direction::Begin) = 0;

		/// <summary>
		/// Moves the write cursor to a new location within the file.
		/// </summary>
		/// <param name="offset">The offset at which the file is relative to the direction.</param>
		/// <param name="dir">The anchor point of the offset.</param>
		virtual void seek_write(Position const offset, Direction const dir = Direction::Begin) = 0;

		/// <summary>
		/// Checks if the cursor is at or past the end of the file.
		/// </summary>
		/// <returns></returns>
		virtual Bool end_of_file() = 0;

		/// <summary>
		/// Gets the current position of the cursor.
		/// </summary>
		/// <returns></returns>
		virtual Position tell() = 0;

		/// <summary>
		/// Gets the current position of the read cursor.
		/// </summary>
		/// <returns></returns>
		virtual Position tell_read() = 0;

		/// <summary>
		/// Gets the current position of the write cursor.
		/// </summary>
		/// <returns></returns>
		virtual Position tell_write() = 0;

		/// <summary>
		/// Gets the size of the file.
		/// </summary>
		/// <returns></returns>
		virtual Size size() const = 0;

		/// <summary>
		/// Checks the next character after the cursor.
		/// </summary>
		/// <returns></returns>
		virtual Char peek() = 0;

		/// <summary>
		/// Gets the next character after the cursor, and moves the cursor to that position.
		/// </summary>
		/// <returns></returns>
		virtual Char get() = 0;

		/// <summary>
		/// Reads the given size of data into the given buffer, and moves the cursor size bytes.
		/// </summary>
		/// <param name="buffer">The location to read the data to.</param>
		/// <param name="size">The number of bytes to read.</param>
		virtual void read(void* const buffer, Size const size) = 0;

		/// <summary>
		/// Reads all of the bytes from the file and returns it as a vector.
		/// </summary>
		/// <returns></returns>
		std::vector<Char> read_all();

		std::vector<Byte> read_all_bytes();

		/// <summary>
		/// Reads the next line of text, and moves the cursor the appropriate amount of bytes.
		/// </summary>
		/// <param name="delimiter">The separating character.</param>
		/// <returns></returns>
		virtual Bool read_line(String& line) = 0;

		/// <summary>
		/// Reads a number of lines from the file and returns it as a vector.
		/// </summary>
		/// <param name="count"></param>
		/// <returns></returns>
		std::vector<String> read_lines(Size const count);

		/// <summary>
		/// Reads all of the lines from the file and returns it as a vector.
		/// </summary>
		/// <returns></returns>
		std::vector<String> read_all_lines();

		/// <summary>
		/// Writes the given size of data to the file, and moves the cursor size number of bytes.
		/// </summary>
		/// <param name="buffer">The location to write the data from.</param>
		/// <param name="size">The number of bytes to write.</param>
		virtual void write(void const* const buffer, Size const size) = 0;

#pragma region Static Read/Write

		/// <summary>
		/// Reads all chars within the file at the given path.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		/// <returns>The chars from the file.</returns>
		static std::vector<Char> read_all_chars(Path const& path);

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

		static std::vector<String> read_lines(Path const& path, Size const count);

		/// <summary>
		/// Parses a Node from the file at the given path.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		/// <returns>A Node with the parsed data.</returns>
		//static Node read_node(Path const& path);

		/// <summary>
		/// Writes all of the given text to the file at the given path.
		/// </summary>
		/// <param name="path">The location of the file.</param>
		/// <param name="text">The text to write.</param>
		/// <returns>True if the text was successfully written.</returns>
		static Bool write_all_text(Path const& path, String const& text);

		/// <summary>
		/// Writes all of the given lines to the file at the given path.
		/// </summary>
		/// <param name="path">The location of the file.</param>
		/// <param name="lines">The text to write.</param>
		/// <returns>True if the lines were successfully written.</returns>
		static Bool write_all_lines(Path const& path, std::vector<String> const& lines);

		/// <summary>
		/// Writes the given Node to the file at the given path.
		/// </summary>
		/// <param name="path"></param>
		/// <param name="node"></param>
		/// <returns></returns>
		//static Bool write_node(Path const& path, Node const& node);

#pragma endregion
	};
}