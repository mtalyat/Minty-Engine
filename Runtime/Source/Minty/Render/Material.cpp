#include "pch.h"
#include "Material.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanMaterial.h"
#endif

using namespace Minty;

Minty::Material::Material(MaterialBuilder const& builder)
	: Asset(builder.id)
	, m_materialTemplate(builder.materialTemplate)
	, m_values(builder.values)
{
	MINTY_ASSERT(builder.materialTemplate != nullptr);

	Ref<Shader> shader = m_materialTemplate->get_shader();
	shader->register_material(*this);
}

Minty::Material::~Material()
{
	if (m_materialTemplate == nullptr)
	{
		return;
	}
	Ref<Shader> shader = m_materialTemplate->get_shader();

	if (shader != nullptr)
	{
		shader->unregister_material(*this);
	}
}

Bool Minty::Material::try_set_input(String const& name, void const* const data)
{
	Ref<MaterialTemplate> materialTemplate = get_template();
	MINTY_ASSERT(materialTemplate != nullptr);
	Ref<Shader> shader = materialTemplate->get_shader();
	MINTY_ASSERT(shader != nullptr);

	// set input, if there is an input with the given name
	if (shader->has_input(name))
	{
		set_input(name, data);

		return true;
	}

	return false;
}

Owner<Material> Minty::Material::create(const MaterialBuilder& builder)
{
#if defined(MINTY_VULKAN)
	return Owner<VulkanMaterial>(builder);
#else
	return Owner<Material>();
#endif
}
