#include "pch.h"
#include "WindowsWindowManager.h"

#include "Minty/Library/GLFW.h"

using namespace Minty;

static void error_callback(Int error, Char const* description)
{
	MINTY_ERROR_FORMAT("GLFW error ({}): {}", error, description);
}

void Minty::WindowsWindowManager::initialize(WindowManagerBuilder const& builder)
{
	// open GLFW
	if (!glfwInit())
	{
		MINTY_ERROR("Failed to initialize GLFW.");
	}

	// set error handling
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void Minty::WindowsWindowManager::shutdown()
{
	// close GLFW
	glfwTerminate();
}
