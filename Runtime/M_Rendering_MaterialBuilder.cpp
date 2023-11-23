#include "pch.h"
#include "M_Rendering_MaterialBuilder.h"

using namespace minty;
using namespace minty::rendering;

minty::rendering::MaterialBuilder::MaterialBuilder(ID const templateId)
	: _templateId(templateId)
	, _values()
{}

ID minty::rendering::MaterialBuilder::get_template_id() const
{
	return _templateId;
}

std::unordered_map<std::string, Dynamic> const& minty::rendering::MaterialBuilder::get_values() const
{
	return _values;
}

void minty::rendering::MaterialBuilder::emplace_value(std::string const& name, Dynamic const& value)
{
	_values[name] = value;
}
