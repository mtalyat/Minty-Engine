#pragma once

namespace minty
{
	class RenderEngine;
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