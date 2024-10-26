#pragma once

#include "Minty/Core/WindowManager.h"

namespace Minty
{
	class WindowsWindowManager
	{
	private:
		WindowsWindowManager() = default;
		~WindowsWindowManager() = default;

	public:
		static void initialize(WindowManagerBuilder const& builder);
		static void shutdown();
	};
}