#include "pch.h"
#include "Window.h"

//#include "Minty/Scripting/M_ScriptEngine.h"
//#include "Minty/Scripting/M_ScriptClass.h"
//#include "Minty/Scripting/M_ScriptObject.h"
//#include "Minty/Scripting/M_ScriptArguments.h"

#ifdef MINTY_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

using namespace Minty;

std::unordered_map<UUID, Window*> Minty::Window::_windows = std::unordered_map<UUID, Window*>();
Window* Minty::Window::_main = nullptr;

Owner<Window> Minty::Window::create(WindowBuilder const& builder)
{
#ifdef MINTY_WINDOWS
	return Owner<WindowsWindow>(builder);
#else
	MINTY_ERROR("Unknown platform: cannot create Window.");
	return nullptr;
#endif
}

void Minty::Window::register_window(Window* window)
{
	// add to window list
	_windows.emplace(window->id(), window);

	//// create script object
	//ScriptEngine& scripts = ScriptEngine::instance();
	//window->_scriptClass = scripts.find_class(MINTY_NAME_SCRIPT_NAMESPACE, "Window");
	//MINTY_ASSERT(window->_scriptClass);

	//UUID windowId = window->id();
	//ScriptArguments args({ &windowId });
	//scripts.create_object(*window->_scriptClass, window->id(), args);
}

void Minty::Window::unregister_window(Window* window)
{
	//// destroy script object
	//ScriptEngine& scripts = ScriptEngine::instance();
	//scripts.destroy_object(window->id());

	// remove from list
	auto found = _windows.find(window->id());
	if (found != _windows.end())
	{
		_windows.erase(found);
	}
}

//
//Vector2 Minty::Window::get_mouse_position() const
//{
//	return _mousePosition;
//}
//
//Vector2 Minty::Window::get_mouse_position(RectF const bounds) const
//{
//	// normalize mouse position, then scale to bound size, then offset using bound position
//	return _mousePosition / Vector2(_width, _height) * bounds.size() - bounds.position();
//}
//
//Bool Minty::Window::get_mouse_down() const
//{
//	return _mouseDown;
//}
//
//void Minty::Window::trigger_key(Key key, KeyAction action, KeyModifiers mods)
//{
//	if (_inputScript && action <= KeyAction::Down)
//	{
//		ScriptArguments arguments({ &key, &action, &mods });
//		_inputScript->invoke(SCRIPT_INPUT_TRIGGER_KEY, arguments);
//	}
//}
//
//void Minty::Window::trigger_mouse_button(MouseButton button, KeyAction action, KeyModifiers mods)
//{
//	if (_inputScript && action <= KeyAction::Down)
//	{
//		ScriptArguments arguments({ &button, &action, &mods });
//		_inputScript->invoke(SCRIPT_INPUT_TRIGGER_MOUSE_CLICK, arguments);
//	}
//
//	_mouseDown = action != KeyAction::Up;
//}
//
//void Minty::Window::trigger_mouse_scroll(float dx, float dy)
//{
//	if (_inputScript)
//	{
//		ScriptArguments arguments({ &dx, &dy });
//		_inputScript->invoke(SCRIPT_INPUT_TRIGGER_MOUSE_SCROLL, arguments);
//	}
//}
//
//void Minty::Window::trigger_mouse_move(float x, float y)
//{
//	_mousePosition.x = x;
//	_mousePosition.y = y;
//	if (_inputScript)
//	{
//		ScriptArguments arguments({ &x, &y });
//		_inputScript->invoke(SCRIPT_INPUT_TRIGGER_MOUSE_MOVE, arguments);
//	}
//}
//
//void Minty::Window::trigger_gamepad_connect(int controller)
//{
//	if (_inputScript)
//	{
//		ScriptArguments arguments({ &controller });
//		_inputScript->invoke(SCRIPT_INPUT_TRIGGER_GAMEPAD_CONNECT, arguments);
//	}
//}
//
//void Minty::Window::trigger_gamepad_disconnect(int controller)
//{
//	if (_inputScript)
//	{
//		ScriptArguments arguments({ &controller });
//		_inputScript->invoke(SCRIPT_INPUT_TRIGGER_GAMEPAD_DISCONNECT, arguments);
//	}
//}
//
//void Minty::Window::trigger_gamepad_button(int controller, GamepadButton button, KeyAction action)
//{
//	if (_inputScript)
//	{
//		ScriptArguments arguments({ &controller, &button, &action });
//		_inputScript->invoke(SCRIPT_INPUT_TRIGGER_GAMEPAD_BUTTON, arguments);
//	}
//}
//
//void Minty::Window::trigger_gamepad_axis(int controller, GamepadAxis axis, float value)
//{
//	if (_inputScript)
//	{
//		ScriptArguments arguments({ &controller, &axis, &value });
//		_inputScript->invoke(SCRIPT_INPUT_TRIGGER_GAMEPAD_AXIS, arguments);
//	}
//}
//
//void Window::resize_callback(GLFWwindow* const window, int const width, int const height)
//{
//	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
//	MINTY_ASSERT_MESSAGE(w != nullptr, "Window is null on resize callback.");
//	if (w)
//	{
//		w->refresh();
//		w->_resized = true;
//	}
//}
//
//void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
//	MINTY_ASSERT_MESSAGE(w != nullptr, "Window is null on key callback.");
//	if (w)
//	{
//		w->trigger_key(static_cast<Key>(key), static_cast<KeyAction>(action), static_cast<KeyModifiers>(mods));
//	}
//}
//
//void Minty::Window::button_ballback(GLFWwindow* window, int button, int action, int mods)
//{
//	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
//	MINTY_ASSERT_MESSAGE(w != nullptr, "Window is null on button callback.");
//	if (w)
//	{
//		w->trigger_mouse_button(static_cast<MouseButton>(button), static_cast<KeyAction>(action), static_cast<KeyModifiers>(mods));
//	}
//}
//
//void Minty::Window::cursor_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
//	MINTY_ASSERT_MESSAGE(w != nullptr, "Window is null on cursor callback.");
//	if (w)
//	{
//		w->trigger_mouse_move(static_cast<float>(xpos), static_cast<float>(ypos));
//	}
//}
//
////void Minty::Window::gamepad_callback(int id, int event)
////{
////	if (event == GLFW_CONNECTED)
////	{
////		MINTY_LOG_FORMAT("Controller {} connected.", id);
////	}
////	else if (event == GLFW_DISCONNECTED)
////	{
////		MINTY_LOG_FORMAT("Controller {} disconnected.", id);
////	}
////
////	void* data = glfwGetJoystickUserPointer(id);
////
////	MINTY_LOG_FORMAT("User Pointer is {}null.", data == nullptr ? "" : "not ");
////}
//
//void Minty::Window::error_callback(int const error, char const* description)
//{
//	MINTY_ERROR_FORMAT("Window GLFW Error: code = {}, message = \"{}\"", error, description);
//}
//
//void Minty::Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
//	MINTY_ASSERT_MESSAGE(w != nullptr, "Window is null on scroll callback.");
//	if (w)
//	{
//		w->trigger_mouse_scroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
//	}
//}

Minty::Window::Window(WindowBuilder const& builder)
	: _id(UUID::create())
	, _data(WindowData{
.title = builder.title,
.width = builder.width,
.height = builder.height
		})
	, _scriptClass()
{}
