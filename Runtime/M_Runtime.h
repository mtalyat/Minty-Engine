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
		// the engine that this runtime uses
		Engine _engine;

	public:
		/// <summary>
		/// Creates a new Runtime.
		/// </summary>
		Runtime();

		~Runtime();
		
		/// <summary>
		/// Runs the Runtime using the given arguments.
		/// 
		/// The arguments should be: executable name, project path.
		/// </summary>
		/// <param name="argc">The number of arguments.</param>
		/// <param name="argv">The arguments.</param>
		/// <returns></returns>
		int run(int argc, char const* argv[]);
	};
}