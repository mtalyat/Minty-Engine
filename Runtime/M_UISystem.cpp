#include "pch.h"
#include "M_UISystem.h"

#include "M_Runtime.h"
#include "M_UITransformComponent.h"
#include "M_CanvasComponent.h"
#include "M_ScriptComponent.h"
#include "M_Scene.h"

using namespace minty;

minty::UISystem::UISystem(Runtime& engine, Scene& scene)
	: System("UI", engine, scene)
{
	//_onMouseMove = [this](MouseMoveEventArgs const& args)
	//	{
	//		
	//	};

	//_onMouseClick = [this](MouseClickEventArgs const& args)
	//	{

	//	};
}

void minty::UISystem::update()
{
	Window& window = get_runtime().get_window();

	EntityRegistry& registry = get_entity_registry();

	for (auto [entity, transform, script, onPointerEnter] : registry.view<UITransformComponent const, ScriptComponent const, ScriptOnPointerEnterComponent const>().each())
	{
		// get the canvas
		if (CanvasComponent* canvas = registry.try_get<CanvasComponent>(transform.canvas))
		{
			// get the mouse position in the canvas
			Vector2 mousePos = window.get_mouse_position(canvas->toRect());

			// TODO: ALL OTHER EVENTS: THIS IS BASICALLY A HOVER, NOT ENTER
			// if that is within the UI transform, call enter events
			if (transform.globalRect.contains(mousePos))
			{
				onPointerEnter.invoke(SCRIPT_METHOD_NAME_ONPOINTERENTER, script);
			}
		}
	}
}
