#pragma once

#include "M_GameEngine.h"

namespace minty
{
	/// <summary>
	/// Runs a game using the Minty Engine.
	/// </summary>
	class Runtime
	{
	private:
		GameEngine* const _engine;

	public:
		Runtime();

		~Runtime();

		int run(int argc, char const* argv[]);

		GameEngine* getEngine() const;
	};
}