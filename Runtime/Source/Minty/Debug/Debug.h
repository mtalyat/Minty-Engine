#pragma once

#include "Minty/Core/Console.h"
#include "Minty/Core/String.h"
#include "Minty/Core/Types.h"
#include "Minty/Debug/Logger.h"

namespace Minty
{
	class Debug
	{
	private:
		Debug() = default;
		~Debug() = default;

	public:
		static void log(String const& text);

		static void log_color(String const& text, Console::Color const color);

		static void log_warning(String const& text);

		static void log_error(String const& text);

		static void log_info(String const& text);

		static void log_todo(String const& text);

		static void log_stack_trace();
	};
}