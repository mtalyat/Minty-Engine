#include "pch.h"
#include "M_File.h"

#include "M_Console.h"
#include <fstream>
#include <sstream>
#include <format>
#include <filesystem>

using namespace minty;

std::vector<char> minty::file::read_all_chars(std::string const& path)
{
	if (!exists(path))
	{
		console::error(std::format("File not found at: {}", path));
		return {};
	}

	std::ifstream file(path, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		console::error(std::format("Failed to open file at: {}", path));
		return {};
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

std::string minty::file::read_all_text(std::string const& path)
{
    if (!exists(path))
    {
        console::error(std::format("File not found at: {}", path));
        return std::string();
    }

    // get the file to read from
    std::fstream file(path, std::ios::in);

	if (!file.is_open())
	{
		console::error(std::format("Failed to open file at: {}", path));
		return std::string();
	}

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

Node minty::file::read_node(std::string const& path)
{
	std::vector<std::string> lines = file::read_all_lines(path);

	int indent = 0;
	std::string key;
	std::string value;

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

	for (std::string line : lines)
	{
		// skip empty/whitespace/comment lines
		if (line.size() == 0 || line.find_first_not_of(" \t\n\v\f\r") == std::string::npos || line.front() == '#' || line.front() == ':')
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

		if (split == std::string::npos)
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

bool minty::file::exists(std::string const& path)
{
    return std::filesystem::exists(path);
}

std::string minty::file::name(std::string const& path)
{
	return std::filesystem::path(path).stem().string();
}

std::string minty::file::extension(std::string const& path)
{
	return std::filesystem::path(path).extension().string();
}

std::string minty::file::absolute(std::string const& path)
{
	return std::filesystem::absolute(path).string();
}

std::string minty::file::relative_name(std::string const& path)
{
	return std::filesystem::relative(path, std::filesystem::current_path()).string();
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
