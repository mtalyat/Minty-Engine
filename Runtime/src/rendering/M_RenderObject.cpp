#include "pch.h"
#include "types/M_Object.h"

#include "runtime/M_Runtime.h"
#include "rendering/M_RenderEngine.h"
#include "rendering/M_RenderSystem.h"
#include "scenes/M_Scene.h"
#include "scenes/M_SceneManager.h"
#include "systems/M_SystemRegistry.h"

using namespace minty;
using namespace minty;

minty::RenderObject::RenderObject()
	: RuntimeObject()
{}

RenderObject::RenderObject(Runtime& engine)
	: RuntimeObject(engine)
{}

RenderObject::~RenderObject()
{}

AssetEngine& minty::RenderObject::get_asset_engine() const
{
	return get_runtime().get_asset_engine();
}

RenderEngine& minty::RenderObject::get_render_engine() const
{
	return get_runtime().get_render_engine();
}

RenderSystem* minty::RenderObject::get_render_system() const
{
	Scene* scene = get_runtime().get_scene_manager().get_loaded_scene();

	if (scene)
	{
		return scene->get_system_registry().find<RenderSystem>();
	}
	else
	{
		return nullptr;
	}
}

String minty::to_string(RenderObject const& value)
{
	return std::format("RenderObject()");
}
