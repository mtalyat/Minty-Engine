#pragma once

#include <string>

namespace minty
{
	namespace console
	{
		/// <summary>
		/// Defines a color that can be printed to the console.
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

		/// <summary>
		/// Prints the string to the console.
		/// </summary>
		/// <param name="message">The string to print.</param>
		void print(std::string const& message);

		/// <summary>
		/// Prints the string to the console in the given color.
		/// </summary>
		/// <param name="message">The string to print.</param>
		/// <param name="color">The color to print the message.</param>
		void print(std::string const& message, Color const color);

		/// <summary>
		/// Debug log prints the given message to the screen.
		/// </summary>
		/// <param name="message">The string to print.</param>
		void log(std::string const& message);

		/// <summary>
		/// Debug info prints the given message to the screen, in gray.
		/// </summary>
		/// <param name="message">The string to print.</param>
		void info(std::string const& message);

		/// <summary>
		/// Debug warn prints the given message to the screen, in yellow.
		/// </summary>
		/// <param name="message">The string to print.</param>
		void warn(std::string const& message);

		/// <summary>
		/// Debug error prints the given message to the screen, in red.
		/// </summary>
		/// <param name="message">The string to print.</param>
		void error(std::string const& message);
	}
}