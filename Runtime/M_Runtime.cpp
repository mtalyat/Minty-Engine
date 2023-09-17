#include "pch.h"
#include "M_Runtime.h"

#include "M_CommandLineParser.h"
#include <filesystem>
#include <iostream>
#include <exception>

using namespace minty;

Runtime::Runtime()
	: _engine(new Engine())
{
}

Runtime::~Runtime()
{
	delete _engine;
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

		_engine->run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

Engine* Runtime::getEngine() const
{
	return _engine;
}
