#pragma once

#include "runtime/M_RuntimeObject.h"

namespace minty
{
	class AudioEngine;

	class AudioObject
		: public RuntimeObject
	{
	private:

	public:
		AudioObject();

		AudioObject(Runtime& engine);

		virtual ~AudioObject();

	public:
		AudioEngine& get_audio_engine() const;
	};
}