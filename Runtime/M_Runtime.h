#pragma once

#include "M_Engine.h"
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

	private:
		// registers all built in systems and components
		void register_builtin();
	};
}