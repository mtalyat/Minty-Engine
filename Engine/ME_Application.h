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

			std::string const getConfig() const
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
		void clean(Info const& info);

		/// <summary>
		/// Builds the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void build(Info const& info);

		/// <summary>
		/// Runs the target project.
		/// </summary>
		/// <param name="info">The target info.</param>
		void run(Info const& info);

		/// <summary>
		/// Runs the command on the terminal.
		/// </summary>
		/// <param name="command">The command to run.</param>
		void run_command(std::string const& command);
	};
}