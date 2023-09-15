#include "pch.h"
#include "M_Runtime.h"

#include "M_CommandLineParser.h"
#include "M_GameEngine.h"
#include <filesystem>
#include <iostream>
#include <exception>

using namespace minty;

int Runtime::run(int argc, char const* argv[])
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

		// change working directory to project path
		std::filesystem::current_path(arg.args[0]);
	}
	else
	{
		std::cerr << "Path argument not found." << std::endl;

		return EXIT_FAILURE;
	}

	if (parser.getArgument("debug"))
	{
		std::cout << "Debug mode true" << std::endl;
	}
	else
	{
		std::cout << "Debug mode false" << std::endl;
	}

	try
	{
		GameEngine engine;

		// TODO: load game

		engine.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
