#include "pch.h"
#include "MaterialTemplate.h"

#include "Minty/Render/Renderer.h"

using namespace Minty;

void Minty::MaterialTemplate::on_bind()
{
    static int temp = 0;
    if (temp) return;
    temp++;
    // TODO: these will not change during runtime: change so it collects the values on creation, then uses those values here (still need cargo for mat template stuff)
    DynamicContainer container;
    for (auto const& [name, cargo] : m_values)
    {
        // compile value to one array of byte data
        container.clear();
        cargo.collect(container);

        // set the input
        m_shader->set_input(name, container.data());
    }
}

Owner<MaterialTemplate> Minty::MaterialTemplate::create(const MaterialTemplateBuilder& builder)
{
    return Owner<MaterialTemplate>(builder);
}
