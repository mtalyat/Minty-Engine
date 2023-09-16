#include "pch.h"
#include "M_Console.h"

#include <iostream>

void minty::print(std::string const& message)
{
	std::cout << message << std::endl;
}

void minty::print(std::string const& message, ConsoleColor const color)
{
	if (color < ConsoleColor::Gray)
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

void minty::debug_log(std::string const& message)
{
#ifndef NDEBUG
	print(message);
#endif
}

void minty::debug_info(std::string const& message)
{
#ifndef NDEBUG
	print(message, ConsoleColor::Gray);
#endif
}

void minty::debug_warn(std::string const& message)
{
#ifndef NDEBUG
	print(message, ConsoleColor::Yellow);
#endif
}

void minty::debug_error(std::string const& message)
{
#ifndef NDEBUG
	print(message, ConsoleColor::Red);
#endif
}
