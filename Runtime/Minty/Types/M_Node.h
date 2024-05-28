#pragma once

#include "Minty/Types/M_Types.h"
#include "Minty/Types/M_UUID.h"
#include <vector>
#include <unordered_map>

namespace Minty
{
	/// <summary>
	/// Holds a bit of serialized data.
	/// </summary>
	class Node
	{
	private:
		/// <summary>
		/// The name of this Node.
		/// </summary>
		String _name;

		/// <summary>
		/// The data stored within this Node.
		/// </summary>
		String _data;

		/// <summary>
		/// A list of all child nodes on this Node.
		/// </summary>
		std::vector<Node> _children;

		/// <summary>
		/// A map of all names to child nodes for quick lookup.
		/// </summary>
		std::unordered_map<String, std::vector<size_t>> _lookup;

	public:
		Node() = default;

		Node(String const& name, String const& data = "")
			: _name(name)
			, _data(data)
			, _children()
			, _lookup()
		{}

		String const& get_name() const
		{
			return _name;
		}

		void set_name(String const& name)
		{
			_name = name;
		}

		String const& get_data() const
		{
			return _data;
		}

		void set_data(String const& data)
		{
			_data = data;
		}

		/// <summary>
		/// Gets the Node string as if it were serialized.
		/// </summary>
		/// <returns></returns>
		String get_node_string() const;

		/// <summary>
		/// Gets the Node string and its children as if it were serialized.
		/// </summary>
		/// <returns></returns>
		std::vector<String> get_formatted(bool const toplevel = false) const;

		String get_formatted_string() const;

		std::vector<Node>& get_children();

		std::vector<Node> const& get_children() const;

		bool has_name() const;

		/// <summary>
		/// Checks if this Node has any data.
		/// </summary>
		/// <returns>True if data is not empty.</returns>
		bool has_data() const;

		/// <summary>
		/// Checks if this Node has any children.
		/// </summary>
		/// <returns>True if children is not empty.</returns>
		bool has_children() const;

		void add_child(Node const& node);

#pragma region To

	public:
		/// <summary>
		/// Converts this Node's data to a string.
		/// </summary>
		String const& to_string() const;

		/// <summary>
		/// Converts this Node's data to a byte.
		/// </summary>
		Byte to_byte(Byte const defaultValue = 0) const;

		/// <summary>
		/// Converts this Node's data to an int.
		/// </summary>
		int to_int(int const defaultValue = 0) const;

		/// <summary>
		/// Converts this Node's data to an ID.
		/// </summary>
		ID to_id(ID const defaultValue = 0) const;

		UUID to_uuid() const;

		/// <summary>
		/// Converts this Node's data to an unsigned int.
		/// </summary>
		unsigned int to_uint(unsigned int const defaultValue = 0) const;

		/// <summary>
		/// Converts this Node's data to a size_t.
		/// </summary>
		size_t to_size(size_t const defaultValue = 0) const;

		/// <summary>
		/// Converts this Node's data to a float.
		/// </summary>
		float to_float(float const defaultValue = 0.0f) const;

		/// <summary>
		/// Converts this Node's data to a bool.
		/// </summary>
		bool to_bool(bool const defaultValue = false) const;

#pragma endregion

#pragma region Find

	public:
		/// <summary>
		/// Searches for a child node with the given name.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <returns>A pointer to the first child node with the given name, or nullptr if none found.</returns>
		Node* find(String const& name);

		/// <summary>
		/// Searches for a child node with the given name.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <returns>A pointer to the first child node with the given name, or nullptr if none found.</returns>
		Node const* find(String const& name) const;

		/// <summary>
		/// Finds a list of all the nodes with the given name.
		/// </summary>
		/// <param name="name">The name of the children nodes.</param>
		/// <returns>A pointer to the vector of child nodes with the given name, or nullptr if none found.</returns>
		std::vector<Node const*> find_all(String const& name) const;

#pragma endregion

	public:
		static Node parse(String const& text);

		static Node parse(std::vector<String> const& lines);

	public:
		friend String to_string(Node const& value);
	};
}