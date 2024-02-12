#include "pch.h"
#include "M_Window.h"
#include "M_Console.h"
#include "M_Error.h"
#include "M_InputMap.h"
#include "M_GLFW.h"
#include <format>
#include <string>

using namespace minty;

int Window::_windowCount = 0;

Window::Window(String const& title, int const width, int const height, InputMap const* const globalInputMap)
	: Window(title, -1, -1, width, height, globalInputMap)
{}

minty::Window::Window(String const& title, int const x, int const y, int const width, int const height, InputMap const* const globalInputMap)
	: _title(title)
	, _window()
	, _width(width)
	, _height(height)
	, _restoreX(x)
	, _restoreY(y)
	, _resized(true) // start as "resized" so render engine regenerates data on start
	, _activeInputMap()
	, _globalInputMap(globalInputMap)
	, _lastMouseX()
	, _lastMouseY()
	, _mouseOutOfBounds(true) // start as "out of bounds"
{
	// if no windows have been made yet, init glfw
	if (_windowCount == 0)
	{
		glfwInit();
		glfwSetErrorCallback(error_callback);
	}
	_windowCount++;

	// do not use OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// resizability
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	// create window
	_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (!_window)
	{
		Error::abort("Failed to create GLFW window \"" + title + "\".");
	}

	if (x >= 0 && y >= 0)
	{
		// if x and y given, set there
		restore();
	}
	else
	{
		// set x and y
		save_restore_info();
	}

	// set user pointer, so this class can be referenced for callbacks
	glfwSetWindowUserPointer(_window, reinterpret_cast<void*>(this));

	// set a callback for when the window is resized
	glfwSetFramebufferSizeCallback(_window, resize_callback);

	// set input callbacks
	glfwSetKeyCallback(_window, key_callback);
	glfwSetMouseButtonCallback(_window, button_ballback);
	glfwSetCursorPosCallback(_window, cursor_callback);
	glfwSetScrollCallback(_window, scroll_callback);

	// might want this for engine:
	// glfwSetDropCallback
}

Window::~Window()
{
	// destroy window
	glfwDestroyWindow(_window);

	// if no windows left, destroy glfw
	_windowCount--;
	if (_windowCount == 0)
	{
		glfwTerminate();
	}
}

void minty::Window::set_title(String const& title)
{
	_title = title;
	glfwSetWindowTitle(_window, title.c_str());
}

String const& minty::Window::get_title() const
{
	return _title;
}

void minty::Window::set_cursor_mode(CursorMode const mode)
{
	switch (mode)
	{
	case CursorMode::Normal:
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case CursorMode::Hidden:
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		break;
	case CursorMode::Disabled:
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	}
}

void minty::Window::maximize()
{
	save_restore_info();
	glfwMaximizeWindow(_window);
}

void minty::Window::minimize()
{
	save_restore_info();
	glfwIconifyWindow(_window);
}

void minty::Window::restore()
{
	glfwSetWindowPos(_window, _restoreX, _restoreY);
	glfwSetWindowSize(_window, _width, _height);
}

bool minty::Window::is_resized()
{
	if (_resized)
	{
		// window has resized, so set to false and return true
		_resized = false;
		return true;
	}

	// no change, return false
	return false;
}

bool minty::Window::is_open() const
{
	return !glfwWindowShouldClose(_window);
}

void minty::Window::close()
{
	glfwSetWindowShouldClose(_window, GLFW_TRUE);
}

void minty::Window::refresh()
{
	glfwGetFramebufferSize(_window, &_frameWidth, &_frameHeight);
	glfwGetWindowSize(_window, &_width, &_height);
}

int minty::Window::get_frame_width() const
{
	return _frameWidth;
}

int minty::Window::get_frame_height() const
{
	return _frameHeight;
}

int minty::Window::get_width() const
{
	return _width;
}

int minty::Window::get_height() const
{
	return _height;
}

GLFWwindow* minty::Window::get_raw() const
{
	return _window;
}

void minty::Window::set_input(InputMap const* const inputMap)
{
	_activeInputMap = inputMap;
}

InputMap const* minty::Window::get_input() const
{
	return _activeInputMap;
}

void minty::Window::poll_events()
{
	glfwPollEvents();
}

void minty::Window::save_restore_info()
{
	glfwGetWindowPos(_window, &_restoreX, &_restoreY);
	glfwGetWindowSize(_window, &_width, &_height);
}

void minty::Window::trigger_key(Key const key, KeyAction const action, KeyModifiers const mods)
{
	KeyPressEventArgs args{
	.key = key,
	.action = action,
	.mods = mods
	};

	if (_globalInputMap)
	{
		_globalInputMap->invoke_key(args);
	}

	if (_activeInputMap)
	{
		_activeInputMap->invoke_key(args);
	}
}

void minty::Window::trigger_button(MouseButton const button, KeyAction const action, KeyModifiers const mods)
{
	MouseClickEventArgs args{
	.button = button,
	.action = action,
	.mods = mods,
	.x = _lastMouseX,
	.y = _lastMouseY
	};

	if (_globalInputMap)
	{
		_globalInputMap->invoke_mouse_click(args);
	}

	if (_activeInputMap)
	{
		_activeInputMap->invoke_mouse_click(args);
	}
}

void minty::Window::trigger_scroll(float dx, float dy)
{
	MouseScrollEventArgs args{
	.dx = dx,
	.dy = dy
	};

	if (_globalInputMap)
	{
		_globalInputMap->invoke_mouse_scroll(args);
	}

	if (_activeInputMap)
	{
		_activeInputMap->invoke_mouse_scroll(args);
	}
}

void minty::Window::trigger_cursor(float x, float y)
{
	// find movement from last time the mouse moved

	float dx, dy;
	if (_mouseOutOfBounds)
	{
		dx = 0.0f;
		dy = 0.0f;
	}
	else
	{
		dx = x - _lastMouseX;
		dy = y - _lastMouseY;
	}

	MouseMoveEventArgs args{
		.x = x,
		.y = y,
		.dx = dx,
		.dy = dy
	};

	if (_globalInputMap)
	{
		_globalInputMap->invoke_mouse_move(args);
	}

	if (_activeInputMap)
	{
		_activeInputMap->invoke_mouse_move(args);
	}

	// update mouse positions
	_lastMouseX = x;
	_lastMouseY = y;
	_mouseOutOfBounds = false;
}

void Window::resize_callback(GLFWwindow* const window, int const width, int const height)
{
	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	Console::ass(w != nullptr, "Window is null on resize callback.");
	if (w)
	{
		w->_resized = true;
	}
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	Console::ass(w != nullptr, "Window is null on key callback.");
	if (w)
	{
		w->trigger_key(static_cast<Key>(key), static_cast<KeyAction>(action), static_cast<KeyModifiers>(mods));
	}
}

void minty::Window::button_ballback(GLFWwindow* window, int button, int action, int mods)
{
	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	Console::ass(w != nullptr, "Window is null on button callback.");
	if (w)
	{
		w->trigger_button(static_cast<MouseButton>(button), static_cast<KeyAction>(action), static_cast<KeyModifiers>(mods));
	}
}

void minty::Window::cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	Console::ass(w != nullptr, "Window is null on cursor callback.");
	if (w)
	{
		w->trigger_cursor(static_cast<float>(xpos), static_cast<float>(ypos));
	}
}

void minty::Window::error_callback(int const error, char const* description)
{
	Console::error(std::format("Window GLFW Error: code = {}, message = \"{}\"", error, description));
}

void minty::Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	Console::ass(w != nullptr, "Window is null on scroll callback.");
	if (w)
	{
		w->trigger_scroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
	}
}

String minty::to_string(Window const& value)
{
	return std::format("Window(title = {})", value._title);
}
