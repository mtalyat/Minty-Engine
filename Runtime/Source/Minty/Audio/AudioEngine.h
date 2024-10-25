#pragma once

#include "Minty/Audio/AudioClip.h"
#include "Minty/Audio/AudioListener.h"
#include "Minty/Audio/AudioSource.h"
#include "Minty/Core/Type.h"

namespace Minty
{
	struct AudioEngineBuilder
	{

	};

	class AudioEngine
	{
	private:
		AudioEngine() = default;
		~AudioEngine() = default;

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