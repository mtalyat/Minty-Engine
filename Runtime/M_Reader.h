#pragma once

#include "M_Types.h"
#include "M_Object.h"
#include "M_SerializedNode.h"
#include "M_Vector.h"
#include "M_Quaternion.h"
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

		Vector2 read_vector2(std::string const& name, Vector2 const& defaultValue = Vector2()) const;

		Vector3 read_vector3(std::string const& name, Vector3 const& defaultValue = Vector3()) const;

		Vector4 read_vector4(std::string const& name, Vector4 const& defaultValue = Vector4()) const;

		Vector2Int read_vector2int(std::string const& name, Vector2Int const& defaultValue = Vector2Int()) const;

		Vector3Int read_vector3int(std::string const& name, Vector3Int const& defaultValue = Vector3Int()) const;

		Vector4Int read_vector4int(std::string const& name, Vector4Int const& defaultValue = Vector4Int()) const;

		Quaternion read_quaternion(std::string const& name, Quaternion const& defaultValue = Quaternion()) const;
	};
}