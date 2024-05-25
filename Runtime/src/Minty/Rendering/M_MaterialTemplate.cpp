#include "pch.h"
#include "Minty/Rendering/M_MaterialTemplate.h"

#include "Minty/Rendering/M_Shader.h"

#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Tools/M_Console.h"

using namespace Minty;
using namespace Minty;

Minty::MaterialTemplate::~MaterialTemplate()
{
	destroy();
}

void Minty::MaterialTemplate::destroy()
{
	_shaderPasses.clear();
	_defaultValues.clear();
}

Dynamic Minty::MaterialTemplate::get_defalt_value(String const& name) const
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

std::unordered_map<String, Dynamic> const& Minty::MaterialTemplate::get_default_values() const
{
	return _defaultValues;
}
