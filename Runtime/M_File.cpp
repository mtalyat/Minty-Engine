#include "pch.h"
#include "M_File.h"

#include "M_Console.h"
#include <fstream>
#include <sstream>
#include <format>
#include <filesystem>

std::string minty::file::read_all_text(std::string const& path)
{
    if (!exists(path))
    {
        console::error(std::format("File not found at: {}", path));
        return std::string();
    }

    // get the file to read from
    std::fstream file(path, std::ios::in);

    // put string data into a stream
    std::stringstream ss;

    // if the file exists and is open
    if (file.is_open())
    {
        std::string line;

        // while there are lines, add to output
        while (std::getline(file, line)) {
            ss << line << std::endl;
        }

        // done with file, close it
        file.close();
    }
    else
    {
        console::error(std::format("File not opened at: ", path));
        return std::string();
    }

    // return string
    return ss.str();
}

std::vector<std::string> minty::file::read_all_lines(std::string const& path)
{
    if (!exists(path))
    {
        console::error(std::format("File not found at: {}", path));
        return std::vector<std::string>();
    }

    std::vector<std::string> lines;

    // get the file to read from
    std::fstream file(path, std::ios::in);

    // if the file exists and is open
    if (file.is_open())
    {
        std::string line;

        // while there are lines, add to output
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        // done with file, close it
        file.close();
    }
    else
    {
        console::error(std::format("File not opened at: ", path));
        return std::vector<std::string>();
    }

    // return list, regardless of size
    return lines;
}

bool minty::file::exists(std::string const& path)
{
    return std::filesystem::exists(path);
}

bool minty::file::write_all_text(std::string const& path, std::string const& text)
{
    std::fstream file(path, std::ios::out);

    if (!file.is_open())
    {
        console::error(std::format("File not created at: {}", path));
        return false;
    }

    // write text
    file << text;

    // close
    file.close();

    return true;
}

bool minty::file::write_all_lines(std::string const& path, std::vector<std::string> const& lines)
{
    std::fstream file(path, std::ios::out);

    if (!file.is_open())
    {
        console::error(std::format("File not created at: {}", path));
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
