#include "pch.h"
#include "M_Material.h"

#include "M_Shader.h"

#include "M_Renderer.h"
#include "M_Console.h"
#include "M_Color.h"
#include "M_Error.h"

using namespace minty;
using namespace minty::rendering;

minty::Material::Material(rendering::MaterialBuilder const& builder, Renderer& renderer)
	: RendererObject::RendererObject(renderer)
	, _shaderId(builder.get_shader_id())
	, _dirty()
	, _values()
{
	// copy over all values
	for (auto const& value : builder.get_values())
	{
		set(value.first, value.second.data, value.second.size);
	}
}

void minty::Material::destroy()
{
	for (auto const& pair : _values)
	{
		free(pair.second.data);
	}
	_values.clear();
}

bool minty::Material::is_dirty() const
{
	return _dirty;
}

void minty::Material::set(std::string const& name, void* const value, size_t const size)
{
	// create copy of the data
	void* dst = malloc(size);

	if (dst == nullptr)
	{
		error::abort("Failed to malloc material data.");
		return;
	}

	memcpy(dst, value, size);

	// check if old data
	auto const& found = _values.find(name);
	if (found != _values.end())
	{
		// old value exists, so free it
		free(found->second.data);
	}

	// set/replace values
	_values[name] = MaterialData
	{
		.data = dst,
		.size = size
	};

	// mark as dirty
	_dirty = true;
}

void* minty::Material::get(std::string const& name)
{
	auto const& found = _values.find(name);

	if (found == _values.end())
	{
		// not found
		return nullptr;
	}

	// found
	return found->second.data;
}

ID minty::Material::get_shader_id() const
{
	return _shaderId;
}

void minty::Material::apply()
{
	// get shader
	Shader& shader = _renderer.get_shader(_shaderId);
	
	// update all constant values
	for (auto const& pair : _values)
	{
		shader.update_uniform_constant_frame(pair.first, pair.second.data, pair.second.size);
	}

	// up to date
	_dirty = false;
}
