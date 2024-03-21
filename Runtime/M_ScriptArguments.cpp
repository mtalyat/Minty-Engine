#include "pch.h"
#include "M_ScriptArguments.h"

using namespace minty;

minty::ScriptArguments::ScriptArguments(std::vector<void*> const& values)
	: _values(values)
{}

std::vector<void*> const& minty::ScriptArguments::get_values() const
{
	return _values;
}

std::vector<void*>& minty::ScriptArguments::get_values()
{
	return _values;
}

size_t minty::ScriptArguments::size() const
{
	return _values.size();
}
