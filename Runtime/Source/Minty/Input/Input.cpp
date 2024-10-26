#include "pch.h"
#include "Input.h"

#include "Minty/Core/Constants.h"
#include "Minty/Window/WindowManager.h"
#include "Minty/Script/ScriptArguments.h"

using namespace Minty;

Ref<ScriptClass> Minty::Input::s_script = nullptr;
Float2 Minty::Input::s_mousePosition = {};
std::unordered_map<MouseButton, KeyAction> Minty::Input::s_mouseButtons = {};

KeyAction Minty::Input::get_mouse_button(MouseButton const button)
{
	auto found = s_mouseButtons.find(button);
	if (found != s_mouseButtons.end())
	{
		return found->second;
	}

	// assume up if no state
	return KeyAction::Up;
}

void Minty::Input::trigger_key(Key key, KeyAction action, KeyModifiers mods)
{
	MINTY_ASSERT(s_script);

	if (action <= KeyAction::Down)
	{
		void* argv[] = { &key, &action, &mods };
		Size argc = 3;
		s_script->invoke(SCRIPT_INPUT_TRIGGER_KEY, argv, argc);
	}
}

Float2 Minty::Input::get_mouse_position(Rect const bounds)
{
	Ref<Window> window = WindowManager::get_main();
	return s_mousePosition / Float2(window->get_width(), window->get_height()) * bounds.size - bounds.position;
}

void Minty::Input::trigger_mouse_button(MouseButton button, KeyAction action, KeyModifiers mods)
{
	MINTY_ASSERT(s_script);

	if (action <= KeyAction::Down)
	{
		void* argv[] = { &button, &action, &mods };
		Size argc = 3;
		s_script->invoke(SCRIPT_INPUT_TRIGGER_MOUSE_CLICK, argv, argc);
	}

	s_mouseButtons[button] = action;
}

void Minty::Input::trigger_mouse_move(float x, float y)
{
	MINTY_ASSERT(s_script);

	void* argv[] = { &x, &y };
	Size argc = 2;
	s_script->invoke(SCRIPT_INPUT_TRIGGER_MOUSE_MOVE, argv, argc);
}

void Minty::Input::trigger_mouse_scroll(float dx, float dy)
{
	MINTY_ASSERT(s_script);

	void* argv[] = { &dx, &dy };
	Size argc = 2;
	s_script->invoke(SCRIPT_INPUT_TRIGGER_MOUSE_SCROLL, argv, argc);
}

void Minty::Input::trigger_gamepad_connect(int controller)
{
	MINTY_ASSERT(s_script);

	void* argv[] = { &controller };
	Size argc = 1;
	s_script->invoke(SCRIPT_INPUT_TRIGGER_GAMEPAD_CONNECT, argv, argc);
}

void Minty::Input::trigger_gamepad_disconnect(int controller)
{
	MINTY_ASSERT(s_script);
	
	void* argv[] = { &controller };
	Size argc = 1;
	s_script->invoke(SCRIPT_INPUT_TRIGGER_GAMEPAD_DISCONNECT, argv, argc);
}

void Minty::Input::trigger_gamepad_button(int controller, GamepadButton button, KeyAction action)
{
	MINTY_ASSERT(s_script);

	void* argv[] = { &controller, &button, &action };
	Size argc = 3;
	s_script->invoke(SCRIPT_INPUT_TRIGGER_GAMEPAD_BUTTON, argv, argc);
}

void Minty::Input::trigger_gamepad_axis(int controller, GamepadAxis axis, float value)
{
	MINTY_ASSERT(s_script);

	void* argv[] = { &controller, &axis, &value };
	Size argc = 3;
	s_script->invoke(SCRIPT_INPUT_TRIGGER_GAMEPAD_AXIS, argv, argc);
}
