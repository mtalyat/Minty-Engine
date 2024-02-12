#pragma once

namespace minty
{
	class RenderEngine;
	class AudioEngine;
	class Window;

	struct RuntimeBuilder
	{
		Window* const window = nullptr;

		RenderEngine* renderEngine = nullptr;

		AudioEngine* audioEngine = nullptr;
	};
}