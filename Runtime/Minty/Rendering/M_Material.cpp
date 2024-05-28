#include "pch.h"
#include "Minty/Rendering/M_Material.h"

#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Rendering/M_RenderSystem.h"
#include "Minty/Scenes/M_Scene.h"

using namespace Minty;
using namespace Minty;

Minty::Material::Material(MaterialBuilder const& builder)
	: Asset(builder.id, builder.path)
	, _template(builder.materialTemplate)
	, _passDescriptorSets()
{
	// use template to generate descriptor sets
	auto const& materialTemplate = _template;
	auto const& passes = materialTemplate->get_shader_passes();

	auto const& defaultValues = materialTemplate->get_default_values();

	for (auto const pass : passes)
	{
		// get the shader that the pass belongs to
		auto const shader = pass->get_shader();

		// get the descriptor set for the pass
		DescriptorSet descriptorSet = shader->create_descriptor_set(DESCRIPTOR_SET_MATERIAL, false);
		
		// set all values
		for (auto const& defaultValue : defaultValues)
		{
			auto found = builder.values.find(defaultValue.first);
			if (found == builder.values.end())
			{
				// if no value exists, use a default value
				descriptorSet.set(defaultValue.first, defaultValue.second);
			}
			else
			{
				// if custom value exists, use it
				descriptorSet.set(found->first, found->second);
			}
		}

		// apply changes
		descriptorSet.apply();

		// add to pass descriptor sets
		_passDescriptorSets.push_back(descriptorSet);
	}
}

Minty::Material::~Material()
{
	destroy();
}

void Minty::Material::destroy()
{
	_template = nullptr;
	for (auto& set : _passDescriptorSets)
	{
		set.destroy();
	}
	_passDescriptorSets.clear();
}

DescriptorSet const& Minty::Material::get_descriptor_set(uint32_t const pass) const
{
	return _passDescriptorSets.at(pass);
}
