#pragma once

#include "M_System.h"
#include "M_Register.h"
#include "M_Audio.h"
#include "M_AudioClip.h"
#include "M_Vector.h"
#include "M_EntityRegistry.h"
#include <unordered_map>

namespace minty
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
		Register<AudioClip> _clips;
		std::unordered_map<AudioHandle, Entity> _playing;

		Entity _listener;
		SoundData _listenerData;
	public:
		AudioSystem(Engine& engine, ID const sceneId);

		void update() override;

		void set_listener(Entity const entity);

		/// <summary>
		/// Plays the clip with the given ID.
		/// </summary>
		/// <param name="id">The ID of the AudioClip to play.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="pan">The pan of the AudioClip. -1.0 is left, 0.0 is centered, and 1.0 is right.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		AudioHandle play(ID const id, float const volume = -1.0f, float const pan = 0.0f, bool const paused = false, unsigned int const bus = 0);

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
		/// Plays the AudioClip using the given ID on the Entity's AudioSource, in 3D space.
		/// </summary>
		/// <param name="id">The ID of the AudioClip to play.</param>
		/// <param name="entity">The entity to play the clip on. Must have an AudioSource component.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		AudioHandle play_spatial(ID const id, Entity const entity, float const volume = -1.0f, bool const paused = false, unsigned int const bus = 0);

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
		/// Plays the AudioClip using the given ID in the background.
		/// </summary>
		/// <param name="id">The ID of the AudioClip to play.</param>
		/// <param name="volume">The volume at which to play the AudioClip. If -1.0, the AudioClip volume is used.</param>
		/// <param name="paused">The pause state of the AudioClip. If true, the clip starts paused.</param>
		/// <returns>A handle to the instance of the AudioClip being played.</returns>
		AudioHandle play_background(ID const id, float const volume = -1.0f, bool const paused = false, unsigned int const bus = 0);

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

		/// <summary>
		/// Loads the AudioClip into this AudioEngine.
		/// </summary>
		/// <param name="name">The name of the AudioClip.</param>
		/// <param name="path">The path to the AudioClip file.</param>
		/// <returns>The ID of the AudioClip.</returns>
		ID load_clip(String const& name, Path const& path);

		/// <summary>
		/// Loads the AudioClip into this AudioEngine. The name is the name of the given file at the path.
		/// </summary>
		/// <param name="path">The path to the AudioClip file.</param>
		/// <returns>The ID of the AudioClip.</returns>
		ID load_clip(Path const& path);

		/// <summary>
		/// Checks if an AudioClip with the given ID has been loaded.
		/// </summary>
		/// <param name="id">The ID to check.</param>
		/// <returns>True if there is an AudioClip with the given ID loaded.</returns>
		bool contains(ID const id) const;

		/// <summary>
		/// Checks if an AudioClip with the given name has been loaded.
		/// </summary>
		/// <param name="name">The name to check.</param>
		/// <returns>True if there is an AudioClip with the given name loaded.</returns>
		bool contains(String const& name) const;

		/// <summary>
		/// Gets the AudioClip with the given ID.
		/// </summary>
		/// <param name="id">The ID of the AudioClip.</param>
		/// <returns>A reference to the AudioClip.</returns>
		AudioClip& at(ID const id);

		/// <summary>
		/// Gets the AudioClip with the given ID.
		/// </summary>
		/// <param name="id">The ID of the AudioClip.</param>
		/// <returns>A reference to the AudioClip.</returns>
		AudioClip const& at(ID const id) const;

		/// <summary>
		/// Gets the AudioClip with the given name.
		/// </summary>
		/// <param name="name">The name of the AudioClip.</param>
		/// <returns>A reference to the AudioClip.</returns>
		AudioClip& at(String const& name);

		/// <summary>
		/// Gets the AudioClip with the given name.
		/// </summary>
		/// <param name="name">The name of the AudioClip.</param>
		/// <returns>A reference to the AudioClip.</returns>
		AudioClip const& at(String const& name) const;

		/// <summary>
		/// Gets the name of the loaded AudioClip with the given ID.
		/// </summary>
		/// <param name="id">The ID of the AudioClip.</param>
		/// <returns>The name of the AudioClip.</returns>
		String get_name(ID const id) const;

		/// <summary>
		/// Gets the ID of the loaded AudioClip with the given name.
		/// </summary>
		/// <param name="name">The name of the AudioClip.</param>
		/// <returns>The ID of the AudioClip.</returns>
		ID get_id(String const& name) const;

		/// <summary>
		/// Unloads the AudioClip with the given ID.
		/// </summary>
		/// <param name="id">The ID of the AudioClip to unload.</param>
		void destroy_clip(ID const id);

		/// <summary>
		/// Unloads all loaded AudioClips.
		/// </summary>
		void destroy_all_clips();

	private:
		AudioEngine& get_audio_engine() const;

		// updates the given sound data using the given entity
		void update_sound_data(SoundData& data, Entity const entity);
	};
}