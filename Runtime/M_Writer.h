#pragma once

#include "M_Types.h"
#include "M_Object.h"
#include "M_Node.h"
#include "M_Vector.h"
#include <map>
#include <vector>
#include "M_Quaternion.h"

namespace minty
{
	class ISerializable;

	/// <summary>
	/// Writes data to a serialized Node.
	/// </summary>
	class Writer
		: public Object
	{
	private:
		Node& _node;
		void* _data;

	public:
		/// <summary>
		/// Creates a new Writer that will write to the given Node.
		/// </summary>
		/// <param name="node">The Node to write to.</param>
		/// <param name="data">The user data that can be referenced while writing.</param>
		Writer(Node& node, void* const data = nullptr);

		/// <summary>
		/// Gets the root node that this Writer is using.
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
		/// Gets the extra data given to this Writer.
		/// </summary>
		/// <returns>The extra data as a void pointer.</returns>
		void* get_data() const;

		/// <summary>
		/// Checks if a child with the given name exists.
		/// </summary>
		/// <param name="name">The name to check.</param>
		/// <returns>True if a child with the given name exists.</returns>
		bool exists(String const& name) const;

#pragma region Writing

		void write(String const& name);

		void write(String const& name, Node const& node);

		void write(String const& name, ISerializable const* const value);

		void write(String const& name, String const& value);

		void write(String const& name, int const value);

		void write(String const& name, unsigned int const value);

		void write(String const& name, float const value);

		void write(String const& name, Byte const value);

		void write(String const& name, size_t const value);

		void write(String const& name, Vector2 const& value);

		void write(String const& name, Vector3 const& value);

		void write(String const& name, Vector4 const& value);

		void write(String const& name, Vector2Int const& value);

		void write(String const& name, Vector3Int const& value);

		void write(String const& name, Vector4Int const& value);

		void write(String const& name, Quaternion const& value);

#pragma endregion

#pragma region Default Writing

		void write(String const& name, String const& value, String const& defaultValue);

		void write(String const& name, int const value, int const defaultValue);

		void write(String const& name, unsigned int const value, unsigned int const defaultValue);

		void write(String const& name, float const value, float const defaultValue);

		void write(String const& name, Byte const value, Byte const defaultValue);

		void write(String const& name, size_t const value, size_t const defaultValue);

		void write(String const& name, Vector2 const& value, Vector2 const& defaultValue);

		void write(String const& name, Vector3 const& value, Vector3 const& defaultValue);

		void write(String const& name, Vector4 const& value, Vector4 const& defaultValue);

		void write(String const& name, Vector2Int const& value, Vector2Int const& defaultValue);

		void write(String const& name, Vector3Int const& value, Vector3Int const& defaultValue);

		void write(String const& name, Vector4Int const& value, Vector4Int const& defaultValue);

		void write(String const& name, Quaternion const& value, Quaternion const& defaultValue);

#pragma endregion

	public:
		friend String to_string(Writer const& value);
	};
}