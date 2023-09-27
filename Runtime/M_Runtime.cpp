#include "pch.h"
#include "M_Runtime.h"

#include "M_SystemRegistry.h"

#include "M_EntityRegistry.h"
#include "M_CameraComponent.h"
#include "M_NameComponent.h"
#include "M_PositionComponent.h"
#include "M_RotationComponent.h"
#include "M_ScaleComponent.h"

#include "M_CommandLineParser.h"
#include "M_Console.h"
#include <filesystem>
#include <iostream>
#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <exception>

using namespace minty;

minty::Runtime::Runtime(int argc, char const* argv[])
	: _engine()
{
	// parse command line arguments...

	// add parameters
	CommandLineParser parser;
	parser.add_parameter(CommandLineParser::Parameter("path", 1));

	// parse the args
	parser.parse(argc, argv);

	// check for args
	CommandLineParser::Argument arg;

	if (parser.get_argument("path", arg))
	{
		std::cout << "Path argument: " << arg.args[0] << std::endl;

		// change working directory to project path
		std::filesystem::current_path(arg.args[0]);
	}
	else
	{
		std::cerr << "Path argument not found." << std::endl;
	}

	register_builtin();
}

Runtime::~Runtime()
{
	
}

Engine& minty::Runtime::get_engine()
{
	return _engine;
}

int Runtime::run()
{
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

void minty::Runtime::register_builtin()
{
	// systems
	//SystemRegistry::register_system<RendererSystem>("Renderer");

	// components
	EntityRegistry::register_component<CameraComponent>("Camera");
	EntityRegistry::register_component<NameComponent>("Name");
	EntityRegistry::register_component<PositionComponent>("Position");
	EntityRegistry::register_component<RotationComponent>("Rotation");
	EntityRegistry::register_component<ScaleComponent>("Scale");
}
