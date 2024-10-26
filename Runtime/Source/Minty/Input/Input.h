#pragma once

#include "Minty/Core/Math.h"
#include "Minty/Core/Rect.h"
#include "Minty/Core/Type.h"
#include "Minty/Input/Key.h"
#include "Minty/Input/Mouse.h"
#include "Minty/Input/Gamepad.h"
#include "Minty/Script/ScriptClass.h"
#include <unordered_map>

namespace Minty
{
	class ScriptClass;

	class Input
	{
	private:
		static Ref<ScriptClass> s_script;

		static Float2 s_mousePosition;
		static std::unordered_map<MouseButton, KeyAction> s_mouseButtons;

	public:
		static Float2 get_mouse_position() { return s_mousePosition; }

		static Float2 get_mouse_position(Rect const bounds);

		static KeyAction get_mouse_button(MouseButton const button);

		static void set_input_script(Ref<ScriptClass> const inputScript) { s_script = inputScript; }

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