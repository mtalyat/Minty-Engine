#include "pch.h"
#include "M_Window.h"
#include "M_Console.h"
#include <exception>
#include <stdexcept>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <format>
#include <string>

using namespace minty;

Window::Window(std::string const& title, int const width, int const height)
	: _window()
	, _resized(true) // start as "resized" so render engine regenerates data on start
	, _activeInputMap()
	, _lastMouseX()
	, _lastMouseY()
	, _mouseOutOfBounds(true) // start as "out of bounds"
{
	// do not use OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// resizability
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	// create window
	_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (!_window)
	{
		throw std::runtime_error("Failed to create GLFW window \"" + title + "\".");
	}

	// set user pointer, so this class can be referenced for callbacks
	glfwSetWindowUserPointer(_window, reinterpret_cast<void*>(this));

	// set a callback for when the window is resized
	glfwSetFramebufferSizeCallback(_window, resizeCallback);

	// set input callbacks
	glfwSetKeyCallback(_window, keyCallback);
	glfwSetMouseButtonCallback(_window, buttonCallback);
	glfwSetCursorPosCallback(_window, cursorCallback);
	glfwSetScrollCallback(_window, scrollCallback);

	// might want this for engine:
	// glfwSetDropCallback
}

Window::~Window()
{
	// destroy window
	glfwDestroyWindow(_window);
}

bool minty::Window::isResized()
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

bool minty::Window::isOpen() const
{
	return !glfwWindowShouldClose(_window);
}

void minty::Window::close()
{
	glfwSetWindowShouldClose(_window, GLFW_TRUE);
}

void minty::Window::getFramebufferSize(int* const width, int* const height) const
{
	glfwGetFramebufferSize(_window, width, height);
}

GLFWwindow* minty::Window::getRaw() const
{
	return _window;
}

void minty::Window::setInput(InputMap const* const inputMap)
{
	_activeInputMap = inputMap;
}

void minty::Window::triggerKey(Key const key, KeyAction const action, KeyModifiers const mods)
{
	if (_activeInputMap)
	{
		KeyPressEventArgs args{
			.key = key,
			.action = action,
			.mods = mods
		};

		_activeInputMap->invokeKey(args);
	}
}

void minty::Window::triggerButton(MouseButton const button, KeyAction const action, KeyModifiers const mods)
{
	if (_activeInputMap)
	{
		MouseClickEventArgs args{
			.button = button,
			.action = action,
			.mods = mods,
			.x = _lastMouseX,
			.y = _lastMouseY
		};

		_activeInputMap->invokeMouseClick(args);
	}
}

void minty::Window::triggerScroll(float dx, float dy)
{
	if (_activeInputMap)
	{
		MouseScrollEventArgs args{
			.dx = dx,
			.dy = dy
		};

		_activeInputMap->invokeMouseScroll(args);
	}
}

void minty::Window::triggerCursor(float x, float y)
{
	if (_activeInputMap)
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

		_activeInputMap->invokeMouseMove(args);
	}

	// update mouse positions
	_lastMouseX = x;
	_lastMouseY = y;
	_mouseOutOfBounds = false;
}

void Window::resizeCallback(GLFWwindow* const window, int const width, int const height)
{
	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	console::ass(w != nullptr, "Window is null on resize callback.");
	if (w)
	{
		w->_resized = true;
	}
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	console::ass(w != nullptr, "Window is null on key callback.");
	if (w)
	{
		w->triggerKey(static_cast<Key>(key), static_cast<KeyAction>(action), static_cast<KeyModifiers>(mods));
	}
}

void minty::Window::buttonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	console::ass(w != nullptr, "Window is null on button callback.");
	if (w)
	{
		w->triggerButton(static_cast<MouseButton>(button), static_cast<KeyAction>(action), static_cast<KeyModifiers>(mods));
	}
}

void minty::Window::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	console::ass(w != nullptr, "Window is null on cursor callback.");
	if (w)
	{
		w->triggerCursor(static_cast<float>(xpos), static_cast<float>(ypos));
	}
}

void minty::Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	console::ass(w != nullptr, "Window is null on scroll callback.");
	if (w)
	{
		w->triggerScroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
	}
}
