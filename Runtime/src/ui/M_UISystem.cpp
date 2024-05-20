#include "pch.h"
#include "ui/M_UISystem.h"

#include "runtime/M_Runtime.h"
#include "ui/M_UITransformComponent.h"
#include "ui/M_CanvasComponent.h"
#include "scripting/M_ScriptComponent.h"
#include "scenes/M_Scene.h"

using namespace minty;

minty::UISystem::UISystem(Runtime& engine, Scene& scene)
	: System("UI", engine, scene)
	, _mousePosition()
	, _mouseDown()
	, _clicking()
	, _family()
{ }

void minty::UISystem::update()
{
	Window& window = get_runtime().get_window();
	bool mouseDown = window.get_mouse_down();
	Vector2 mousePosition = window.get_mouse_position();

	EntityRegistry& registry = get_entity_registry();

	Entity hovering = NULL_ENTITY;

	for (auto [entity, transform] : registry.view<UITransformComponent>().each())
	{
		// get the canvas
		if (CanvasComponent* canvas = registry.try_get<CanvasComponent>(transform.canvas))
		{
			// get the mouse position in the canvas
			Vector2 mousePos = window.get_mouse_position(canvas->toRect());

			// if that is within the UI transform, call enter events
			if (transform.globalRect.contains(mousePos))
			{
				// found the first one, all done
				hovering = entity;
				break;
			}
		}
	}

	// get the family line being hovered over
	std::vector<Entity> familyLine = registry.get_family_line(hovering);
	std::unordered_set<Entity> newFamily(familyLine.begin(), familyLine.end());

	// ON ENTER
	for (Entity entity : familyLine)
	{
		if (_family.contains(entity)) continue;

		registry.trigger_event<ScriptOnPointerEnterComponent>(entity);
	}

	// ON HOVER
	for (Entity entity : familyLine)
	{
		registry.trigger_event<ScriptOnPointerHoverComponent>(entity);
	}

	// ON EXIT
	for (Entity entity : _family)
	{
		if (newFamily.contains(entity)) continue;

		registry.trigger_event<ScriptOnPointerExitComponent>(entity);
	}

	// ON DOWN
	if (!_mouseDown && mouseDown)
	{
		for (Entity entity : familyLine)
		{
			registry.trigger_event<ScriptOnPointerDownComponent>(entity);
		}

		_clicking = true;
	}

	// ON MOVE
	if (mousePosition != _mousePosition)
	{
		for (Entity entity : familyLine)
		{
			registry.trigger_event<ScriptOnPointerMoveComponent>(entity);
		}

		// if moved, no longer clicking
		_clicking = false;
	}

	// ON UP
	if (_mouseDown && !mouseDown)
	{
		for (Entity entity : familyLine)
		{
			registry.trigger_event<ScriptOnPointerUpComponent>(entity);
		}

		if (_clicking)
		{
			for (Entity entity : familyLine)
			{
				registry.trigger_event<ScriptOnPointerClickComponent>(entity);
			}
		}
	}

	// update family
	_family.clear();
	_family.reserve(familyLine.size());
	_family.insert(familyLine.begin(), familyLine.end());

	// update mouse states
	_mouseDown = mouseDown;
	_mousePosition = mousePosition;
}
