#include "pch.h"
#include "M_Window.h"
#include <exception>
#include <stdexcept>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using namespace minty;

Window::Window(std::string const& title, int const width, int const height)
	: _window(nullptr)
	, _resized(true) // start as "resized" so render engine regenerates data on start
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

	// set a callback for when the window is resized
	glfwSetFramebufferSizeCallback(_window, resizeCallback);
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

void minty::Window::getFramebufferSize(int* const width, int* const height) const
{
	glfwGetFramebufferSize(_window, width, height);
}

GLFWwindow* minty::Window::getRaw() const
{
	return _window;
}

void Window::resizeCallback(GLFWwindow* const window, int const width, int const height)
{
	// check if resized, and set value if it has been resized
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	if (app)
	{
		app->_resized = true;
	}
}
