#pragma once

#include "M_Engine.h"
#include "M_EntityRegistry.h"
#include "M_Component.h"
#include "M_System.h"
#include <functional>
#include <map>

namespace minty
{
	/// <summary>
	/// Runs a game using the Minty Engine.
	/// </summary>
	class Runtime
	{
	private:
		// the engine that this runtime uses
		Engine _engine;

	public:
		/// <summary>
		/// Creates a new Runtime based on the given arguments.
		/// </summary>
		Runtime(int argc, char const* argv[]);

		~Runtime();

		Engine& get_engine();

		/// <summary>
		/// Runs the Runtime.
		/// </summary>
		/// <returns></returns>
		int run();
	};
}