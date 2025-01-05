#include "pch.h"
#include "AudioClip.h"

#include "Minty/Audio/SoLoud/SoLoudAudioClip.h"

using namespace Minty;

Owner<AudioClip> Minty::AudioClip::create(AudioClipBuilder const& builder)
{
	return Owner<SoLoudAudioClip>(builder);
}
