#pragma once

#include "ME_Console.h"
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
			minty::Project* project;
			bool debug;

			std::string const get_config() const
			{
				return debug ? "Debug" : "Release";
			}
		};

		Info _info;
		Console _console;
	public:
		Application();

		/// <summary>
		/// Runs the game engine application.
		/// </summary>
		/// <param name="argc">The command line argument count.</param>
		/// <param name="argv">The command line arguments.</param>
		int run(int argc, char const* argv[]);
	private:
		void draw_controls();
		void draw_console();

		/// <summary>
		/// Generates all necessary files for the target project.
		/// </summary>
		/// <param name="info"></param>
		void generate();

		/// <summary>
		/// Generates and updates the cmake file for the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void generate_cmake();

		/// <summary>
		/// Generates the main file for the target project.
		/// </summary>
		/// <param name="info"></param>
		void generate_main();

		/// <summary>
		/// Cleans the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void clean();

		/// <summary>
		/// Builds the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void build();

		/// <summary>
		/// Runs the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void run();
	};
}