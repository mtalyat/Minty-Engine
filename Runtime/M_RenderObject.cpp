#include "pch.h"
#include "M_Object.h"

#include "M_Runtime.h"
#include "M_RenderEngine.h"
#include "M_RenderSystem.h"
#include "M_Scene.h"
#include "M_SystemRegistry.h"

using namespace minty;
using namespace minty;

minty::RenderObject::RenderObject()
	: SceneObject()
{}

RenderObject::RenderObject(Runtime& engine, ID const sceneId)
	: SceneObject(engine, sceneId)
{}

RenderObject::~RenderObject()
{}

RenderEngine& minty::RenderObject::get_render_engine() const
{
	return get_runtime().get_render_engine();
}

RenderSystem* minty::RenderObject::get_render_system() const
{
	return get_scene().get_system_registry().find<RenderSystem>();
}

String minty::to_string(RenderObject const& value)
{
	return std::format("RenderObject()");
}