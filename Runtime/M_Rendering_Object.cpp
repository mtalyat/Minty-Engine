#include "pch.h"
#include "M_Rendering_Object.h"

#include "M_RenderEngine.h"
#include "M_Scene.h"
#include "M_RenderSystem.h"

using namespace minty;
using namespace minty::rendering;

minty::rendering::RenderObject::RenderObject()
	: _renderer()
{}

RenderObject::RenderObject(RenderEngine& renderer)
	: _renderer(&renderer)
{}

RenderObject::~RenderObject()
{}

void minty::rendering::RenderObject::set_render_engine(RenderEngine& renderer)
{
	_renderer = &renderer;
}

RenderEngine& minty::rendering::RenderObject::get_render_engine() const
{
	return *_renderer;
}

RenderSystem* minty::rendering::RenderObject::get_render_system() const
{
	return _renderer->get_scene()->get_system_registry().find<RenderSystem>();
}

String minty::rendering::to_string(RenderObject const& value)
{
	return std::format("RenderObject()");
}
