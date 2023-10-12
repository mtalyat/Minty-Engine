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
{}

minty::Material::~Material()
{
	for (auto const& pair : _values)
	{
		free(pair.second.data);
	}
	_values.clear();
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
}

ID minty::Material::get_shader_id() const
{
	return _shaderId;
}

void minty::Material::bind(VkCommandBuffer const commandBuffer) const
{
	Shader const& shader = _renderer.get_shader(_shaderId);

	// bind shader
	shader.bind(commandBuffer);

	// update self

	// update all constant values
	for (auto const& pair : _values)
	{
		shader.update_uniform_constant(pair.first, pair.second.data, pair.second.size);
	}
}