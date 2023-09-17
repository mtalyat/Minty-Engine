#pragma once

#include "M_Engine.h"

namespace minty
{
	/// <summary>
	/// Runs a game using the Minty Engine.
	/// </summary>
	class Runtime
	{
	private:
		Engine* const _engine;

	public:
		Runtime();

		~Runtime();

		int run(int argc, char const* argv[]);

		Engine* getEngine() const;
	};
}