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

		AudioObject(Runtime& engine, ID const sceneId);

		virtual ~AudioObject();

	public:
		AudioEngine& get_audio_engine() const;
	};
}