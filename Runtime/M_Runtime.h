#pragma once

#include "M_CommandLineParser.h"
#include "M_GameEngine.h"
#include <iostream>

namespace minty
{
	/// <summary>
	/// Runs a game using the Minty Engine.
	/// </summary>
	class Runtime
	{
	public:
		void run(int argc, char const* argv[])
		{
			// parse command line arguments...

			// add parameters
			CommandLineParser parser;
			parser.addParameter(CommandLineParser::Parameter("path", 1));
			parser.addParameter(CommandLineParser::Parameter("debug", "-d", 0));

			// parse the args
			parser.parse(argc, argv);

			// check for args
			CommandLineParser::Argument arg;

			if (parser.getArgument("path", arg))
			{
				std::cout << "Path argument: " << arg.args[0] << std::endl;
			}
			else
			{
				std::cout << "Path argument not found." << std::endl;
			}

			if (parser.getArgument("debug"))
			{
				std::cout << "Debug mode true" << std::endl;
			}
			else
			{
				std::cout << "Debug mode false" << std::endl;
			}

			GameEngine engine;

			//// TODO: load game

			// run in engine
			engine.run();
		}
	};
}