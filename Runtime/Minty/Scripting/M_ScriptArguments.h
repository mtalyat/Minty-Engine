#pragma once

#include "Minty/Types/M_Types.h"
#include "Minty/Core/M_Macros.h"
#include <vector>

namespace Minty
{
	class ScriptArguments
	{
	private:
		std::vector<void*> _values;

	public:
		ScriptArguments(std::vector<void*> const& values)
			: _values(values) {}

		std::vector<void*> const& get_values() const { return _values; }

		std::vector<void*>& get_values() { return _values; }

		size_t size() const { return _values.size(); }
	};
}