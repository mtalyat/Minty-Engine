#pragma once

#include "M_Types.h"
#include "M_Object.h"
#include "M_SerializedNode.h"
#include <map>
#include <vector>

namespace minty
{
	class ISerializable;

	class Writer
		: public Object
	{
	private:
		SerializedNode& _node;

	public:
		Writer(SerializedNode& node);

		void write(std::string const& name);

		void write(std::string const& name, ISerializable const* const value);

		void write(std::string const& name, std::string const& value);

		void write(std::string const& name, int const value);

		void write(std::string const& name, float const value);

		void write(std::string const& name, byte const value);
	};
}