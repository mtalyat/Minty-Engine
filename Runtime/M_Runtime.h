#pragma once

#include "M_Engine.h"

namespace minty
{
	class Info;

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
	private:
		// registers all built in systems and components
		void register_builtin();

	public:
		friend String to_string(Runtime const& runtime);
	};
}