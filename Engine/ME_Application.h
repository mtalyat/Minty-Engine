#pragma once

#include <Minty.h>

namespace mintye
{
	/// <summary>
	/// Holds data and runs the game engine application.
	/// </summary>
	class Application
	{
	public:
		void run(int argc, char const* argv[])
		{
			minty::Runtime runtime;
			runtime.run(argc, argv);
		}
	};
}