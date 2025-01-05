#include "pch.h"
#include "Node.h"

#include "Minty/Core/Console.h"

using namespace Minty;

String Minty::Node::get_data_as_string() const
{
    // if no data, return empty string
    if (!m_data.size())
    {
        return "";
    }

    std::vector<Char> data(m_data.size() + 1);
    data[m_data.size()] = 0;
    memcpy(data.data(), m_data.data(), m_data.size());
    return String(data.data());
}

Bool Minty::Node::add_child(String const& name, Node const& node)
{
    if (!name.empty() && has_child(name))
    {
        // cannot add child with the same name
        return false;
    }

    Size index = m_children.size();

    m_children.push_back(node);

    if (!name.empty())
    {
        m_lookup.emplace(name, index);
    }

    return true;
}

void Minty::Node::print() const
{
    // print recursively until no more children
    std::vector<std::pair<Size, const Node*>> stack;
    stack.push_back({ 0, this });
    while (!stack.empty())
    {
        auto const [indent, node] = stack.back();
        stack.pop_back();

        String dataString = node->get_data_as_string();

        // print self
        if (node->get_name().empty())
        {
            // no name: print bullet point
            Console::log(std::format("{}- {}", String(indent, '\t'), node->get_data_as_string()));
        }
        else if (dataString.empty())
        {
            // no data: print just name
            Console::log(std::format("{}{}", String(indent, '\t'), node->get_name()));
        }
        else
        {
            // name and data
            Console::log(std::format("{}{}: {}", String(indent, '\t'), node->get_name(), dataString));
        }

        // add children in reverse order
        const std::vector<Node>& children = node->get_children();

        for (auto it = children.rbegin(); it != children.rend(); it++)
        {
            stack.push_back({ indent + 1, &(*it) });
        }
    }
}
