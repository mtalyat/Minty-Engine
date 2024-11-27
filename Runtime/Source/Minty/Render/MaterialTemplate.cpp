#include "pch.h"
#include "MaterialTemplate.h"

#include "Minty/Render/Renderer.h"

using namespace Minty;

Owner<MaterialTemplate> Minty::MaterialTemplate::create(MaterialTemplateBuilder const& builder)
{
    return Owner<MaterialTemplate>(builder);
}
