#pragma once

#include "M_SceneObject.h"

namespace minty
{
	class AudioEngine;

	class AudioObject
		: public SceneObject
	{
	private:

	public:
		AudioObject();

		AudioObject(Engine& engine, ID const sceneId);

		virtual ~AudioObject();

	public:
		AudioEngine& get_audio_engine() const;
	};
}