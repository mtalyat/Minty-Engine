#include "pch.h"
#include "M_File.h"

#include "M_Console.h"
#include <sstream>
#include <filesystem>

using namespace minty;

minty::File::File()
    : _path()
    , _stream()
    , _flags()
{}

minty::File::File(Path const& path, Flags const flags)
    : _path()
    , _stream()
    , _flags(flags)
{
    open(path, flags);
}

minty::File::~File()
{}

bool minty::File::is_open()
{
    return _stream.is_open();
}

void minty::File::open(Path const& path, Flags const flags)
{
    // close if open
    close();

    _path = path;

    // open the file
    _stream = std::fstream(path, static_cast<std::ios_base::openmode>(flags));

    // check if open
    if (!_stream.is_open())
    {
        console::error(std::format("Cannot open File at path: \"{}\"", path.string()));
        return;
    }
}

void minty::File::flush()
{
    _stream.flush();
}

void minty::File::close()
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

void minty::File::seek_read(Position const offset, Direction dir)
{
    _stream.seekg(offset, static_cast<std::ios_base::seekdir>(dir));
}

void minty::File::seek_write(Position const offset, Direction const dir)
{
    _stream.seekp(offset, static_cast<std::ios_base::seekdir>(dir));
}

bool minty::File::eof()
{
    return _stream.eof();
}

File::Position minty::File::tell_read()
{
    return _stream.tellg();
}

File::Position minty::File::tell_write()
{
    return _stream.tellp();
}

File::Size minty::File::size() const
{
    return std::filesystem::file_size(_path);
}

char minty::File::peek()
{
    return _stream.peek();
}

char minty::File::get()
{
    return _stream.get();
}

void minty::File::read(void* const buffer, Size const size)
{
    _stream.read(static_cast<char*>(buffer), size);
}

String minty::File::read_line()
{
    String line;

    std::getline(_stream, line);

    return line;
}

String minty::File::read_line(char const delimiter)
{
    String line;

    std::getline(_stream, line, delimiter);

    return line;
}

std::vector<char> minty::File::read_all_chars(Path const& path)
{
    if (!std::filesystem::exists(path))
    {
        console::error(std::format("File not found at: {}", path.string()));
        return {};
    }

    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        console::error(std::format("Failed to open file at: {}", path.string()));
        return {};
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

String minty::File::read_all_text(Path const& path)
{
    if (!std::filesystem::exists(path))
    {
        console::error(std::format("File not found at: {}", path.string()));
        return String();
    }

    // get the file to read from
    std::fstream file(path, std::ios::in);

    if (!file.is_open())
    {
        console::error(std::format("Failed to open file at: {}", path.string()));
        return String();
    }

    // put string data into a stream
    std::stringstream ss;

    // if the file exists and is open
    if (file.is_open())
    {
        String line;

        // while there are lines, add to output
        while (std::getline(file, line)) {
            ss << line << std::endl;
        }

        // done with file, close it
        file.close();
    }
    else
    {
        console::error(std::format("File not opened at: ", path.string()));
        return String();
    }

    // return string
    return ss.str();
}

std::vector<String> minty::File::read_all_lines(Path const& path)
{
    if (!std::filesystem::exists(path))
    {
        console::error(std::format("File not found at: {}", path.string()));
        return std::vector<String>();
    }

    std::vector<String> lines;

    // get the file to read from
    std::fstream file(path, std::ios::in);

    // if the file exists and is open
    if (file.is_open())
    {
        String line;

        // while there are lines, add to output
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        // done with file, close it
        file.close();
    }
    else
    {
        console::error(std::format("File not opened at: ", path.string()));
        return std::vector<String>();
    }

    // return list, regardless of size
    return lines;
}

Node minty::File::read_node(Path const& path)
{
    std::vector<String> lines = File::read_all_lines(path);

    int indent = 0;
    String key;
    String value;

    Node root;

    // if no lines, node is empty
    if (!lines.size())
    {
        return root;
    }

    // if first line starts with a ": ", then that is the data for the root node
    if (lines.front().starts_with(": "))
    {
        root.data = lines.front().substr(2, lines.front().size() - 2);
    }

    std::vector<Node*> nodeStack;
    nodeStack.push_back(&root);
    Node* node = nodeStack.back();

    int const SPACES_PER_TAB = 4;

    for (String line : lines)
    {
        // skip empty/whitespace/comment lines
        if (line.size() == 0 || line.find_first_not_of(" \t\n\v\f\r") == String::npos || line.front() == '#' || line.front() == ':')
        {
            continue;
        }

        // count number of tabs (indents)
        int spaces = 0;
        for (char const c : line)
        {
            if (c == ' ' || c == '\t')
            {
                spaces++;
            }
            else
            {
                break;
            }
        }
        int i = spaces / SPACES_PER_TAB;

        int indentChange = i - indent;

        // if new indent is too deep, ignore
        if (indentChange > 1)
        {
            console::warn(std::format("Discarding line, invalid indent change of {}: {}", indentChange, line));
            continue;
        }

        // check change in index
        if (indentChange > 0)
        {
            // add last child to node stack
            nodeStack.push_back(&node->children.at(key).back());

            // start using that as active node
            node = nodeStack.back();

            // update indent
            indent = i;
        }
        else if (indentChange < 0)
        {
            // going down, so pop down X nodes, where X is the difference between indents
            for (; indentChange < 0; indentChange++)
            {
                nodeStack.pop_back();
            }

            // update node reference
            node = nodeStack.back();

            // update indent
            indent = i;
        }

        // remove indents for parsing
        if (spaces > 0)
        {
            line = line.substr(static_cast<size_t>(spaces), line.size() - spaces);
        }

        // split by colon, if there is one
        size_t split = line.find_first_of(':');

        Node newNode;

        if (split == String::npos)
        {
            // no split, just key
            key = line;
        }
        else
        {
            // split: implies key: value
            key = line.substr(0, split);
            value = line.substr(split + 2, line.size() - split - 2); // ignore ": "
            newNode.data = value;
        }

        // add node to children
        // if key name exists, add there, otherwise create new
        auto found = node->children.find(key);
        if (found != node->children.end())
        {
            found->second.push_back(newNode);
        }
        else
        {
            node->children.emplace(key, std::vector<Node>{ newNode });
        }
    }

    // root node should contain everything
    return root;
}

void minty::File::write(void const* const buffer, Size const size)
{
    _stream.write(static_cast<char const* const>(buffer), size);
}

bool minty::File::write_all_text(Path const& path, String const& text)
{
    std::fstream file(path, std::ios::out);

    if (!file.is_open())
    {
        console::error(std::format("File not created at: {}", path.string()));
        return false;
    }

    // write text
    file << text;

    // close
    file.close();

    return true;
}

bool minty::File::write_all_lines(Path const& path, std::vector<String> const& lines)
{
    std::fstream file(path, std::ios::out);

    if (!file.is_open())
    {
        console::error(std::format("File not created at: {}", path.string()));
        return false;
    }

    // write all lines
    for (auto const& line : lines)
    {
        file << line << std::endl;
    }

    // close
    file.close();

    return true;
}

minty::VirtualFile::VirtualFile(Path const& path, Flags const flags, Position const offset, Size const size)
    : File()
    , _virtualOffset()
    , _virtualSize()
{
    open(path, flags, offset, size);
}

void minty::VirtualFile::open(Path const& path, Flags const flags, Position const offset, Size const size)
{
    File::open(path, flags);
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
        File::seek_read(_virtualOffset + offset);
        break;
    case Direction::Current:
        File::seek_read(offset);
        break;
    case Direction::End:
        File::seek_read(offset + (_virtualOffset + _virtualSize));
        break;
    }
}

void minty::VirtualFile::seek_write(Position const offset, Direction const seekDir)
{
    switch (seekDir)
    {
    case Direction::Begin:
        File::seek_write(_virtualOffset + offset);
        break;
    case Direction::Current:
        File::seek_write(offset);
        break;
    case Direction::End:
        File::seek_write(offset + (_virtualOffset + _virtualSize));
        break;
    }
}

bool minty::VirtualFile::eof()
{
    return File::eof() || (tell_read() >= _virtualSize);
}

File::Position minty::VirtualFile::tell_read()
{
    return File::tell_read() - _virtualOffset;
}

File::Position minty::VirtualFile::tell_write()
{
    return File::tell_write() - _virtualOffset;
}

File::Size minty::VirtualFile::size() const
{
    return _virtualSize;
}

void minty::VirtualFile::read(void* const buffer, Size const size)
{
    // if at the end of the file, stop
    if (eof())
    {
        console::error("Cannot read from file to buffer. End Of File.");
        return;
    }

    // read as per normal
    File::read(buffer, size);
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
    File::write(buffer, size);
}

minty::PhysicalFile::PhysicalFile()
    : File::File()
{}

minty::PhysicalFile::PhysicalFile(Path const& path, Flags const flags)
    : File::File(path, flags)
{}
