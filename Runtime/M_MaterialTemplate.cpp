#include "pch.h"
#include "M_MaterialTemplate.h"

#include "M_Shader.h"

#include "M_RenderEngine.h"
#include "M_Console.h"

using namespace minty;
using namespace minty;

minty::MaterialTemplate::MaterialTemplate()
	: Asset()
	, _shaderPasses()
	, _defaultValues()
{}

minty::MaterialTemplate::MaterialTemplate(MaterialTemplateBuilder const& builder, Runtime& engine)
	: Asset(builder.id, builder.path, engine)
	, _shaderPasses(builder.shaderPasses)
	, _defaultValues(builder.defaultValues)
{}

minty::MaterialTemplate::~MaterialTemplate()
{
	destroy();
}

void minty::MaterialTemplate::destroy()
{
	_shaderPasses.clear();
	_defaultValues.clear();
}

std::vector<ShaderPass*> const& minty::MaterialTemplate::get_shader_passes() const
{
	return _shaderPasses;
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
