/***********************

	Minty Engine

***********************/

#include "ME_Application.h"
#include <exception>	// for exception catching
#include <iostream>		// for cout, endl, etc.

int main(int argc, char const* argv[])
{
	mintye::Application app;

	try
	{
		app.run(argc, argv);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}