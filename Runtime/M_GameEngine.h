#pragma once

#include <chrono>

typedef std::chrono::steady_clock::time_point time_point_t;

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
	private:
		time_point_t getNow() const;
	};
}
