#include "pch.h"
#include "M_VirtualFile.h"

#include "M_Console.h"

using namespace minty;

minty::VirtualFile::VirtualFile()
    : PhysicalFile()
    , _virtualOffset()
    , _virtualSize()
{}

minty::VirtualFile::VirtualFile(Path const& path, Flags const flags, Position const offset, Size const size)
    : PhysicalFile()
    , _virtualOffset()
    , _virtualSize()
{
    open(path, flags, offset, size);
}

minty::VirtualFile::~VirtualFile()
{}

void minty::VirtualFile::open(Path const& path, Flags const flags, Position const offset, Size const size)
{
    PhysicalFile::open(path, flags);
    _virtualOffset = offset;
    _virtualSize = size;

    // seek to beginning of the virtual file
    seek_write(0);
    seek_read(0);
}

void minty::VirtualFile::seek_read(Position const offset, Direction seekDir)
{
    switch (seekDir)
    {
    case Direction::Begin:
        PhysicalFile::seek_read(_virtualOffset + offset);
        break;
    case Direction::Current:
        PhysicalFile::seek_read(offset);
        break;
    case Direction::End:
        PhysicalFile::seek_read(offset + (_virtualOffset + _virtualSize));
        break;
    }
}

void minty::VirtualFile::seek_write(Position const offset, Direction const seekDir)
{
    switch (seekDir)
    {
    case Direction::Begin:
        PhysicalFile::seek_write(_virtualOffset + offset);
        break;
    case Direction::Current:
        PhysicalFile::seek_write(offset);
        break;
    case Direction::End:
        PhysicalFile::seek_write(offset + (_virtualOffset + _virtualSize));
        break;
    }
}

bool minty::VirtualFile::end_of_file()
{
    return PhysicalFile::end_of_file() || (tell_read() >= _virtualSize);
}

PhysicalFile::Position minty::VirtualFile::tell_read()
{
    return PhysicalFile::tell_read() - _virtualOffset;
}

PhysicalFile::Position minty::VirtualFile::tell_write()
{
    return PhysicalFile::tell_write() - _virtualOffset;
}

PhysicalFile::Size minty::VirtualFile::size() const
{
    return _virtualSize;
}

void minty::VirtualFile::read(void* const buffer, Size const size)
{
    // if at the end of the file, stop
    if (end_of_file())
    {
        console::error("Cannot read from file to buffer. End Of File.");
        return;
    }

    // read as per normal
    PhysicalFile::read(buffer, size);
}

void minty::VirtualFile::write(void const* const buffer, Size const size)
{
    // ensure will not write out of bounds
    if (tell_read() + size >= _virtualSize)
    {
        console::error("Failed to write to virtual file. Out of bounds.");
        return;
    }

    // write as per normal
    PhysicalFile::write(buffer, size);
}
