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

			std::string const get_config() const
			{
				return debug ? "Debug" : "Release";
			}
		};
	public:
		Application();

		/// <summary>
		/// Runs the game engine application.
		/// </summary>
		/// <param name="argc">The command line argument count.</param>
		/// <param name="argv">The command line arguments.</param>
		void run(int argc, char const* argv[]);
	private:
		/// <summary>
		/// Generates all necessary files for the target project.
		/// </summary>
		/// <param name="info"></param>
		void generate(Info const& info);

		/// <summary>
		/// Generates and updates the cmake file for the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void generate_cmake(Info const& info);

		/// <summary>
		/// Generates the main file for the target project.
		/// </summary>
		/// <param name="info"></param>
		void generate_main(Info const& info);

		/// <summary>
		/// Cleans the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		size_t clean(Info const& info);

		/// <summary>
		/// Builds the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		size_t build(Info const& info);

		/// <summary>
		/// Runs the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		size_t run(Info const& info);

		/// <summary>
		/// Runs the command on the terminal.
		/// </summary>
		/// <param name="command">The command to run.</param>

		/// <summary>
		/// Runs the command on the terminal.
		/// </summary>
		/// <param name="command">The command to run.</param>
		/// <returns>The number of errors that were printed during the execution of the command.</returns>
		size_t run_command(std::string const& command);
	};
}