#pragma once

#include "M_Engine.h"
#include "M_Info.h"
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
		/// Creates a new Runtime based on the given info.
		/// </summary>
		Runtime(Info const& appInfo);

		~Runtime();

		/// <summary>
		/// Gets the Engine that this Runtime is using.
		/// </summary>
		/// <returns></returns>
		Engine& get_engine();

		/// <summary>
		/// Gets the Engine that this Runtime is using.
		/// </summary>
		/// <returns></returns>
		Engine const& get_engine() const;

		/// <summary>
		/// Runs the Runtime.
		/// </summary>
		/// <returns>The program result code for this Runtime upon completion.</returns>
		int run();
	private:
		// registers all built in systems and components
		void register_builtin();

	public:
		friend std::string to_string(Runtime const& runtime);
	};
}