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

	class Writer
		: public Object
	{
	private:
		Node& _node;
		void* _data;

	public:
		Writer(Node& node, void* const data = nullptr);

		/// <summary>
		/// Gets the extra data given to this Writer.
		/// </summary>
		/// <returns>The extra data as a void pointer.</returns>
		void* get_data() const;

#pragma region Normal Writing

		void write(std::string const& name);

		void write(std::string const& name, Node const& node);

		void write(std::string const& name, ISerializable const* const value);

		void write(std::string const& name, std::string const& value);

		void write(std::string const& name, int const value);

		void write(std::string const& name, float const value);

		void write(std::string const& name, byte const value);

		void write(std::string const& name, size_t const value);

		void write(std::string const& name, Vector2 const& value);

		void write(std::string const& name, Vector3 const& value);

		void write(std::string const& name, Vector4 const& value);

		void write(std::string const& name, Vector2Int const& value);

		void write(std::string const& name, Vector3Int const& value);

		void write(std::string const& name, Vector4Int const& value);

		void write(std::string const& name, Quaternion const& value);

#pragma endregion

#pragma region Default Writing

		void write(std::string const& name, std::string const& value, std::string const& defaultValue);

		void write(std::string const& name, int const value, int const defaultValue);

		void write(std::string const& name, float const value, float const defaultValue);

		void write(std::string const& name, byte const value, byte const defaultValue);

		void write(std::string const& name, size_t const value, size_t const defaultValue);

		void write(std::string const& name, Vector2 const& value, Vector2 const& defaultValue);

		void write(std::string const& name, Vector3 const& value, Vector3 const& defaultValue);

		void write(std::string const& name, Vector4 const& value, Vector4 const& defaultValue);

		void write(std::string const& name, Vector2Int const& value, Vector2Int const& defaultValue);

		void write(std::string const& name, Vector3Int const& value, Vector3Int const& defaultValue);

		void write(std::string const& name, Vector4Int const& value, Vector4Int const& defaultValue);

		void write(std::string const& name, Quaternion const& value, Quaternion const& defaultValue);

#pragma endregion

	public:
		friend std::string to_string(Writer const& value);
	};
}