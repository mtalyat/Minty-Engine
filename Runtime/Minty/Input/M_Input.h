#pragma once

#include "Minty/Types/M_Vector.h"
#include "Minty/Types/M_RectF.h"

#include "Minty/Input/M_Key.h"
#include "Minty/Input/M_KeyAction.h"
#include "Minty/Input/M_KeyModifiers.h"
#include "Minty/Input/M_MouseButton.h"
#include "Minty/Input/M_Gamepad.h"
#include <unordered_map>

namespace Minty
{
	class ScriptClass;

	class Input
	{
	private:
		static ScriptClass const* _script;

		static Vector2 _mousePosition;
		static std::unordered_map<MouseButton, KeyAction> _mouseButtons;

	public:
		static Vector2 get_mouse_position() { return _mousePosition; }

		static Vector2 get_mouse_position(RectF const bounds);

		static KeyAction get_mouse_button(MouseButton const button);

		static void set_input_script(ScriptClass const* const inputScript) { _script = inputScript; }

		static void trigger_key(Key key, KeyAction action, KeyModifiers mods);

		static void trigger_mouse_button(MouseButton button, KeyAction action, KeyModifiers mods);

		static void trigger_mouse_move(float x, float y);

		static void trigger_mouse_scroll(float dx, float dy);

		static void trigger_gamepad_connect(int controller);

		static void trigger_gamepad_disconnect(int controller);

		static void trigger_gamepad_button(int controller, GamepadButton button, KeyAction action);

		static void trigger_gamepad_axis(int controller, GamepadAxis axis, float value);
	};
}