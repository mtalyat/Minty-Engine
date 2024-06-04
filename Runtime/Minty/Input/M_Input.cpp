#include "pch.h"
#include "M_Input.h"

#include "Minty/Core/M_Window.h"
#include "Minty/Scripting/M_ScriptClass.h"
#include "Minty/Scripting/M_ScriptArguments.h"

using namespace Minty;

ScriptClass const* Input::_script = nullptr;

Vector2 Input::_mousePosition = Vector2();
std::unordered_map<MouseButton, KeyAction> Input::_mouseButtons = std::unordered_map<MouseButton, KeyAction>();

KeyAction Minty::Input::get_mouse_button(MouseButton const button)
{
	auto found = _mouseButtons.find(button);
	if (found != _mouseButtons.end())
	{
		return found->second;
	}

	// assume up if no state
	return KeyAction::Up;
}

void Minty::Input::trigger_key(Key key, KeyAction action, KeyModifiers mods)
{
	MINTY_ASSERT(_script);

	if (action <= KeyAction::Down)
	{
		ScriptArguments arguments({ &key, &action, &mods });
		_script->invoke(SCRIPT_INPUT_TRIGGER_KEY, arguments);
	}
}

Vector2 Minty::Input::get_mouse_position(RectF const bounds)
{
	Window& window = Window::main();
	return _mousePosition / Vector2(window.get_width(), window.get_height()) * bounds.size() - bounds.position();
}

void Minty::Input::trigger_mouse_button(MouseButton button, KeyAction action, KeyModifiers mods)
{
	MINTY_ASSERT(_script);

	if (action <= KeyAction::Down)
	{
		ScriptArguments arguments({ &button, &action, &mods });
		_script->invoke(SCRIPT_INPUT_TRIGGER_MOUSE_CLICK, arguments);
	}

	_mouseButtons[button] = action;
}

void Minty::Input::trigger_mouse_move(float x, float y)
{
	MINTY_ASSERT(_script);

	ScriptArguments arguments({ &x, &y });
	_script->invoke(SCRIPT_INPUT_TRIGGER_MOUSE_MOVE, arguments);
}

void Minty::Input::trigger_mouse_scroll(float dx, float dy)
{
	MINTY_ASSERT(_script);

	ScriptArguments arguments({ &dx, &dy });
	_script->invoke(SCRIPT_INPUT_TRIGGER_MOUSE_SCROLL, arguments);
}

void Minty::Input::trigger_gamepad_connect(int controller)
{
	MINTY_ASSERT(_script);

	ScriptArguments arguments({ &controller });
	_script->invoke(SCRIPT_INPUT_TRIGGER_GAMEPAD_CONNECT, arguments);
}

void Minty::Input::trigger_gamepad_disconnect(int controller)
{
	MINTY_ASSERT(_script);

	ScriptArguments arguments({ &controller });
	_script->invoke(SCRIPT_INPUT_TRIGGER_GAMEPAD_DISCONNECT, arguments);
}

void Minty::Input::trigger_gamepad_button(int controller, GamepadButton button, KeyAction action)
{
	MINTY_ASSERT(_script);

	ScriptArguments arguments({ &controller, &button, &action });
	_script->invoke(SCRIPT_INPUT_TRIGGER_GAMEPAD_BUTTON, arguments);
}

void Minty::Input::trigger_gamepad_axis(int controller, GamepadAxis axis, float value)
{
	MINTY_ASSERT(_script);

	ScriptArguments arguments({ &controller });
	_script->invoke(SCRIPT_INPUT_TRIGGER_GAMEPAD_AXIS, arguments);
}
