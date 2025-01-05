#pragma once

#include "Minty/File/PhysicalFile.h"

namespace Minty
{
	/// <summary>
	/// Handles reading/writing to a virtual file.
	/// </summary>
	class VirtualFile
		: public PhysicalFile
	{
	protected:
		// offset within the physical file
		Position _virtualOffset;
		// size within the physical file
		Size _virtualSize;

	public:
		VirtualFile() = default;

		VirtualFile(Path const& path, Flags const flags, Position const offset, Size const size)
			: PhysicalFile(), _virtualOffset(), _virtualSize() {
			open(path, flags, offset, size);
		}

		virtual ~VirtualFile() = default;

		virtual void open(Path const& path, Flags const flags, Position const offset, Size const size);

		virtual void seek_read(Position const offset, Direction dir = Direction::Begin) override;

		virtual void seek_write(Position const offset, Direction const dir = Direction::Begin) override;

		virtual Bool end_of_file() override;

		virtual Position tell_read() override;

		virtual Position tell_write() override;

		virtual Size size() const override;

		virtual void read(void* const buffer, Size const size) override;

		virtual void write(void const* const buffer, Size const size) override;
	};
}