#pragma once
#include "System.h"

#include "Minty/Audio/AudioClip.h"
#include "Minty/Audio/AudioListener.h"
#include "Minty/Audio/AudioSource.h"
#include "Minty/Entity/Entity.h"
#include "Minty/Core/Math.h"
#include <unordered_map>

namespace Minty
{
	class AudioEngine;

	class AudioSystem
		: public System
	{
	private:
		std::unordered_map<String, Ref<AudioClip>> m_clips;
		std::unordered_map<Handle, Entity> m_playing;

		Entity m_listener;
		AudioListener m_listenerData;
	public:
		AudioSystem(Scene& scene);

		~AudioSystem() = default;

		void update(Time const& time) override;

		void set_listener(Entity const entity);

		/// <summary>
		/// Plays the clip with the given clip.
		/// </summary>
		/// <param name="clip">The AudioClip to play.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="pan">The pan of the AudioClip. -1.0 is left, 0.0 is centered, and 1.0 is right.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		Handle play(Ref<AudioClip> const clip, Float const volume = -1.0f, Float const pan = 0.0f, Bool const paused = false, UInt const bus = 0);

		/// <summary>
		/// Plays the clip with the given name.
		/// </summary>
		/// <param name="name">The name of the AudioClip to play.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="pan">The pan of the AudioClip. -1.0 is left, 0.0 is centered, and 1.0 is right.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		Handle play(String const& name, Float const volume = -1.0f, Float const pan = 0.0f, Bool const paused = false, UInt const bus = 0);

		/// <summary>
		/// Plays the clip on the Entity's AudioSource, in 3D space.
		/// </summary>
		/// <param name="entity">The entity to play the clip on. Must have an AudioSource component.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		Handle play_object(Entity const entity, Float const volume = -1.0f, Bool const paused = false, UInt const bus = 0);

		/// <summary>
		/// Plays the AudioClip using the given clip on the Entity's AudioSource, in 3D space.
		/// </summary>
		/// <param name="clip">The AudioClip to play.</param>
		/// <param name="entity">The entity to play the clip on. Must have an AudioSource component.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		Handle play_object(Ref<AudioClip> const clip, Entity const entity, Float const volume = -1.0f, Bool const paused = false, UInt const bus = 0);

		/// <summary>
		/// Plays the AudioClip using the given name on the Entity's AudioSource, in 3D space.
		/// </summary>
		/// <param name="name">The name of the AudioClip to play.</param>
		/// <param name="entity">The entity to play the clip on. Must have an AudioSource component.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		Handle play_object(String const& name, Entity const entity, Float const volume = -1.0f, Bool const paused = false, UInt const bus = 0);

		/// <summary>
		/// Plays the AudioClip using the given clip in the background.
		/// </summary>
		/// <param name="clip">The AudioClip to play.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		Handle play_background(Ref<AudioClip> const clip, Float const volume = -1.0f, Bool const paused = false, UInt const bus = 0);

		/// <summary>
		/// Plays the AudioClip using the given name in the background.
		/// </summary>
		/// <param name="name">The name of the AudioClip to play.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		Handle play_background(String const& name, Float const volume = -1.0f, Bool const paused = false, UInt const bus = 0);

		/// <summary>
		/// Checks if the audio clip with the given handle is still playing.
		/// </summary>
		/// <param name="handle">The handle of the audio.</param>
		/// <returns>True if the audio is still playing, otherwise false.</returns>
		Bool is_playing(Handle const handle);

		/// <summary>
		/// Stops playing the audio clip with the given handle.
		/// </summary>
		/// <param name="handle">The handle of the audio clip to stop.</param>
		void stop(Handle const handle);

		/// <summary>
		/// Stops all audio clips playing in this AudioEngine.
		/// </summary>
		void stop_all();
	private:
		// updates the given sound data using the given entity
		void initialize_audio_listener(Entity const entity, AudioListener& listener);

		void initialize_audio_source(Entity const entity, AudioSource& source);
	};
}