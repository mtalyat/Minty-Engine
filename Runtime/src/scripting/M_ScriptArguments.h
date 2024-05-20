#pragma once

#include "types/M_Types.h"
#include "core/M_Macros.h"
#include <vector>

namespace minty
{
	class ScriptArguments
	{
	private:
		std::vector<void*> _values;

	public:
		ScriptArguments(std::vector<void*> const& values);

		std::vector<void*> const& get_values() const;

		std::vector<void*>& get_values();

		size_t size() const;
	};
}