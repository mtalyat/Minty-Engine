#pragma once

#include <Minty.h>

namespace mintye
{
	/// <summary>
	/// Holds data and runs the game engine application.
	/// </summary>
	class Application
	{
	private:
		struct Info
		{
			minty::Project const& project;
			bool debug;
		};
	public:
		Application();

		void run(int argc, char const* argv[]);
	private:
		void clean(Info const& info);

		void build(Info const& info);

		void run(Info const& info);

		void run_command(std::string const& command);
	};
}