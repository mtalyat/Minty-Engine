#pragma once
#include "Minty/Systems/M_System.h"

#include "Minty/Types/M_Register.h"
#include "Minty/Audio/M_Audio.h"
#include "Minty/Audio/M_AudioClip.h"
#include "Minty/Entities/M_Entity.h"
#include "Minty/Types/M_Vector.h"
#include <unordered_map>

namespace Minty
{
	class AudioEngine;

	class AudioSystem
		: public System
	{
	private:
		struct SoundData
		{
			Vector3 position;
			Vector3 velocity;
		};
	private:
		std::unordered_map<String, AudioClip*> _clips;
		std::unordered_map<AudioHandle, Entity> _playing;

		Entity _listener;
		SoundData _listenerData;
	public:
		AudioSystem(Scene& scene);

		void update(Time const time) override;

		void set_listener(Entity const entity);

		/// <summary>
		/// Plays the clip with the given clip.
		/// </summary>
		/// <param name="clip">The AudioClip to play.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="pan">The pan of the AudioClip. -1.0 is left, 0.0 is centered, and 1.0 is right.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		AudioHandle play(AudioClip& clip, float const volume = -1.0f, float const pan = 0.0f, bool const paused = false, unsigned int const bus = 0);

		/// <summary>
		/// Plays the clip with the given name.
		/// </summary>
		/// <param name="name">The name of the AudioClip to play.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="pan">The pan of the AudioClip. -1.0 is left, 0.0 is centered, and 1.0 is right.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		AudioHandle play(String const& name, float const volume = -1.0f, float const pan = 0.0f, bool const paused = false, unsigned int const bus = 0);

		/// <summary>
		/// Plays the clip on the Entity's AudioSource, in 3D space.
		/// </summary>
		/// <param name="entity">The entity to play the clip on. Must have an AudioSource component.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		AudioHandle play_spatial(Entity const entity, float const volume = -1.0f, bool const paused = false, unsigned int const bus = 0);

		/// <summary>
		/// Plays the AudioClip using the given clip on the Entity's AudioSource, in 3D space.
		/// </summary>
		/// <param name="clip">The AudioClip to play.</param>
		/// <param name="entity">The entity to play the clip on. Must have an AudioSource component.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		AudioHandle play_spatial(AudioClip& clip, Entity const entity, float const volume = -1.0f, bool const paused = false, unsigned int const bus = 0);

		/// <summary>
		/// Plays the AudioClip using the given name on the Entity's AudioSource, in 3D space.
		/// </summary>
		/// <param name="name">The name of the AudioClip to play.</param>
		/// <param name="entity">The entity to play the clip on. Must have an AudioSource component.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		AudioHandle play_spatial(String const& name, Entity const entity, float const volume = -1.0f, bool const paused = false, unsigned int const bus = 0);

		/// <summary>
		/// Plays the AudioClip using the given clip in the background.
		/// </summary>
		/// <param name="clip">The AudioClip to play.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		AudioHandle play_background(AudioClip& clip, float const volume = -1.0f, bool const paused = false, unsigned int const bus = 0);

		/// <summary>
		/// Plays the AudioClip using the given name in the background.
		/// </summary>
		/// <param name="name">The name of the AudioClip to play.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		AudioHandle play_background(String const& name, float const volume = -1.0f, bool const paused = false, unsigned int const bus = 0);

		/// <summary>
		/// Checks if the audio clip with the given handle is still playing.
		/// </summary>
		/// <param name="handle">The handle of the audio.</param>
		/// <returns>True if the audio is still playing, otherwise false.</returns>
		bool is_playing(AudioHandle const handle);

		/// <summary>
		/// Sets the pause of the audio clip with the given handle.
		/// </summary>
		/// <param name="handle">The handle to pause.</param>
		/// <param name="paused">The pause state of the audio.</param>
		void set_pause(AudioHandle const handle, bool const paused);

		/// <summary>
		/// Gets the pause state of the audio clip with the given handle.
		/// </summary>
		/// <param name="handle">The handle to get the pause state of.</param>
		/// <returns>True if the audio clip is paused.</returns>
		bool get_pause(AudioHandle const handle);

		/// <summary>
		/// Stops playing the audio clip with the given handle.
		/// </summary>
		/// <param name="handle">The handle of the audio clip to stop.</param>
		void stop(AudioHandle const handle);

		/// <summary>
		/// Stops all audio clips playing in this AudioEngine.
		/// </summary>
		void stop_all();
	private:
		// updates the given sound data using the given entity
		void update_sound_data(SoundData& data, Entity const entity);
	};
}