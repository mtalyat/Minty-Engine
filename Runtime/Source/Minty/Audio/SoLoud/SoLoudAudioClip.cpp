#include "pch.h"
#include "SoLoudAudioClip.h"

using namespace Minty;

Minty::SoLoudAudioClip::SoLoudAudioClip(AudioClipBuilder const& builder)
	: AudioClip(builder)
	, mp_clip(new SoLoud::Wav())
{
	// load clip
	SoLoud::result result = mp_clip->loadMem(builder.data.data(), static_cast<UInt>(builder.data.size()), true, false);

	MINTY_ASSERT_FORMAT(result == SoLoud::SOLOUD_ERRORS::SO_NO_ERROR, "Failed to loud AudioClip with SoLoud error code \"{}\".", result);

	mp_clip->setVolume(builder.volume);
	mp_clip->setLooping(builder.looping);
	mp_clip->setLoopPoint(builder.loopPoint);
	mp_clip->setSingleInstance(builder.singleInstance);
}
