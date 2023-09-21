#include "pch.h"
#include "M_Runtime.h"

#include "M_CommandLineParser.h"
#include "M_Console.h"
#include <filesystem>
#include <iostream>
#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <exception>

using namespace minty;

Runtime::Runtime()
	: _engine()
{
}

Runtime::~Runtime()
{
	
}

int Runtime::run(int argc, char const* argv[])
{
	// parse command line arguments...

	// add parameters
	CommandLineParser parser;
	parser.addParameter(CommandLineParser::Parameter("path", 1));

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

	try
	{
		// TODO: load game

		_engine.run();
	}
	catch (std::exception const& e)
	{
		console::error(e.what());
		return EXIT_FAILURE;
	}
	catch (...)
	{
		console::error("An uncaught exception occured.");
	}

	return EXIT_SUCCESS;
}