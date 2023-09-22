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
	// registry all built-in systems and components
	
}

Runtime::~Runtime()
{
	
}

void minty::Runtime::registerSystem(std::string const& name, SystemFunc const& func)
{
	_systemTypes.emplace(name, func);
}

void minty::Runtime::registerComponent(std::string const& name, ComponentFunc const& func)
{
	_componentTypes.emplace(name, func);
}

System* minty::Runtime::createSystem(std::string const& name, Registry& registry) const
{
	auto const& found = _systemTypes.find(name);
	if (found == _systemTypes.end())
	{
		// name not found
		return nullptr;
	}
	else
	{
		// name found
		return found->second(registry);
	}
}

Component* minty::Runtime::createComponent(std::string const& name, Entity const entity, Registry& registry) const
{
	auto const& found = _componentTypes.find(name);
	if (found == _componentTypes.end())
	{
		// name not found
		return nullptr;
	}
	else
	{
		// name found
		return found->second(registry, entity);
	}
}

Engine& minty::Runtime::getEngine()
{
	return _engine;
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