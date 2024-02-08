#pragma once

#include "M_Base.h"
#include "M_Node.h"

namespace minty::Console
{
	/// <summary>
	/// Defines a color that can be printed to the Console.
	/// </summary>
	enum Color
	{
		Black = 0,
		Red = 1,
		Green = 2,
		Yellow = 3,
		Blue = 4,
		Magenta = 5,
		Cyan = 6,
		White = 7,

		Gray = 8,
		BrightRed = 9,
		BrightGreen = 10,
		BrightYellow = 11,
		BrightBlue = 12,
		BrightMagenta = 13,
		BrightCyan = 14,
		BrightWhite = 15
	};

	String to_string(Color const value);

	/// <summary>
	/// Prints the String to the Console.
	/// </summary>
	/// <param name="message">The String to print.</param>
	void print(String const& message);

	/// <summary>
	/// Prints the Node to the Console.
	/// </summary>
	/// <param name="node">The Node to print.</param>
	void print(Node const& node);

	/// <summary>
	/// Prints the string to the Console in the given color.
	/// </summary>
	/// <param name="message">The string to print.</param>
	/// <param name="color">The color to print the message.</param>
	void print(String const& message, Color const color);

	/// <summary>
	/// Waits until the user presses the enter key.
	/// </summary>
	void wait();

	/// <summary>
	/// Debug log prints the given message to the screen.
	/// </summary>
	/// <param name="message">The string to print.</param>
	void log(String const& message, Color const color = Color::White);

	/// <summary>
	/// Debug log prints the given message to the screen, in cyan.
	/// </summary>
	/// <param name="message">The string to print.</param>
	void todo(String const& message);

	/// <summary>
	/// Debug log prints the given message to the screen, in blue.
	/// </summary>
	/// <param name="message">The string to print.</param>
	void test(String const& message, Color const color = Color::Blue);

	/// <summary>
	/// Debug info prints the given message to the screen, in gray.
	/// </summary>
	/// <param name="message">The string to print.</param>
	void info(String const& message);

	/// <summary>
	/// Debug warn prints the given message to the screen, in yellow.
	/// </summary>
	/// <param name="message">The string to print.</param>
	void warn(String const& message);

	/// <summary>
	/// Debug error prints the given message to the screen, in red.
	/// </summary>
	/// <param name="message">The string to print.</param>
	void error(String const& message);

	/// <summary>
	/// Prints an error message if the given condition is false.
	/// Short for assert.
	/// </summary>
	/// <param name="value">The value to check.</param>
	/// <param name="errorMessage">The error message.</param>
	/// <return>True if the value passed, or if not in debug mode.</return>
	bool ass(bool const value, String const& errorMessage);
}