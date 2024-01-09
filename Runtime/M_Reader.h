#pragma once

#include "M_Types.h"
#include "M_Object.h"
#include "M_Node.h"
#include "M_Vector.h"
#include "M_Quaternion.h"
#include <map>
#include <vector>

namespace minty
{
	class ISerializable;

	/// <summary>
	/// Reads data from a serialized Node.
	/// </summary>
	class Reader
		: public Object
	{
	private:
		Node const& _node;
		void* _data;

	public:
		/// <summary>
		/// Creates a new Reader that will read from the given Node.
		/// </summary>
		/// <param name="node">The Node to read from.</param>
		/// <param name="data">The user data that can be referenced while reading.</param>
		Reader(Node const& node, void* const data = nullptr);

		/// <summary>
		/// Gets the root node that this Reader is using.
		/// </summary>
		/// <returns>A pointer to the root node.</returns>
		Node const& get_node() const;

		/// <summary>
		/// Gets the child node with the given name from the root node.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <returns>The child node, or null if it does not exist.</returns>
		Node const* get_node(String const& name) const;

		/// <summary>
		/// Gets the extra data given to this Reader.
		/// </summary>
		/// <returns>The extra data as a void pointer.</returns>
		void* get_data() const;

		/// <summary>
		/// Checks if a child with the given name exists.
		/// </summary>
		/// <param name="name">The name to check.</param>
		/// <returns>True if a child with the given name exists.</returns>
		bool exists(String const& name) const;

#pragma region Reading

		void read_object(String const& name, ISerializable* const value) const;

		String read_string(String const& name, String const& defaultValue = "") const;

		int read_int(String const& name, int const defaultValue = 0) const;

		unsigned int read_uint(String const& name, unsigned int const defaultValue = 0) const;

		ID read_id(String const& name, ID const defaultValue = ERROR_ID) const;

		float read_float(String const& name, float const defaultValue = 0.0f) const;

		Byte read_byte(String const& name, Byte const defaultValue = 0) const;

		size_t read_size(String const& name, size_t const defaultValue = 0) const;

		Vector2 read_vector2(String const& name, Vector2 const& defaultValue = Vector2()) const;

		Vector3 read_vector3(String const& name, Vector3 const& defaultValue = Vector3()) const;

		Vector4 read_vector4(String const& name, Vector4 const& defaultValue = Vector4()) const;

		Vector2Int read_vector2int(String const& name, Vector2Int const& defaultValue = Vector2Int()) const;

		Vector3Int read_vector3int(String const& name, Vector3Int const& defaultValue = Vector3Int()) const;

		Vector4Int read_vector4int(String const& name, Vector4Int const& defaultValue = Vector4Int()) const;

		Quaternion read_quaternion(String const& name, Quaternion const& defaultValue = Quaternion()) const;

#pragma endregion

	public:
		friend String to_string(Reader const& value);
	};
}