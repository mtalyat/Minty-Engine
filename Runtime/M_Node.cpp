#include "pch.h"
#include "M_Node.h"

#include "M_Console.h"
#include "M_File.h"
#include "M_Parse.h"
#include "M_Text.h"
#include <vector>
#include <algorithm>

using namespace minty;

minty::Node::Node()
    : _name()
    , _data()
    , _children()
    , _lookup()
{}

minty::Node::Node(String const& name, String const& data)
    : _name(name)
    , _data(data)
    , _children()
    , _lookup()
{}

String const& minty::Node::get_name() const
{
	return _name;
}

void minty::Node::set_name(String const& name)
{
	_name = name;
}

String const& minty::Node::get_data() const
{
	return _data;
}

void minty::Node::set_data(String const& data)
{
	_data = data;
}

String minty::Node::get_node_string() const
{
    if (has_data())
    {
        if (has_name())
        {
            // name and data
            return std::format("{}: {}", get_name(), get_data());
        }
        else
        {
            // only data
            return std::format("- {}", get_data());
        }
    }
    else
    {
        if (has_name())
        {
            // only name
            return get_name();
        }
        else
        {
            // none
            return "";
        }
    }
}

// TODO: make iterative
static void get_formatted_recursive(std::vector<String>& list, int indent, Node const& node)
{
    list.push_back(String(static_cast<size_t>(std::max(0, indent)), '\t').append(node.get_node_string()));

    for (Node const& child : node.get_children())
    {
        get_formatted_recursive(list, indent + 1, child);
    }
}

std::vector<String> minty::Node::get_formatted(bool const toplevel) const
{
    std::vector<String> result;

    get_formatted_recursive(result, toplevel ? -1 : 0, *this);

    // if the first line is a value only (ex. "- value"), switch to ": value" to signify an ID or whatever
    if (!result.empty() && !has_name() && has_data())
    {
        result[0] = std::format(": {}", get_data());
    }

    return result;
}

String minty::Node::get_formatted_string() const
{
    return Text::join(get_formatted(), "\n");
}

std::vector<Node>& minty::Node::get_children()
{
	return _children;
}

std::vector<Node> const& minty::Node::get_children() const
{
	return _children;
}

bool minty::Node::has_name() const
{
    return static_cast<bool>(_name.size());
}

bool minty::Node::has_data() const
{
	return static_cast<bool>(_data.size());
}

bool minty::Node::has_children() const
{
	return static_cast<bool>(_children.size());
}

void minty::Node::add_child(Node const& node)
{
    // get new child node index
    size_t index = _children.size();

    // add to children
    _children.push_back(node);

    // add to lookup
    auto found = _lookup.find(node.get_name());
    if (found == _lookup.end())
    {
        // new name
        _lookup.emplace(node.get_name(), std::vector<size_t> { index });
    }
    else
    {
        // old name
        found->second.push_back(index);
    }
}

String const& minty::Node::to_string() const
{
	return _data;
}

Byte minty::Node::to_byte(Byte const defaultValue) const
{
	Byte out;
	if (Parse::try_byte(_data, out))
	{
		return out;
	}

	return defaultValue;
}

int minty::Node::to_int(int const defaultValue) const
{
	int out;
	if (Parse::try_int(_data, out))
	{
		return out;
	}

	return defaultValue;
}

ID minty::Node::to_id(ID const defaultValue) const
{
	ID out;
	if (Parse::try_id(_data, out))
	{
		return out;
	}

	return defaultValue;
}

UUID minty::Node::to_uuid() const
{
    UUID out = INVALID_UUID;
    Parse::try_uuid(_data, out);
    return out;
}

unsigned int minty::Node::to_uint(unsigned int const defaultValue) const
{
	unsigned int out;
	if (Parse::try_uint(_data, out))
	{
		return out;
	}

	return defaultValue;
}

size_t minty::Node::to_size(size_t const defaultValue) const
{
	size_t out;
	if (Parse::try_size(_data, out))
	{
		return out;
	}

	return defaultValue;
}

float minty::Node::to_float(float const defaultValue) const
{
	float out;
	if (Parse::try_float(_data, out))
	{
		return out;
	}

	return defaultValue;
}

bool minty::Node::to_bool(bool const defaultValue) const
{
	bool out;
	if (Parse::try_bool(_data, out))
	{
		return out;
	}

	return defaultValue;
}

Node* minty::Node::find(String const& name)
{
	auto found = _lookup.find(name);

	if (found != _lookup.end())
	{
		return &_children.at(found->second.front());
	}

	return nullptr;
}

Node const* minty::Node::find(String const& name) const
{
	auto found = _lookup.find(name);

	if (found != _lookup.end())
	{
		return &_children.at(found->second.front());
	}

	return nullptr;
}

std::vector<Node const*> minty::Node::find_all(String const& name) const
{
	std::vector<Node const*> list;

	auto found = _lookup.find(name);

	if (found != _lookup.end())
	{
		list.reserve(found->second.size());

		for (size_t const index : found->second)
		{
			list.push_back(&_children.at(index));
		}
	}

	return list;
}

Node minty::Node::parse(String const& text)
{
    return parse(Text::split(text, '\n'));
}

Node minty::Node::parse(std::vector<String> const& lines)
{
    int indent = 0;
    String key;
    String value;

    Node root;

    // if no lines, node is empty
    if (lines.empty())
    {
        return root;
    }

    // if first line starts with a ": ", then that is the data for the root node
    if (lines.front().starts_with(": "))
    {
        root._data = lines.front().substr(2, lines.front().size() - 2);
    }

    std::vector<Node*> nodeStack;
    nodeStack.push_back(&root);
    Node* node = nodeStack.back();

    int const SPACES_PER_TAB = 4;

    for (String line : lines)
    {
        // skip empty/whitespace/comment lines
        size_t solidIndex = line.find_first_not_of(" \t\n\v\f\r");
        if (line.size() == 0 || solidIndex == String::npos || line.front() == '#' || line.front() == ':')
        {
            continue;
        }

        // count number of tabs (indents)

        int spaces = 0;
        for (char const c : line)
        {
            if (c == ' ')
            {
                // add once space
                spaces += 1;
            }
            else if(c == '\t')
            {
                // multiple sapces
                spaces += SPACES_PER_TAB;
            }
            else
            {
                // done
                break;
            }
        }
        int i = spaces / SPACES_PER_TAB;

        int indentChange = i - indent;

        // if new indent is too deep, ignore
        if (indentChange > 1)
        {
            MINTY_WARN_FORMAT("Discarding line, invalid indent change of {}: {}", indentChange, line);
            continue;
        }

        // check change in index
        if (indentChange > 0)
        {
            nodeStack.push_back(&node->_children.back());

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
            line = line.substr(solidIndex, line.size() - solidIndex);
        }

        Node newNode;

        if (line.starts_with("- "))
        {
            // bullet point, use "" as key and the whole line as the value
            key = "";
            value = line.substr(2, line.size() - 2);
            newNode.set_data(value);
        }
        else
        {
            // split by colon, if there is one
            size_t split = line.find_first_of(':');

            if (split == String::npos)
            {
                // no split, just key
                key = line;
            }
            else
            {
                // split: implies key: value
                key = line.substr(0, split);
                // ignore ": "
                size_t size = line.size() - split - 2;
                if (split < line.size() - 2)
                {
                    // something on other side of ": "
                    value = line.substr(split + 2, size);
                }
                else
                {
                    // nothing on other side of the ": "
                    value = "";
                }
                newNode.set_data(value);
            }
        }

        // set newNode name to key
        newNode.set_name(key);

        // get index of newNode
        size_t index = node->_children.size();

        // add node to children
        node->_children.push_back(newNode);

        // add node to lookup
        // if key name exists, add there, otherwise create new
        auto found = node->_lookup.find(key);
        if (found != node->_lookup.end())
        {
            // name exists
            found->second.push_back(index);
        }
        else
        {
            // name does not exist
            node->_lookup.emplace(key, std::vector<size_t>{ index });
        }
    }

    // root node should contain everything
    return root;
}

String minty::to_string(Node const& value)
{
    return value.get_node_string();
}
