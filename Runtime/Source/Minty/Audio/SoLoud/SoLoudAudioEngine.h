#pragma once
#include "Minty/Audio/AudioEngine.h"

#include "Minty/Audio/SoLoud/SoLoudAudioClip.h"

namespace Minty
{
	class SoLoudAudioEngine
	{
	private:
		static SoLoud::Soloud s_engine;

	private:
		SoLoudAudioEngine() = default;
		~SoLoudAudioEngine() = default;

	public:
		static void initialize(AudioEngineBuilder const& builder);

		static void shutdown();

		static Bool is_valid_handle(Handle const handle);

		static void set_audio_listener(AudioListener const& listener);

		static void set_audio_source(Handle const handle, AudioSource const& source);

		static void apply();

		static Handle play(Ref<AudioClip> const clip, Float const volume = -1.0f, Float const pan = 0.0f, Bool const paused = false, UInt const bus = 0);

		static Handle play_object(Ref<AudioClip> const clip, AudioSource const& source, Float const volume = -1.0f, Bool const paused = false, UInt const bus = 0);

		static Handle play_background(Ref<AudioClip> const clip, Float const volume = -1.0f, Bool const paused = false, UInt const bus = 0);

		static void pause(Handle const handle);

		static void pause_all();

		static void unpause(Handle const handle);

		static void unpause_all();

		static Bool is_paused(Handle const handle);

		static void stop(Handle const handle);

		static void stop_all();
	};
}