#include "pch.h"
#include "M_Rendering_Object.h"

#include "M_Renderer.h"

using namespace minty;
using namespace minty::rendering;

RendererObject::RendererObject(Renderer& renderer)
	: _renderer(renderer)
{}

RendererObject::~RendererObject()
{}
