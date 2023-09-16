#include "pch.h"
#include "M_Console.h"

#include <iostream>

void minty::console::print(std::string const& message)
{
	std::cout << message << std::endl;
}

void minty::console::print(std::string const& message, Color const color)
{
	if (color < Color::Gray)
	{
		// basic color
		std::cout << "\033[" << static_cast<int>(color) + 30 << ";40m" << message << "\033[0m" << std::endl;
	}
	else
	{
		// advanced color
		std::cout << "\033[" << static_cast<int>(color) + 82 << ";40m" << message << "\033[0m" << std::endl;
	}
}

void minty::console::debug_log(std::string const& message)
{
#ifndef NDEBUG
	print(message);
#endif
}

void minty::console::debug_info(std::string const& message)
{
#ifndef NDEBUG
	print(message, Color::Gray);
#endif
}

void minty::console::debug_warn(std::string const& message)
{
#ifndef NDEBUG
	print(message, Color::Yellow);
#endif
}

void minty::console::debug_error(std::string const& message)
{
#ifndef NDEBUG
	print(message, Color::Red);
#endif
}
