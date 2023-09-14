#pragma once

#include "M_GameEngine.h"

namespace minty
{
	/// <summary>
	/// Runs a game using the Minty Engine.
	/// </summary>
	class Runtime
	{
	public:
		void run()
		{
			GameEngine engine;

			// TODO: load game

			// run in engine
			engine.run();
		}
	};
}