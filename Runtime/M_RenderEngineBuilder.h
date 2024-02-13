#pragma once

namespace minty
{
	class Info;
	class Window;

	/// <summary>
	/// Holds data to create a new RenderEngine.
	/// </summary>
	struct RenderEngineBuilder
	{
		Info const* info;

		Window* window;
	};
}