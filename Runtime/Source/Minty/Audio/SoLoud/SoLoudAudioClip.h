#pragma once
#include "SoLoud.h"

#include "Minty/Audio/AudioClip.h"
#include "Minty/Asset/AssetManager.h"

namespace Minty
{
	class SoLoudAudioClip
		: public AudioClip
	{
	private:
		SoLoud::Wav* mp_clip;

	public:
		SoLoudAudioClip(AudioClipBuilder const& builder);

		~SoLoudAudioClip()
		{
			delete mp_clip;
		}

	public:
		void* get_native() const override { return mp_clip; }
	};
}
