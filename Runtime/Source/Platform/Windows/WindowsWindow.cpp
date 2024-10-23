#include "pch.h"
#include "WindowsWindow.h"

#include "Minty/Core/Math.h"
#include "Minty/Input/Mouse.h"
#include "Minty/Library/Stb.h"

#include "Minty/Core/Event.h"

using namespace Minty;

static constexpr float JOYSTICK_THRESHOLD = 0.1f;

static void error_callback(int error, char const* description)
{
	MINTY_ERROR_FORMAT("GLFW error ({}): {}", error, description);
}

Minty::WindowsWindow::WindowsWindow(WindowBuilder const& builder)
	: Window(builder)
	, _window()
	, _gamepads()
	, _frameWidth(static_cast<int>(builder.width))
	, _frameHeight(static_cast<int>(builder.height))
	, _restoreX(builder.x)
	, _restoreY(builder.y)
{
	// if no windows have been made yet, init glfw
	if (_windows.empty())
	{
		if (!glfwInit())
		{
			MINTY_ERROR("Failed to initialize GLFW.");
		}
		glfwSetErrorCallback(error_callback);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	_window = glfwCreateWindow(static_cast<int>(builder.width), static_cast<int>(builder.height), builder.title.c_str(), nullptr, nullptr);

	MINTY_ASSERT_FORMAT(_window, "Failed to create GLFW window \"{}\".", get_title());

	if (_windows.empty() == 1)
	{
		_main = this;
	}

	register_window(this);

	glfwSetWindowUserPointer(_window, &_data);

	if (builder.x >= 0 && builder.y >= 0)
	{
		// if x and y given, set there
		restore();
	}
	else
	{
		// set x and y
		save_restore_info();
	}

	// set callbacks
	glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.width = width;
			data.height = height;

			WindowResizeEvent event(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
			data.callback(event);
		});

	glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent event = {};
			data.callback(event);
		});

	glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			KeyEvent event(static_cast<Key>(key), static_cast<KeyAction>(action), static_cast<KeyModifiers>(mods));
			data.callback(event);
		});

	//glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keycode){});

	glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseButtonEvent event(static_cast<MouseButton>(button), static_cast<KeyAction>(action), static_cast<KeyModifiers>(mods));
			data.callback(event);
		});

	glfwSetScrollCallback(_window, [](GLFWwindow* window, double deltaX, double deltaY)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event(static_cast<float>(deltaX), static_cast<float>(deltaY));
			data.callback(event);
		});

	glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double x, double y)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
			data.callback(event);
		});
}

Minty::WindowsWindow::~WindowsWindow()
{
	// destroy gamepads
	for (auto const& [id, gamepad] : _gamepads)
	{
		delete gamepad.state;
	}

	glfwDestroyWindow(_window);

	unregister_window(this);

	if (_windows.empty())
	{
		glfwTerminate();
	}

	if (this == _main)
	{
		_main = nullptr;
	}
}

void Minty::WindowsWindow::set_title(String const& title)
{
	Window::set_title(title);
	glfwSetWindowTitle(_window, title.c_str());
}

void Minty::WindowsWindow::set_icon(Path const& path)
{
	if (path.empty()) return;

	// https://stackoverflow.com/questions/44321902/load-icon-function-for-glfwsetwindowicon

	GLFWimage image = {};
	image.pixels = stbi_load(path.string().c_str(), &image.width, &image.height, 0, 4);
	glfwSetWindowIcon(_window, 1, &image);
	stbi_image_free(image.pixels);
}

void Minty::WindowsWindow::set_cursor_mode(CursorMode const mode)
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

CursorMode Minty::WindowsWindow::get_cursor_mode() const
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

void Minty::WindowsWindow::maximize()
{
	save_restore_info();
	glfwMaximizeWindow(_window);
}

void Minty::WindowsWindow::minimize()
{
	save_restore_info();
	glfwIconifyWindow(_window);
}

void Minty::WindowsWindow::restore()
{
	glfwSetWindowPos(_window, _restoreX, _restoreY);
	glfwSetWindowSize(_window, get_width(), get_height());
}

Bool Minty::WindowsWindow::is_open() const
{
	return !glfwWindowShouldClose(_window);
}

void Minty::WindowsWindow::close()
{
	glfwSetWindowShouldClose(_window, GLFW_TRUE);
}

void Minty::WindowsWindow::refresh()
{
	glfwGetFramebufferSize(_window, &_frameWidth, &_frameHeight);

	update_size();
}

void Minty::WindowsWindow::process()
{
	glfwPollEvents();

	GLFWgamepadstate state;

	// check for each controller
	for (int i = 0; i <= GLFW_JOYSTICK_LAST; i++)
	{
		if (glfwGetGamepadState(i, &state))
		{
			auto found = _gamepads.find(i);
			if (found == _gamepads.end())
			{
				// newly connected controller
				GLFWgamepadstate* newState = new GLFWgamepadstate();

				// add to connected
				_gamepads.emplace(i, Gamepad
					{
						.state = newState,
						.name = glfwGetGamepadName(i),
					});

				Gamepad const& gamepad = _gamepads.at(i);

				// trigger callback
				GamepadConnectedEvent event(i);
				_data.callback(event);
			}

			// get old data
			GLFWgamepadstate* oldState = _gamepads.at(i).state;

			// check button changes
			for (int j = 0; j <= GLFW_GAMEPAD_BUTTON_LAST; j++)
			{
				if (state.buttons[j] != oldState->buttons[j])
				{
					// trigger callback
					GamepadButtonEvent event(i, static_cast<GamepadButton>(j), static_cast<KeyAction>(state.buttons[j]));
					_data.callback(event);
				}
			}

			// check axes changes
			for (int j = 0; j <= GLFW_GAMEPAD_AXIS_LAST; j++)
			{
				// round to zero if needed, only for joysticks
				if (j <= GLFW_GAMEPAD_AXIS_RIGHT_Y && Math::abs(state.axes[j]) < JOYSTICK_THRESHOLD)
				{
					state.axes[j] = 0.0f;
				}

				// compare to old
				if (state.axes[j] != oldState->axes[j])
				{
					// trigger callback
					GamepadAxisEvent event(i, static_cast<GamepadAxis>(j), state.axes[j]);
					_data.callback(event);
				}
			}

			// copy over new state data
			memcpy(oldState, &state, sizeof(GLFWgamepadstate));
		}
		else if (_gamepads.contains(i))
		{
			Gamepad const& gamepad = _gamepads.at(i);

			// trigger callback
			GamepadDisconnectedEvent event(i);
			_data.callback(event);

			delete gamepad.state;
			_gamepads.erase(i);
		}
	}
}

void Minty::WindowsWindow::poll_events() const
{
	glfwPollEvents();
}

void Minty::WindowsWindow::update_size()
{
	int width, height;
	glfwGetWindowSize(_window, &width, &height);
	_data.width = static_cast<unsigned int>(width);
	_data.height = static_cast<unsigned int>(height);
}

void Minty::WindowsWindow::save_restore_info()
{
	glfwGetWindowPos(_window, &_restoreX, &_restoreY);
	update_size();
}