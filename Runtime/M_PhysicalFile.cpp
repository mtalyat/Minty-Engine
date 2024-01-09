#include "pch.h"
#include "M_PhysicalFile.h"

#include "M_Console.h"

using namespace minty;

minty::PhysicalFile::PhysicalFile()
    : File::File()
{}

minty::PhysicalFile::PhysicalFile(Path const& path, Flags const flags)
    : File::File()
{
    open(path, flags);
}

minty::PhysicalFile::~PhysicalFile()
{}

bool minty::PhysicalFile::is_open()
{
    return _stream.is_open();
}

void minty::PhysicalFile::open(Path const& path, Flags const flags)
{
    // close if open
    close();

    _path = path;
    _flags = flags;

    // open the file
    _stream = std::fstream(path, static_cast<std::ios_base::openmode>(flags));

    // check if open
    if (!_stream.is_open())
    {
        console::error(std::format("Cannot open File at path: \"{}\"", path.string()));
        return;
    }
}

void minty::PhysicalFile::flush()
{
    _stream.flush();
}

void minty::PhysicalFile::close()
{
    if (is_open())
    {
        // close the stream
        _stream.flush();
        _stream.close();

        // reset all data
        _flags = Flags::None;
        _path = "";
    }
}

void minty::PhysicalFile::seek(Position const offset, Direction const dir)
{
    seek_read(offset, dir);
    seek_write(offset, dir);
}

void minty::PhysicalFile::seek_read(Position const offset, Direction const dir)
{
    _stream.seekg(offset, static_cast<std::ios_base::seekdir>(dir));
}

void minty::PhysicalFile::seek_write(Position const offset, Direction const dir)
{
    _stream.seekp(offset, static_cast<std::ios_base::seekdir>(dir));
}

bool minty::PhysicalFile::end_of_file()
{
    return _stream.eof();
}

File::Position minty::PhysicalFile::tell()
{
    error::abort("PhysicalFile::tell() not supported. Use tell_read() or tell_write().");
    return Position();
}

File::Position minty::PhysicalFile::tell_read()
{
    return Position();
}

File::Position minty::PhysicalFile::tell_write()
{
    return Position();
}

File::Size minty::PhysicalFile::size() const
{
    return std::filesystem::file_size(_path);
}

char minty::PhysicalFile::peek()
{
    return _stream.peek();
}

char minty::PhysicalFile::get()
{
    return _stream.get();
}

void minty::PhysicalFile::read(void* const buffer, Size const size)
{
    _stream.read(static_cast<char*>(buffer), size);
}

bool minty::PhysicalFile::read_line(String& line, char const delimiter)
{
    return static_cast<bool>(std::getline(_stream, line, delimiter));
}

void minty::PhysicalFile::write(void const* const buffer, Size const size)
{
    _stream.write(static_cast<char const* const>(buffer), size);
}

