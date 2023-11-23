#include "pch.h"
#include "M_Rendering_MaterialTemplateBuilder.h"

using namespace minty;
using namespace minty::rendering;

minty::rendering::MaterialTemplateBuilder::MaterialTemplateBuilder(std::vector<ID> const& shaderPassIds)
    : _shaderPassIds(shaderPassIds)
    , _defaultValues()
{}

void minty::rendering::MaterialTemplateBuilder::emplace_default_value(std::string const& name, Dynamic const& value)
{
    _defaultValues[name] = value;
}

std::vector<ID> const& minty::rendering::MaterialTemplateBuilder::get_shader_pass_ids() const
{
    return _shaderPassIds;
}

std::unordered_map<std::string, Dynamic> const& minty::rendering::MaterialTemplateBuilder::get_default_values() const
{
    return _defaultValues;
}
