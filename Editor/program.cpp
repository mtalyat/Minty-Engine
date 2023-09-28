/***********************

	Minty Engine

***********************/

#include "ME_Application.h"
#include <exception>	// for exception catching
#include <iostream>		// for cout, endl, etc.

int main(int argc, char const* argv[])
{
	mintye::Application app;
	app.run(argc, argv);

	return EXIT_SUCCESS;
}