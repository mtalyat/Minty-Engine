#pragma once

namespace minty
{
	class RenderEngine;
	class AudioEngine;
	class ScriptEngine;
	class Window;

	struct RuntimeBuilder
	{
		Window* const window = nullptr;

		RenderEngine* renderEngine = nullptr;

		AudioEngine* audioEngine = nullptr;

		ScriptEngine* scriptEngine = nullptr;
	};
}