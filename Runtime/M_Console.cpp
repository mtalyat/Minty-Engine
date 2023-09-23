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

void minty::console::wait()
{
	print("\nPress enter to continue...");
	std::string temp;
	std::getline(std::cin, temp);
}

void minty::console::log(std::string const& message)
{
#ifndef NDEBUG
	print(message, Color::White);
#endif
}

void minty::console::todo(std::string const& message)
{
#ifndef NDEBUG
	print("TODO: " + message, Color::Cyan);
#endif
}

void minty::console::test(std::string const& message)
{
#ifndef NDEBUG
	print(message, Color::Blue);
#endif
}

void minty::console::info(std::string const& message)
{
#ifndef NDEBUG
	print(message, Color::Gray);
#endif
}

void minty::console::warn(std::string const& message)
{
#ifndef NDEBUG
	print(message, Color::Yellow);
#endif
}

void minty::console::error(std::string const& message)
{
#ifndef NDEBUG
	print(message, Color::Red);
#endif
}

bool minty::console::ass(bool const value, std::string const& errorMessage)
{
#ifndef NDEBUG
	if (!value)
	{
		console::error(errorMessage);
	}
	return value;
#else
	return true;
#endif
}
