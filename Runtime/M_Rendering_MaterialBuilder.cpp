#include "pch.h"
#include "M_Rendering_MaterialBuilder.h"

#include "M_Error.h"

using namespace minty;

minty::rendering::MaterialBuilder::MaterialBuilder(ID const shaderId)
	: _shaderId(shaderId)
{}

minty::rendering::MaterialBuilder::~MaterialBuilder()
{
	for (auto const& pair : _values)
	{
		free(pair.second.data);
	}
}

void minty::rendering::MaterialBuilder::set_shader_id(ID const id)
{
	_shaderId = id;
}

void minty::rendering::MaterialBuilder::set(std::string const& name, void* const data, size_t const size)
{
	// check for existing
	if (_values.contains(name))
	{
		error::abort("MaterialBuilder already contains name.");
	}

	// make copy
	void* dst = malloc(size);
	if (dst == nullptr)
	{
		error::abort("Unable to malloc in MaterialBuilder.");
	}
	memcpy(dst, data, size);

	// add to list
	_values.emplace(name, MaterialData
		{
			.data = dst,
			.size = size,
		});
}

ID minty::rendering::MaterialBuilder::get_shader_id() const
{
	return _shaderId;
}

std::unordered_map<std::string, MaterialData> const& minty::rendering::MaterialBuilder::get_values() const
{
	return _values;
}