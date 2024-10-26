#pragma once

#include "Minty/Window/Window.h"
#include "Minty/Data/Register.h"

namespace Minty
{
	struct WindowManagerBuilder
	{

	};

	class WindowManager
	{
	private:
		static Register<Owner<Window>> s_windows;
		static Ref<Window> s_main;

	private:
		WindowManager() = default;
		~WindowManager() = default;

	public:
		static void initialize(WindowManagerBuilder const& builder);

		static void shutdown();

	public:
		static Ref<Window> create_window(WindowBuilder const& builder);

		static void destroy_window(Ref<Window> const window);

		static Ref<Window> get_window(UUID const id);

		static Ref<Window> get_main() { return s_main; }
	};
}