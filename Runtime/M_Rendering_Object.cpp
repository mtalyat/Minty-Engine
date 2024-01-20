#include "pch.h"
#include "M_Rendering_Object.h"

#include "M_RenderEngine.h"

using namespace minty;
using namespace minty::rendering;

RenderObject::RenderObject(RenderEngine* renderer)
	: _renderer(renderer)
{}

RenderObject::~RenderObject()
{}

void minty::rendering::RenderObject::set_renderer(RenderEngine* renderer)
{
	_renderer = renderer;
}

RenderEngine* minty::rendering::RenderObject::get_renderer()
{
	return _renderer;
}

RenderEngine const* minty::rendering::RenderObject::get_renderer() const
{
	return _renderer;
}

String minty::rendering::to_string(RenderObject const& value)
{
	return std::format("RenderObject()");
}
