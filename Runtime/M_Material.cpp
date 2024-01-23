#include "pch.h"
#include "M_Material.h"

#include "M_RenderEngine.h"
#include "M_RenderSystem.h"
#include "M_Scene.h"

using namespace minty;
using namespace minty::rendering;

minty::Material::Material()
	: rendering::RenderObject()
	, _templateId(ERROR_ID)
	, _passDescriptorSets()
{}

minty::Material::Material(rendering::MaterialBuilder const& builder, RenderEngine& renderer)
	: rendering::RenderObject(renderer)
	, _templateId(builder.templateId)
	, _passDescriptorSets()
{
	RenderSystem* renderSystem = renderer.get_scene()->get_system_registry().find<RenderSystem>();

	MINTY_ASSERT(renderSystem != nullptr, "Material(): renderSystem cannot be null.");

	// use template to generate descriptor sets
	auto const& materialTemplate = renderSystem->get_material_template(_templateId);
	auto const& passIds = materialTemplate.get_shader_pass_ids();

	auto const& defaultValues = materialTemplate.get_default_values();

	for (auto const passId : passIds)
	{
		// get the shader pass
		auto const& shaderPass = renderSystem->get_shader_pass(passId);

		// get the shader that the pass belongs to
		auto const shaderId = shaderPass.get_shader_id();
		auto& shader = renderSystem->get_shader(shaderId);

		// get the descriptor set for the pass
		DescriptorSet descriptorSet = shader.create_descriptor_set(DESCRIPTOR_SET_MATERIAL, false);
		
		// set all values
		for (auto const& defaultValue : defaultValues)
		{
			auto const& found = builder.values.find(defaultValue.first);
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

void minty::Material::destroy()
{
	_templateId = ERROR_ID;
	_passDescriptorSets.clear();
}

ID minty::Material::get_template_id() const
{
	return _templateId;
}

DescriptorSet const& minty::Material::get_descriptor_set(uint32_t const pass) const
{
	return _passDescriptorSets.at(pass);
}

String minty::to_string(Material const& value)
{
	return std::format("Material()");
}
