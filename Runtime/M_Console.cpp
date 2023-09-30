#include "pch.h"
#include "M_Console.h"

#include <iostream>

void print_raw(std::ostream& stream, std::string const& message, minty::console::Color const color)
{
	if (color < minty::console::Color::Gray)
	{
		// basic color
		stream << "\033[" << static_cast<int>(color) + 30 << ";40m" << message << "\033[0m" << std::endl;
	}
	else
	{
		// advanced color
		stream << "\033[" << static_cast<int>(color) + 82 << ";40m" << message << "\033[0m" << std::endl;
	}
}

void minty::console::print(std::string const& message)
{
	std::cout << message << std::endl;
}

void minty::console::print(std::string const& message, Color const color)
{
	print_raw(std::cout, message, color);
}

void minty::console::wait()
{
	print("\nPress enter to continue...");
	std::string temp;
	std::getline(std::cin, temp);
}

void minty::console::log(std::string const& message, Color const color)
{
#ifndef NDEBUG
	print_raw(std::cout, message, color);
#endif
}

void minty::console::todo(std::string const& message)
{
#ifndef NDEBUG
	print_raw(std::cerr, "[todo]: " + message, Color::Cyan);
#endif
}

void minty::console::test(std::string const& message, Color const color)
{
#ifndef NDEBUG
	print_raw(std::cerr, "[test]: " + message, color);
#endif
}

void minty::console::info(std::string const& message)
{
#ifndef NDEBUG
	print_raw(std::cerr, "[info]: " + message, Color::Gray);
#endif
}

void minty::console::warn(std::string const& message)
{
#ifndef NDEBUG
	print_raw(std::cerr, "[warn]: " + message, Color::Yellow);
#endif
}

void minty::console::error(std::string const& message)
{
#ifndef NDEBUG
	print_raw(std::cerr, "[erro]: " + message, Color::Red);
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
