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
		typedef std::streamoff Offset;
		typedef std::streamsize Size;
		enum class Direction
		{
			Begin = std::ios_base::beg,
			Current = std::ios_base::cur,
			End = std::ios_base::end,
		};
		enum class Flags
		{
			None = 0,
			Read = std::ios_base::in,
			Write = std::ios_base::out,
			End = std::ios_base::end,
			Append = std::ios_base::app,
			Truncate = std::ios_base::trunc,
			Binary = std::ios_base::binary,
		};
	protected:
		// file stream, could be in or out
		std::fstream _stream;

		// offset of the stream
		Offset _streamOffset;
		// size of the stream
		Size _streamSize;

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

		bool is_open();

		void open(Path const& path, Flags const flags);

		void close();

		virtual void seek(Offset const offset, Direction const dir = Direction::Begin);

		virtual bool eof();

		virtual Offset tell();

		virtual Offset offset() const;

		virtual Size size() const;

		virtual char peek();

		virtual char get();

		virtual void read(void* const buffer, Size const size);

		template<typename T>
		T read();

		virtual String read_line();

		virtual String read_line(char const delimiter);

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

	/// <summary>
	/// Handles reading/writing to a physical file on the disk.
	/// </summary>
	class PhysicalFile
		: public File
	{
	public:
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
		Offset _virtualOffset;
		// size within the physical file
		Size _virtualSize;

	public:
		VirtualFile(Path const& path, Flags const flags, Offset const offset, Size const size);

		virtual void seek(Offset const offset, Direction dir = Direction::Begin) override;

		virtual bool eof() override;

		virtual Offset tell() override;

		virtual Offset offset() const override;

		virtual Size size() const override;

		virtual void read(void* const buffer, Size const size) override;
	};

}