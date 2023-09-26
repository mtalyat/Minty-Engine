#pragma once

#include "M_Types.h"
#include "M_Object.h"
#include "M_SerializedNode.h"
#include <map>
#include <vector>

namespace minty
{
	class ISerializable;

	class Reader
		: public Object
	{
	private:
		SerializedNode const& _node;

	public:
		Reader(SerializedNode const& node);

		/// <summary>
		/// Gets the root node that this Reader is using.
		/// </summary>
		/// <returns>A pointer to the root node.</returns>
		SerializedNode const* get_node() const;

		/// <summary>
		/// Gets the child node with the given name from the root node.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <returns>The child node, or null if it does not exist.</returns>
		SerializedNode const* get_node(std::string const& name) const;

		void read_object(std::string const& name, ISerializable* const value) const;

		std::string read_string(std::string const& name, std::string const& defaultValue = "") const;

		int read_int(std::string const& name, int const defaultValue = 0) const;

		float read_float(std::string const& name, float const defaultValue = 0.0f) const;

		byte read_byte(std::string const& name, byte const defaultValue = 0) const;
	};
}