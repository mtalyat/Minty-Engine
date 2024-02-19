#include "pch.h"
#include "M_MaterialTemplate.h"

#include "M_MaterialTemplateBuilder.h"
#include "M_Shader.h"

#include "M_RenderEngine.h"
#include "M_Console.h"

using namespace minty;
using namespace minty;

minty::MaterialTemplate::MaterialTemplate()
	: RenderObject::RenderObject()
	, _shaderPassIds()
	, _defaultValues()
{}

minty::MaterialTemplate::MaterialTemplate(MaterialTemplateBuilder const& builder, Runtime& engine, ID const sceneId)
	: RenderObject::RenderObject(engine, sceneId)
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

Dynamic minty::MaterialTemplate::get_defalt_value(String const& name) const
{
	auto found = _defaultValues.find(name);

	if (found != _defaultValues.end())
	{
		// value found
		return found->second;
	}

	// not found
	return Dynamic();
}

std::unordered_map<String, Dynamic> const& minty::MaterialTemplate::get_default_values() const
{
	return _defaultValues;
}

String minty::to_string(MaterialTemplate const& value)
{
	return std::format("MaterialTemplate()");
}
