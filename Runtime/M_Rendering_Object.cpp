#include "pch.h"
#include "M_Rendering_Object.h"

#include "M_RenderEngine.h"

using namespace minty;
using namespace minty::rendering;

RenderObject::RenderObject(RenderEngine& renderer)
	: _renderer(renderer)
{}

RenderObject::~RenderObject()
{}

String minty::rendering::to_string(RenderObject const& value)
{
	return std::format("RenderObject()");
}
