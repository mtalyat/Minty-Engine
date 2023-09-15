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
		Application();

		void run(int argc, char const* argv[]);
	private:
		void clean(minty::Project const& project);

		void build(minty::Project const& project);

		void run(minty::Project const& project);

		void run_command(std::string const& command);
	};
}