#include "pch.h"
#include "M_File.h"

#include "M_Console.h"
#include <sstream>
#include <filesystem>

using namespace minty;

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
    return Node::load_node(path);
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