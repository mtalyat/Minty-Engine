#include "pch.h"
#include "WindowManager.h"

#include "Minty/Script/ScriptEngine.h"

#if defined(MINTY_WINDOWS)
#include "Platform/Windows/WindowsWindowManager.h"
#endif

using namespace Minty;

Register<Owner<Window>> WindowManager::s_windows = {};
Ref<Window> WindowManager::s_main = {};

void Minty::WindowManager::initialize(WindowManagerBuilder const& builder)
{
#if defined(MINTY_WINDOWS)
	WindowsWindowManager::initialize(builder);
#endif
}

void Minty::WindowManager::shutdown()
{
#if defined(MINTY_WINDOWS)
	WindowsWindowManager::shutdown();
#endif

	// close all windows
	for (auto const& [id, window] : s_windows)
	{
		window->close();
	}
	s_windows.clear();
}

Ref<Window> Minty::WindowManager::create_window(WindowBuilder const& builder)
{
	// make sure not too many windows
	MINTY_ASSERT_FORMAT(s_windows.size() < MAX_WINDOW_COUNT, "Cannot create Window. The maximum number of windows allowed is {}.", MAX_WINDOW_COUNT);

	// create window
	Owner<Window> window = Window::create(builder);

	Ref<Window> ref = window.create_ref();

	// add to registry
	s_windows.emplace(builder.id, window);

	// if no main: select this one
	if (s_main == nullptr)
	{
		s_main = ref;
	}
	
	return ref;
}

void Minty::WindowManager::destroy_window(Ref<Window> const window)
{
	// close window
	window->close();

	// if this was main, remove reference
	if (s_main == window)
	{
		s_main = nullptr;
	}

	// remove from registry
	s_windows.erase(window->id());
}

Ref<Window> Minty::WindowManager::get_window(UUID const id)
{
	auto found = s_windows.find(id);

	if (found == s_windows.end())
	{
		return {};
	}

	return found->second;
}
