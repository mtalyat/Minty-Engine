#include "pch.h"
#include "Minty/Types/M_Object.h"

#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Rendering/M_RenderSystem.h"
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Scenes/M_SceneManager.h"
#include "Minty/Systems/M_SystemRegistry.h"
#include "Minty/Core/M_Application.h"
#include "Minty/Layers/M_DefaultLayer.h"

using namespace Minty;
using namespace Minty;

RenderSystem* Minty::RenderObject::get_render_system() const
{
	Ref<Scene> scene = Application::instance().get_default_layer().get_scene_manager().get_loaded_scene();

	if (scene)
	{
		return scene->get_system_registry().find<RenderSystem>();
	}

	return nullptr;
}
