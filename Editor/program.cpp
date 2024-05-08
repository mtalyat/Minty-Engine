/***********************

	Minty Engine

***********************/

#include "ME_EditorApplication.h"
#include <exception>	// for exception catching
#include <iostream>		// for cout, endl, etc.

int main(int argc, char const* argv[])
{
	mintye::EditorApplication app;
	return app.execute();
}