#include "pch.h"
#include "Minty/Files/M_File.h"

#include "Minty/Tools/M_Console.h"
#include <sstream>
#include <filesystem>

using namespace Minty;

std::vector<char> Minty::File::read_all()
{
    // goto beginning
    seek_read(0, Direction::Begin);

    // get the size of the file
    File::Size fileSize = size();

    // read all "bytes"
    std::vector<char> result(fileSize);
    read(result.data(), fileSize);

    return result;
}

std::vector<Byte> Minty::File::read_all_bytes()
{
    // goto beginning
    seek_read(0, Direction::Begin);

    // get the size of the file
    File::Size fileSize = size();

    // read all bytes
    std::vector<Byte> result(fileSize);
    read(result.data(), fileSize);

    return result;
}

std::vector<String> Minty::File::read_lines(size_t const count)
{
    std::vector<String> result;
    if (count > 0) result.reserve(count);

    String line;
    for (size_t i = 0; i < count && !end_of_file() && read_line(line); i++)
    {
        result.push_back(line);
    }

    return result;
}

std::vector<String> Minty::File::read_all_lines()
{
    std::vector<String> result;

    String line;
    while(!end_of_file() && read_line(line))
    {
        result.push_back(line);
    }

    return result;
}

std::vector<char> Minty::File::read_all_chars(Path const& path)
{
    if (!std::filesystem::exists(path))
    {
        MINTY_ERROR_FORMAT("File not found at: {}", std::filesystem::absolute(path).string());
        return {};
    }

    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        MINTY_ERROR_FORMAT("Failed to open file at: {}", std::filesystem::absolute(path).string());
        return {};
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

String Minty::File::read_all_text(Path const& path)
{
    if (!std::filesystem::exists(path))
    {
        MINTY_ERROR_FORMAT("File not found at: {}", std::filesystem::absolute(path).string());
        return String();
    }

    // get the file to read from
    std::fstream file(path, std::ios::in);

    if (!file.is_open())
    {
        MINTY_ERROR_FORMAT("Failed to open file at: {}", std::filesystem::absolute(path).string());
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
        MINTY_ERROR_FORMAT("File not opened at: ", std::filesystem::absolute(path).string());
        return String();
    }

    // return string
    return ss.str();
}

std::vector<String> Minty::File::read_all_lines(Path const& path)
{
    if (!std::filesystem::exists(path))
    {
        MINTY_ERROR_FORMAT("File not found at: {}", std::filesystem::absolute(path).string());
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
        MINTY_ERROR_FORMAT("File not opened at: ", std::filesystem::absolute(path).string());
        return std::vector<String>();
    }

    // return list, regardless of size
    return lines;
}

std::vector<String> Minty::File::read_lines(Path const& path, size_t const count)
{
    if (!std::filesystem::exists(path) || count <= 0)
    {
        MINTY_ERROR_FORMAT("File not found at: {}", std::filesystem::absolute(path).string());
        return std::vector<String>();
    }

    std::vector<String> lines;

    // get the file to read from
    std::fstream file(path, std::ios::in);

    // if the file exists and is open
    if (file.is_open())
    {
        String line;

        size_t i = 0;

        // while there are lines, add to output
        while (i < count && std::getline(file, line)) {
            lines.push_back(line);
            i++;
        }

        // done with file, close it
        file.close();
    }
    else
    {
        MINTY_ERROR_FORMAT("File not opened at: ", std::filesystem::absolute(path).string());
        return std::vector<String>();
    }

    // return list, regardless of size
    return lines;
}

Node Minty::File::read_node(Path const& path)
{
    return Node::parse(File::read_all_lines(path));
}

bool Minty::File::write_all_text(Path const& path, String const& text)
{
    std::fstream file(path, std::ios::out);

    if (!file.is_open())
    {
        MINTY_ERROR_FORMAT("File not created at: {}", std::filesystem::absolute(path).string());
        return false;
    }

    // write text
    file << text;

    // close
    file.close();

    return true;
}

bool Minty::File::write_all_lines(Path const& path, std::vector<String> const& lines)
{
    std::fstream file(path, std::ios::out);

    if (!file.is_open())
    {
        MINTY_ERROR_FORMAT("File not created at: {}", std::filesystem::absolute(path).string());
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

bool Minty::File::write_node(Path const& path, Node const& node)
{
    return File::write_all_lines(path, node.get_formatted(true));
}