#pragma once

#include "Minty/Core/Type.h"
#include <vector>

namespace Minty
{
	/// <summary>
	/// Holds a collection of values used to pass into a Script function.
	/// </summary>
	class ScriptArguments
	{
	private:
		std::vector<void*> m_values;

	public:
		ScriptArguments()
			: m_values()
		{}

		ScriptArguments(std::vector<void*> const& values)
			: m_values(values)
		{}

		std::vector<void*> const& get_values() const { return m_values; }

		std::vector<void*>& get_values() { return m_values; }

		Size size() const { return m_values.size(); }
	};
}