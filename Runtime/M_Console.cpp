#include "pch.h"
#include "M_Console.h"

#include "M_Base.h"
#include "M_Node.h"
#include <iostream>

using namespace minty;

void print_raw(std::ostream& stream, String const& message, minty::Console::Color const color)
{
	if (color < minty::Console::Color::Gray)
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

String minty::Console::to_string(Color const value)
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
	default: return "";
	}
}

void minty::Console::print(String const& message)
{
	std::cout << message << std::endl;
}

void minty::Console::print(Node const& node)
{
	for (String const& line : node.get_formatted())
	{
		print(line);
	}
}

void minty::Console::print(String const& message, Color const color)
{
	print_raw(std::cout, message, color);
}

void minty::Console::wait()
{
	print("\nPress enter to continue...");
	String temp;
	std::getline(std::cin, temp);
}

void minty::Console::log_color(String const& message, Color const color)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, message, color);
#endif
}

void minty::Console::log(String const& message)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, message, Color::White);
#endif
}

void minty::Console::todo(String const& message)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, "[todo]: " + message, Color::Magenta);
#endif
}

void minty::Console::test(String const& message)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, "[test]: " + message, Color::Blue);
#endif
}

void minty::Console::info(String const& message)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, "[info]: " + message, Color::Gray);
#endif
}

void minty::Console::warn(String const& message)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, "[warn]: " + message, Color::Yellow);
#endif
}

void minty::Console::error(String const& message)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, "[errr]: " + message, Color::Red);
#endif
}

bool minty::Console::ass(bool const value, String const& errorMessage)
{
#ifdef MINTY_DEBUG
	if (!value)
	{
		Console::error(errorMessage);
	}
	return value;
#else
	return true;
#endif
}
