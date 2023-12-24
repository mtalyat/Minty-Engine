#include "pch.h"
#include "M_Console.h"

#include <iostream>

using namespace minty;

void print_raw(std::ostream& stream, String const& message, minty::console::Color const color)
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

String minty::console::to_string(Color const value)
{
	switch (value) {
	case Color::Black: return "Black";
	case Color::Red: return "Red";
	case Color::Green: return "Green";
	case Color::Yellow: return "Yellow";
	case Color::Blue: return "Blue";
	case Color::Magenta: return "Magenta";
	case Color::Cyan: return "Cyan";
	case Color::White: return "White";
	case Color::Gray: return "Gray";
	case Color::BrightRed: return "Bright Red";
	case Color::BrightGreen: return "Bright Green";
	case Color::BrightYellow: return "Bright Yellow";
	case Color::BrightBlue: return "Bright Blue";
	case Color::BrightMagenta: return "Bright Magenta";
	case Color::BrightCyan: return "Bright Cyan";
	case Color::BrightWhite: return "Bright White";
	default: return minty::error::ERROR_TEXT;
	}
}

void minty::console::print(String const& message)
{
	std::cout << message << std::endl;
}

void minty::console::print(String const& message, Color const color)
{
	print_raw(std::cout, message, color);
}

void minty::console::wait()
{
	print("\nPress enter to continue...");
	String temp;
	std::getline(std::cin, temp);
}

void minty::console::log(String const& message, Color const color)
{
#ifndef NDEBUG
	print_raw(std::cout, message, color);
#endif
}

void minty::console::todo(String const& message)
{
#ifndef NDEBUG
	print_raw(std::cout, "[todo]: " + message, Color::Magenta);
#endif
}

void minty::console::test(String const& message, Color const color)
{
#ifndef NDEBUG
	print_raw(std::cout, "[test]: " + message, color);
#endif
}

void minty::console::info(String const& message)
{
#ifndef NDEBUG
	print_raw(std::cout, "[info]: " + message, Color::Gray);
#endif
}

void minty::console::warn(String const& message)
{
#ifndef NDEBUG
	print_raw(std::cout, "[warn]: " + message, Color::Yellow);
#endif
}

void minty::console::error(String const& message)
{
#ifndef NDEBUG
	print_raw(std::cout, "[errr]: " + message, Color::Red);
#endif
}

bool minty::console::ass(bool const value, String const& errorMessage)
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
