#include "pch.h"
#include "Minty/UI/M_UISystem.h"

#include "Minty/Entities/M_EntityRegistry.h"
#include "Minty/Input/M_Input.h"
#include "Minty/UI/M_UITransformComponent.h"
#include "Minty/UI/M_CanvasComponent.h"
#include "Minty/Scripting/M_ScriptComponent.h"
#include "Minty/Scenes/M_Scene.h"

using namespace Minty;

void Minty::UISystem::update(Time const time)
{
	bool mouseDown = Input::get_mouse_button(MouseButton::Left) != KeyAction::Up;
	Vector2 mousePosition = Input::get_mouse_position();

	EntityRegistry& registry = get_entity_registry();

	Entity hovering = NULL_ENTITY;

	for (auto [entity, transform] : registry.view<UITransformComponent>().each())
	{
		// get the canvas
		if (CanvasComponent* canvas = registry.try_get<CanvasComponent>(transform.canvas))
		{
			// get the mouse position in the canvas
			Vector2 mousePos = Input::get_mouse_position(canvas->toRect());

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
