#pragma once

namespace minty
{
	/// <summary>
	/// Handles the core part of the engine, which includes running a game.
	/// </summary>
	class GameEngine
	{
	public:
		GameEngine();

		~GameEngine();

		/// <summary>
		/// Runs the game.
		/// </summary>
		void run();
	};
}
