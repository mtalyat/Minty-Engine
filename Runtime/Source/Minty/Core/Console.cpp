#include "pch.h"
#include "Console.h"

#include "Minty/Core/Base.h"
//#include "Minty/Types/M_Node.h"
#include <iostream>

using namespace Minty;

void print_raw(std::ostream& stream, String const& message, Minty::Console::Color const color)
{
	if (color < Minty::Console::Color::Gray)
	{
		// basic color
		stream << "\033[" << static_cast<Int>(color) + 30 << ";40m" << message << "\033[0m" << std::endl;
	}
	else
	{
		// advanced color
		stream << "\033[" << static_cast<Int>(color) + 82 << ";40m" << message << "\033[0m" << std::endl;
	}
}

void Minty::Console::print(String const& message)
{
	std::cout << message << std::endl;
}

//void Minty::Console::print(Node const& node)
//{
//	for (String const& line : node.get_formatted())
//	{
//		print(line);
//	}
//}

void Minty::Console::print(String const& message, Color const color)
{
	print_raw(std::cout, message, color);
}

void Minty::Console::wait()
{
	print("\nPress enter to continue...");
	String temp;
	std::getline(std::cin, temp);
}

void Minty::Console::log_color(String const& message, Color const color)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, message, color);
#endif
}

void Minty::Console::log(String const& message)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, message, Color::White);
#endif
}

void Minty::Console::todo(String const& message)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, "[todo]: " + message, Color::Magenta);
#endif
}

void Minty::Console::test(String const& message)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, "[test]: " + message, Color::Blue);
#endif
}

void Minty::Console::info(String const& message)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, "[info]: " + message, Color::Gray);
#endif
}

void Minty::Console::warn(String const& message)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, "[warn]: " + message, Color::Yellow);
#endif
}

void Minty::Console::error(String const& message)
{
#ifdef MINTY_DEBUG
	print_raw(std::cout, "[errr]: " + message, Color::Red);
#endif
}
