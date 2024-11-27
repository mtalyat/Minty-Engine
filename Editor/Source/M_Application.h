#pragma once

#include "renderengine.h"

namespace minty
{
	/// <summary>
	/// Holds data and runs the game engine application.
	/// </summary>
	class Application
	{
	public:
		void run()
		{
			RenderEngine engine;

			// TODO: load game

			// run in engine
			engine.run();
		}
	};
}