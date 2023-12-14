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
		std::string data;

		/// <summary>
		/// The child nodes. Organized by name and a list of each node with that name.
		/// </summary>
		std::map<std::string, std::vector<Node>> children;

#pragma region To

		/// <summary>
		/// Converts this Node's data to a string.
		/// </summary>
		std::string const& to_string() const;

		/// <summary>
		/// Converts this Node's data to a byte.
		/// </summary>
		byte to_byte(byte const defaultValue = 0) const;

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
		Node* find(std::string const& name);

		/// <summary>
		/// Searches for a child node with the given name.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <returns>A pointer to the first child node with the given name, or nullptr if none found.</returns>
		Node const* find(std::string const& name) const;

		/// <summary>
		/// Finds a list of all the nodes with the given name.
		/// </summary>
		/// <param name="name">The name of the children nodes.</param>
		/// <returns>A pointer to the vector of child nodes with the given name, or nullptr if none found.</returns>
		std::vector<Node>* find_all(std::string const& name);

		/// <summary>
		/// Finds a list of all the nodes with the given name.
		/// </summary>
		/// <param name="name">The name of the children nodes.</param>
		/// <returns>A pointer to the vector of child nodes with the given name, or nullptr if none found.</returns>
		std::vector<Node> const* find_all(std::string const& name) const;

#pragma endregion

#pragma region Get

		/// <summary>
		/// Gets the value of a child node with the given name as a string, or returns the default value if no child found.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <param name="defaultValue">The default value to use if no child found.</param>
		/// <returns>The value found within the child node.</returns>
		std::string const& get_string(std::string const& name, std::string const& defaultValue = "") const;

		/// <summary>
		/// Gets the value of a child node with the given name as a byte, or returns the default value if no child found.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <param name="defaultValue">The default value to use if no child found.</param>
		/// <returns>The value found within the child node.</returns>
		byte get_byte(std::string const& name, byte const defaultValue = 0) const;

		/// <summary>
		/// Gets the value of a child node with the given name as an int, or returns the default value if no child found.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <param name="defaultValue">The default value to use if no child found.</param>
		/// <returns>The value found within the child node.</returns>
		int get_int(std::string const& name, int const defaultValue = 0) const;

		/// <summary>
		/// Gets the value of a child node with the given name as an ID, or returns the default value if no child found.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <param name="defaultValue">The default value to use if no child found.</param>
		/// <returns>The value found within the child node.</returns>
		ID get_id(std::string const& name, ID const defaultValue = 0) const;

		/// <summary>
		/// Gets the value of a child node with the given name as an unsigned int, or returns the default value if no child found.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <param name="defaultValue">The default value to use if no child found.</param>
		/// <returns>The value found within the child node.</returns>
		unsigned int get_uint(std::string const& name, unsigned int const defaultValue = 0) const;

		/// <summary>
		/// Gets the value of a child node with the given name as a size, or returns the default value if no child found.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <param name="defaultValue">The default value to use if no child found.</param>
		/// <returns>The value found within the child node.</returns>
		size_t get_size(std::string const& name, size_t const defaultValue = 0) const;

		/// <summary>
		/// Gets the value of a child node with the given name as a float, or returns the default value if no child found.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <param name="defaultValue">The default value to use if no child found.</param>
		/// <returns>The value found within the child node.</returns>
		float get_float(std::string const& name, float const defaultValue = 0.0f) const;

		/// <summary>
		/// Gets the value of a child node with the given name as a bool, or returns the default value if no child found.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <param name="defaultValue">The default value to use if no child found.</param>
		/// <returns>The value found within the child node.</returns>
		bool get_bool(std::string const& name, bool const defaultValue = false) const;

#pragma endregion

		/// <summary>
		/// console::logs this Node tree.
		/// </summary>
		/// <param name="indent">The base indent for this Node.</param>
		void print(int const indent = 0) const;

	public:
		friend std::string to_string(Node const& value);
	};
}