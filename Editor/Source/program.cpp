/***********************

	Minty Engine

***********************/

#include "ME_EditorApplication.h"
#include <exception>	// for exception catching
#include <iostream>		// for cout, endl, etc.

int main(int argc, char const* argv[])
{
	Mintye::EditorApplication app;
	return app.run();
}