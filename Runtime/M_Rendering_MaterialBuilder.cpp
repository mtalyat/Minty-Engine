#include "pch.h"
#include "M_Rendering_MaterialBuilder.h"

using namespace minty;
using namespace minty::rendering;

minty::rendering::MaterialBuilder::MaterialBuilder(ID const shaderId)
    : _shaderId(shaderId)
{}

void minty::rendering::MaterialBuilder::set_shader_id(ID const id)
{
    _shaderId = id;
}

ID minty::rendering::MaterialBuilder::get_shader_id() const
{
    return _shaderId;
}
