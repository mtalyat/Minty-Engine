#include "pch.h"
#include "M_Material.h"

#include "M_Shader.h"

#include "M_Renderer.h"
#include "M_Console.h"
#include "M_Color.h"
#include "M_Error.h"

using namespace minty;
using namespace minty::rendering;

minty::Material::Material(ID const id, void const* const data, rendering::MaterialBuilder const& builder, Renderer& renderer)
	: RendererObject::RendererObject(renderer)
	, _shaderId(builder.get_shader_id())
	, _id(id)
	, _data(nullptr)
{
	set(data);
}

void minty::Material::destroy()
{
	if (_data)
	{
		free(_data);
	}
}

ID minty::Material::get_shader_id() const
{
	return _shaderId;
}

void minty::Material::set(void const* const data)
{
	// get shader
	Shader& shader = _renderer.get_shader(_shaderId);

	// get element size
	size_t size = shader.get_material_size();

	// copy to local data
	if (!_data)
	{
		_data = malloc(size);

		if (!_data)
		{
			console::error("Failed to malloc Material data.");
			return;
		}
	}
	memcpy(_data, data, size);

	// update constant at material position
	shader.update_uniform_constant("materials", data, size, _id * size);
}

void* minty::Material::get() const
{
	return _data;
}
