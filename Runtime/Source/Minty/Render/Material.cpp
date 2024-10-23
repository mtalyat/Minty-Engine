#include "pch.h"
#include "Material.h"

#include "Minty/Render/Renderer.h"

using namespace Minty;

void Minty::Material::on_bind()
{
    // get shader
    Ref<Shader> shader = m_materialTemplate->get_shader();

    // set all override values in the shader
    DynamicContainer container;
    for (auto const& [name, cargo] : m_values)
    {
        // compile value to one array of byte data
        container.clear();
        cargo.collect(container);

        // set the input
        shader->set_input(name, container.data());
    }
}

Owner<Material> Minty::Material::create(const MaterialBuilder& builder)
{
    return Owner<Material>(builder);
}
