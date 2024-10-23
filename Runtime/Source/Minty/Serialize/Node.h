#pragma once

#include "Minty/Data/DynamicContainer.h"
#include "Minty/Core/String.h"
#include "Minty/Core/Types.h"
#include <unordered_map>
#include <vector>

namespace Minty
{
	class Node
	{
	private:
		String m_name;

		DynamicContainer m_data;

		std::vector<Node> m_children;

		std::unordered_map<String, Size> m_lookup;

	public:
		Node()
			: m_name(), m_data(), m_children(), m_lookup()
		{}

		Node(const String& name, const DynamicContainer& data)
			: m_name(name), m_data(data), m_children(), m_lookup()
		{}

		Node(const String& name, const void* const data, const Size size)
			: m_name(name), m_data(size), m_children(), m_lookup()
		{
			m_data.set(data, size);
		}

		Node& operator=(Node const& other)
		{
			if (this != &other)
			{
				m_name = other.m_name;
				m_data = other.m_data;
				m_children = other.m_children;
				m_lookup = other.m_lookup;
			}

			return *this;
		}

		Node(Node&& other) noexcept
			: m_name(std::move(other.m_name))
			, m_data(std::move(other.m_data))
			, m_children(std::move(other.m_children))
			, m_lookup(std::move(other.m_lookup))
		{}

		Node& operator=(Node&& other) noexcept
		{
			if (this != &other)
			{
				m_name = std::move(other.m_name);
				m_data = std::move(other.m_data);
				m_children = std::move(other.m_children);
				m_lookup = std::move(other.m_lookup);
			}

			return *this;
		}

		~Node() = default;

		Node(Node const& other)
			: m_name(other.m_name), m_data(other.m_data), m_children(other.m_children), m_lookup(other.m_lookup)
		{}

		const String& get_name() const { return m_name; }

		void set_name(const String& name) { m_name = name; }

		const Container& get_data() const { return m_data; }

		String get_data_as_string() const;

		void set_data(const void* const data, const Size size) { m_data.set(data, size); }

		void set_data(String const& data) { set_data(data.data(), data.size()); }

		const Node& get_child(const Size index) const { return m_children.at(index); }

		const Node& get_child(const String& name) const { return m_children.at(m_lookup.at(name)); }

		Bool has_child(const Size index) const { return index < m_children.size(); }

		Bool has_child(const String& name) const { return m_lookup.contains(name); }

		std::vector<Node>& get_children() { return m_children; }

		const std::vector<Node>& get_children() const { return m_children; }

		Size get_children_count() const { return m_children.size(); }

		Bool has_children() const { return static_cast<Bool>(m_children.size()); }

		Bool add_child(const String& name, const Node& node);

		void print() const;
	};
}