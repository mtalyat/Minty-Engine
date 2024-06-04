#pragma once

#include "Minty/Files/M_File.h"

namespace Minty
{
	/// <summary>
	/// Handles reading/writing to a physical file on the disk.
	/// </summary>
	class PhysicalFile
		: public File
	{
	protected:
		// file stream, could be in or out
		std::fstream _stream;
	public:
		PhysicalFile() = default;

		PhysicalFile(Path const& path, Flags const flags)
			: File() { open(path, flags); }

		virtual ~PhysicalFile() = default;

		virtual bool is_open() override;

		virtual void open(Path const& path, Flags const flags) override;

		virtual void flush() override;

		virtual void close() override;

		virtual void seek(Position const offset, Direction const dir = Direction::Begin) override;

		virtual void seek_read(Position const offset, Direction const dir = Direction::Begin) override;

		virtual void seek_write(Position const offset, Direction const dir = Direction::Begin) override;

		virtual bool end_of_file() override;

		virtual Position tell() override;

		virtual Position tell_read() override;

		virtual Position tell_write() override;

		virtual Size size() const override;

		virtual char peek() override;

		virtual char get() override;

		virtual void read(void* const buffer, Size const size) override;

		virtual bool read_line(String& line) override;

		virtual void write(void const* const buffer, Size const size) override;
	};
}