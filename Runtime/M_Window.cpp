#include "pch.h"
#include "M_Window.h"
#include "M_Console.h"
#include "M_ScriptClass.h"
#include "M_ScriptEngine.h"
#include "M_ScriptArguments.h"
#include "M_GLFW.h"
#include "stb_image.h"
#include <format>
#include <string>

using namespace minty;

int Window::_windowCount = 0;

Window::Window(String const& title, int const width, int const height, Path const& iconPath)
	: Window(title, -1, -1, width, height, iconPath)
{}

minty::Window::Window(String const& title, int const x, int const y, int const width, int const height, Path const& iconPath)
	: _title(title)
	, _window()
	, _width(width)
	, _height(height)
	, _restoreX(x)
	, _restoreY(y)
	, _resized(true) // start as "resized" so render engine regenerates data on start
	, _windowScript()
	, _inputScript()
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
		MINTY_ABORT("Failed to create GLFW window \"" + title + "\".");
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

	set_icon(iconPath);
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

void minty::Window::on_link(ScriptEngine& engine)
{
	_windowScript = engine.get_assembly(ASSEMBLY_ENGINE_NAME)->get_class("MintyEngine", "Window");
	_inputScript = engine.get_assembly(ASSEMBLY_ENGINE_NAME)->get_class("MintyEngine", "Input");
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

void minty::Window::set_icon(Path const& path)
{
	if (path.empty()) return;

	// https://stackoverflow.com/questions/44321902/load-icon-function-for-glfwsetwindowicon

	GLFWimage image;
	image.pixels = stbi_load(path.string().c_str(), &image.width, &image.height, 0, 4);
	glfwSetWindowIcon(_window, 1, &image);
	stbi_image_free(image.pixels);
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

CursorMode minty::Window::get_cursor_mode() const
{
	switch (glfwGetInputMode(_window, GLFW_CURSOR))
	{
	case GLFW_CURSOR_NORMAL:
		return CursorMode::Normal;
	case GLFW_CURSOR_HIDDEN:
		return CursorMode::Hidden;
	case GLFW_CURSOR_DISABLED:
		return CursorMode::Disabled;
	default:
		return CursorMode::Normal;
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

void minty::Window::poll_events()
{
	glfwPollEvents();
}

void minty::Window::save_restore_info()
{
	glfwGetWindowPos(_window, &_restoreX, &_restoreY);
	glfwGetWindowSize(_window, &_width, &_height);
}

void minty::Window::trigger_key(Key key, KeyAction action, KeyModifiers mods)
{
	if (_inputScript && action <= KeyAction::Down)
	{
		ScriptArguments arguments({ &key, &action, &mods });
		_inputScript->invoke(SCRIPT_INPUT_TRIGGER_KEY, arguments);
	}
}

void minty::Window::trigger_mouse_click(MouseButton button, KeyAction action, KeyModifiers mods)
{
	if (_inputScript && action <= KeyAction::Down)
	{
		ScriptArguments arguments({ &button, &action, &mods });
		_inputScript->invoke(SCRIPT_INPUT_TRIGGER_MOUSE_CLICK, arguments);
	}
}

void minty::Window::trigger_mouse_scroll(float dx, float dy)
{
	if (_inputScript)
	{
		ScriptArguments arguments({ &dx, &dy });
		_inputScript->invoke(SCRIPT_INPUT_TRIGGER_MOUSE_SCROLL, arguments);
	}
}

void minty::Window::trigger_mouse_move(float x, float y)
{
	if (_inputScript)
	{
		ScriptArguments arguments({ &x, &y });
		_inputScript->invoke(SCRIPT_INPUT_TRIGGER_MOUSE_MOVE, arguments);
	}
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
		w->trigger_mouse_click(static_cast<MouseButton>(button), static_cast<KeyAction>(action), static_cast<KeyModifiers>(mods));
	}
}

void minty::Window::cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	Console::ass(w != nullptr, "Window is null on cursor callback.");
	if (w)
	{
		w->trigger_mouse_move(static_cast<float>(xpos), static_cast<float>(ypos));
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
		w->trigger_mouse_scroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
	}
}

String minty::to_string(Window const& value)
{
	return std::format("Window(title = {})", value._title);
}
