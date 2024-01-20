#pragma once

#include "M_Types.h"
#include <string>
#include <vector>
#include <map>

namespace minty
{
	/// <summary>
	/// Holds a bit of serialized data.
	/// </summary>
	struct Node
	{
		/// <summary>
		/// The data stored within this Node.
		/// </summary>
		String data;

		/// <summary>
		/// The child nodes. Organized by name and a list of each node with that name.
		/// </summary>
		std::map<String, std::vector<Node>> children;

#pragma region To

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
		std::vector<Node>* find_all(String const& name);

		/// <summary>
		/// Finds a list of all the nodes with the given name.
		/// </summary>
		/// <param name="name">The name of the children nodes.</param>
		/// <returns>A pointer to the vector of child nodes with the given name, or nullptr if none found.</returns>
		std::vector<Node> const* find_all(String const& name) const;

#pragma endregion

		/// <summary>
		/// console::logs this Node tree.
		/// </summary>
		/// <param name="indent">The base indent for this Node.</param>
		void print(int const indent = 0) const;

	public:
		friend String to_string(Node const& value);
	};
}