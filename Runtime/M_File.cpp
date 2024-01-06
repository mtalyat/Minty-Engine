#include "pch.h"
#include "M_File.h"

#include "M_Console.h"
#include <sstream>
#include <filesystem>

using namespace minty;

minty::File::File()
    : _path()
    , _flags()
{}

minty::File::File(Path const& path, Flags const flags)
    : _path()
    , _flags()
{
    open(path, flags);
}

minty::File::~File()
{}

bool minty::File::read_line(String& line)
{
    return read_line(line, '\n');
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