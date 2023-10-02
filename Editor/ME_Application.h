#pragma once

#include "ME_Console.h"
#include "ME_Project.h"
#include "ME_GuiRenderer.h"
#include <Minty.h>

namespace mintye
{
	/// <summary>
	/// Holds data and runs the game engine application.
	/// </summary>
	class Application
	{
	private:
		struct BuildInfo
		{
			Project* project;
			bool debug;

			BuildInfo(Project* const project, bool const debug);

			std::string const get_config() const;
		};

		Project _project;
		BuildInfo _info;
		minty::Window _window;
		GuiRenderer _renderer;
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
		void draw_commands();
		void draw_console();
		void draw_hierarchy();
		void draw_inspector();
		void draw_game();
		void draw_scene();

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