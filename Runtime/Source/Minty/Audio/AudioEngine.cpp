#include "AudioEngine.h"
#include "pch.h"

#include "SoLoud/SoLoudAudioEngine.h"

using namespace Minty;

void Minty::AudioEngine::initialize(AudioEngineBuilder const& builder)
{
	SoLoudAudioEngine::initialize(builder);
}

void Minty::AudioEngine::shutdown()
{
	stop_all();

	SoLoudAudioEngine::shutdown();
}

Bool Minty::AudioEngine::is_valid_handle(Handle const handle)
{
	return SoLoudAudioEngine::is_valid_handle(handle);
}

void Minty::AudioEngine::set_audio_listener(AudioListener const& listener)
{
	SoLoudAudioEngine::set_audio_listener(listener);
}

void Minty::AudioEngine::set_audio_source(Handle const handle, AudioSource const& source)
{
	SoLoudAudioEngine::set_audio_source(handle, source);
}

void Minty::AudioEngine::apply()
{
	SoLoudAudioEngine::apply();
}

Handle Minty::AudioEngine::play(Ref<AudioClip> const clip, Float const volume, Float const pan, Bool const paused, UInt const bus)
{
	return SoLoudAudioEngine::play(clip, volume, pan, paused, bus);
}

Handle Minty::AudioEngine::play_object(Ref<AudioClip> const clip, AudioSource const& source, Float const volume, Bool const paused, UInt const bus)
{
	return SoLoudAudioEngine::play_object(clip, source, volume, paused, bus);
}

Handle Minty::AudioEngine::play_background(Ref<AudioClip> const clip, Float const volume, Bool const paused, UInt const bus)
{
	return SoLoudAudioEngine::play_background(clip, volume, paused, bus);
}

void Minty::AudioEngine::pause(Handle const handle)
{
	SoLoudAudioEngine::pause(handle);
}

void Minty::AudioEngine::pause_all()
{
	SoLoudAudioEngine::pause_all();
}

void Minty::AudioEngine::unpause(Handle const handle)
{
	SoLoudAudioEngine::unpause(handle);
}

void Minty::AudioEngine::unpause_all()
{
	SoLoudAudioEngine::unpause_all();
}

Bool Minty::AudioEngine::is_paused(Handle const handle)
{
	return SoLoudAudioEngine::is_paused(handle);
}

void Minty::AudioEngine::stop(Handle const handle)
{
	SoLoudAudioEngine::stop(handle);
}

void Minty::AudioEngine::stop_all()
{
	SoLoudAudioEngine::stop_all();
}
