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

	/// <summary>
	/// Controls all audio within the game engine.
	/// </summary>
	class AudioEngine
	{
	private:
		AudioEngine() = default;
		~AudioEngine() = default;

	public:
		/// <summary>
		/// Initializes the audio engine.
		/// </summary>
		/// <param name="builder"></param>
		static void initialize(AudioEngineBuilder const& builder);

		/// <summary>
		/// Shuts down the audio engine.
		/// </summary>
		static void shutdown();

		/// <summary>
		/// Checks if the given audio handle is valid.
		/// </summary>
		/// <param name="handle">The audio handle to check.</param>
		/// <returns>True if the handle is valid.</returns>
		static Bool is_valid_handle(Handle const handle);

		/// <summary>
		/// Sets the AudioListener data.
		/// </summary>
		/// <param name="listener">The listener data.</param>
		static void set_audio_listener(AudioListener const& listener);

		/// <summary>
		/// Sets the AudioSource data with the given handle.
		/// </summary>
		/// <param name="handle">The handle to the source.</param>
		/// <param name="source">The source data.</param>
		static void set_audio_source(Handle const handle, AudioSource const& source);

		/// <summary>
		/// Applies all changes to the audio engine.
		/// </summary>
		static void apply();

		/// <summary>
		/// Plays the given AudioClip in 2D space.
		/// </summary>
		/// <param name="clip">The clip to play.</param>
		/// <param name="volume">The volume percentage to start at.</param>
		/// <param name="pan">The pan to start at. -1 is left, 1 is right.</param>
		/// <param name="paused">If true, the clip will pause immediately.</param>
		/// <param name="bus">The audio bus to play on.</param>
		/// <returns>The handle to the clip that has begun to play.</returns>
		static Handle play(Ref<AudioClip> const clip, Float const volume = -1.0f, Float const pan = 0.0f, Bool const paused = false, UInt const bus = 0);

		/// <summary>
		/// Plays the given AudioClip in 3D space.
		/// </summary>
		/// <param name="clip">The clip to play.</param>
		/// <param name="source">The audio source data to set.</param>
		/// <param name="volume">The volume percentage to start at.</param>
		/// <param name="paused">If true, the clip will pause immediately.</param>
		/// <param name="bus">The audio bus to play on.</param>
		/// <returns>The handle to the clip that has begun to play.</returns>
		static Handle play_object(Ref<AudioClip> const clip, AudioSource const& source, Float const volume = -1.0f, Bool const paused = false, UInt const bus = 0);

		/// <summary>
		/// Plays the given AudioClip in the background.
		/// </summary>
		/// <param name="clip">The clip to play.</param>
		/// <param name="volume">The volume percentage to start at.</param>
		/// <param name="paused">If true, the clip will pause immediately.</param>
		/// <param name="bus">The audio bus to play on.</param>
		/// <returns>The handle to the clip that has begun to play.</returns>
		static Handle play_background(Ref<AudioClip> const clip, Float const volume = -1.0f, Bool const paused = false, UInt const bus = 0);

		/// <summary>
		/// Pauses the audio playing with the given Handle.
		/// </summary>
		/// <param name="handle">The handle to the audio.</param>
		static void pause(Handle const handle);

		/// <summary>
		/// Pauses all playing audio.
		/// </summary>
		static void pause_all();

		/// <summary>
		/// Unpauses the paused audio with the given Handle.
		/// </summary>
		/// <param name="handle">The handle to the audio.</param>
		static void unpause(Handle const handle);

		/// <summary>
		/// Unpauses all paused audio.
		/// </summary>
		static void unpause_all();

		/// <summary>
		/// Checks if the audio with the given Handle is paused.
		/// </summary>
		/// <param name="handle">The handle to the audio.</param>
		/// <returns>True if valid and paused.</returns>
		static Bool is_paused(Handle const handle);

		/// <summary>
		/// Stops the audio with the given Handle.
		/// </summary>
		/// <param name="handle">The handle to the audio.</param>
		static void stop(Handle const handle);

		/// <summary>
		/// Stops all audio in the engine.
		/// </summary>
		static void stop_all();
	};
}