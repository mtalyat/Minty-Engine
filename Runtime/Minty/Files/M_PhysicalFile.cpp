#include "pch.h"
#include "Minty/Files/M_PhysicalFile.h"

#include "Minty/Tools/M_Console.h"

using namespace Minty;

bool Minty::PhysicalFile::is_open()
{
    return _stream.is_open();
}

void Minty::PhysicalFile::open(Path const& path, Flags const flags)
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
        MINTY_ERROR_FORMAT("Cannot open File at path: \"{}\"", path.string());
        return;
    }
}

void Minty::PhysicalFile::flush()
{
    _stream.flush();
}

void Minty::PhysicalFile::close()
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

void Minty::PhysicalFile::seek(Position const offset, Direction const dir)
{
    seek_read(offset, dir);
    seek_write(offset, dir);
}

void Minty::PhysicalFile::seek_read(Position const offset, Direction const dir)
{
    _stream.seekg(offset, static_cast<std::ios_base::seekdir>(dir));
}

void Minty::PhysicalFile::seek_write(Position const offset, Direction const dir)
{
    _stream.seekp(offset, static_cast<std::ios_base::seekdir>(dir));
}

bool Minty::PhysicalFile::end_of_file()
{
    return _stream.eof();
}

File::Position Minty::PhysicalFile::tell()
{
    MINTY_ABORT("PhysicalFile::tell() not supported. Use tell_read() or tell_write().");
    return Position();
}

File::Position Minty::PhysicalFile::tell_read()
{
    return Position();
}

File::Position Minty::PhysicalFile::tell_write()
{
    return Position();
}

File::Size Minty::PhysicalFile::size() const
{
    return std::filesystem::file_size(_path);
}

char Minty::PhysicalFile::peek()
{
    return _stream.peek();
}

char Minty::PhysicalFile::get()
{
    return _stream.get();
}

void Minty::PhysicalFile::read(void* const buffer, Size const size)
{
    _stream.read(static_cast<char*>(buffer), size);
}

bool Minty::PhysicalFile::read_line(String& line)
{
    // get the line
    if (std::getline(_stream, line))
    {
        // remove the \r
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        return true;
    }

    return false;
}

void Minty::PhysicalFile::write(void const* const buffer, Size const size)
{
    _stream.write(static_cast<char const* const>(buffer), size);
}

