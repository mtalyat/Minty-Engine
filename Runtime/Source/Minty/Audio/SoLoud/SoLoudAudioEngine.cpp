#include "pch.h"
#include "SoLoudAudioEngine.h"

using namespace Minty;

SoLoud::Soloud Minty::SoLoudAudioEngine::s_engine = {};

void Minty::SoLoudAudioEngine::initialize(AudioEngineBuilder const& builder)
{
	s_engine.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::LEFT_HANDED_3D);
}

void Minty::SoLoudAudioEngine::shutdown()
{
	s_engine.deinit();
}

Bool Minty::SoLoudAudioEngine::is_valid_handle(Handle const handle)
{
	return s_engine.isValidVoiceHandle(handle);
}

void Minty::SoLoudAudioEngine::set_audio_listener(AudioListener const& listener)
{
	s_engine.set3dListenerParameters(listener.position.x, listener.position.y, listener.position.z, listener.forward.x, listener.forward.y, listener.forward.z, listener.up.x, listener.up.y, listener.up.z, listener.velocity.x, listener.velocity.y, listener.velocity.z);
}

void Minty::SoLoudAudioEngine::set_audio_source(Handle const handle, AudioSource const& source)
{
	s_engine.set3dSourceParameters(handle, source.position.x, source.position.y, source.position.z, source.velocity.x, source.velocity.y, source.velocity.z);
}

void Minty::SoLoudAudioEngine::apply()
{
	s_engine.update3dAudio();
}

Handle Minty::SoLoudAudioEngine::play(Ref<AudioClip> const clip, Float const volume, Float const pan, Bool const paused, UInt const bus)
{
	void* nativeClip = clip->get_native();
	SoLoud::Wav& native = *static_cast<SoLoud::Wav*>(nativeClip);
	return s_engine.play(native, volume, pan, paused, bus);
}

Handle Minty::SoLoudAudioEngine::play_object(Ref<AudioClip> const clip, AudioSource const& source, Float const volume, Bool const paused, UInt const bus)
{
	void* nativeClip = clip->get_native();
	SoLoud::Wav& native = *static_cast<SoLoud::Wav*>(nativeClip);
	Handle handle = s_engine.play3d(native, source.position.x, source.position.y, source.position.z, source.velocity.x, source.velocity.y, source.velocity.z, volume, paused, bus);
	s_engine.set3dSourceAttenuation(handle, static_cast<UInt>(source.attenuation), source.attenuationRollOff);
	s_engine.set3dSourceMinMaxDistance(handle, source.minDistance, source.maxDistance);
	return handle;
}

Handle Minty::SoLoudAudioEngine::play_background(Ref<AudioClip> const clip, Float const volume, Bool const paused, UInt const bus)
{
	void* nativeClip = clip->get_native();
	SoLoud::Wav& native = *static_cast<SoLoud::Wav*>(nativeClip);
	return s_engine.playBackground(native, volume, paused, bus);
}

void Minty::SoLoudAudioEngine::pause(Handle const handle)
{
	s_engine.setPause(handle, true);
}

void Minty::SoLoudAudioEngine::pause_all()
{
	s_engine.setPauseAll(true);
}

void Minty::SoLoudAudioEngine::unpause(Handle const handle)
{
	s_engine.setPause(handle, false);
}

void Minty::SoLoudAudioEngine::unpause_all()
{
	s_engine.setPauseAll(false);
}

Bool Minty::SoLoudAudioEngine::is_paused(Handle const handle)
{
	return s_engine.getPause(handle);
}

void Minty::SoLoudAudioEngine::stop(Handle const handle)
{
	s_engine.stop(handle);
}

void Minty::SoLoudAudioEngine::stop_all()
{
	s_engine.stopAll();
}
