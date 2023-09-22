#pragma once

#include "M_Types.h"
#include "M_Object.h"
#include "M_SerializedNode.h"
#include <map>
#include <vector>

namespace minty
{
	class ISerializable;

	class Serializer
		: public Object
	{
	private:
		SerializedNode& _node;

	protected:
		Serializer(SerializedNode& node);

	public:
#pragma region Write

		void write(std::string const& name, ISerializable const& value);
		
		void write(std::string const& name, std::string const& value);

		void write(std::string const& name, int const value);

		void write(std::string const& name, byte const value);

#pragma endregion

#pragma region Read

		void read_object(std::string const& name, ISerializable& value) const;

		std::string read_string(std::string const& name, std::string const& defaultValue = "") const;

		int read_int(std::string const& name, int const defaultValue = 0) const;

		byte read_byte(std::string const& name, byte const defaultValue = 0) const;

#pragma endregion

#pragma region Static
		
		static SerializedNode parse_file(std::string const& path);

#pragma endregion
	};
}