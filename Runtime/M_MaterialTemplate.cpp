#include "pch.h"
#include "M_MaterialTemplate.h"

#include "M_Shader.h"

#include "M_RenderEngine.h"
#include "M_Console.h"
#include "M_Color.h"
#include "M_Error.h"

using namespace minty;
using namespace minty::rendering;

minty::MaterialTemplate::MaterialTemplate(rendering::MaterialTemplateBuilder const& builder, RenderEngine& renderer)
	: RenderObject::RenderObject(renderer)
	, _shaderPassIds(builder.shaderPassIds)
	, _defaultValues(builder.defaultValues)
{}

void minty::MaterialTemplate::destroy()
{
	_shaderPassIds.clear();
	_defaultValues.clear();
}

std::vector<ID> const& minty::MaterialTemplate::get_shader_pass_ids() const
{
	return _shaderPassIds;
}

Dynamic minty::MaterialTemplate::get_defalt_value(std::string const& name) const
{
	auto const& found = _defaultValues.find(name);

	if (found != _defaultValues.end())
	{
		// value found
		return found->second;
	}

	// not found
	return Dynamic();
}

std::unordered_map<std::string, Dynamic> const& minty::MaterialTemplate::get_default_values() const
{
	return _defaultValues;
}

std::string minty::to_string(MaterialTemplate const& value)
{
	return std::format("MaterialTemplate()");
}
