#pragma once

#include "M_Object.h"
#include "M_Register.h"

#include "M_AudioClip.h"
#include "M_Audio.h"
#include "M_EntityRegistry.h"
#include <unordered_map>
#include <climits>

namespace minty
{
	class Scene;

	typedef SoLoud::handle AudioHandle;
	constexpr const AudioHandle ERROR_AUDIO_HANDLE = UINT_MAX;

	class AudioEngine
		: public Object
	{
	private:
		struct SoundData
		{
			Vector3 position;
			Vector3 velocity;
		};
	private:
		SoLoud::Soloud _engine;
		Register<AudioClip> _clips;
		std::unordered_map<AudioHandle, Entity> _playing;

		Scene const* _scene;
		EntityRegistry* _registry;
		Entity _listener;
		SoundData _listenerData;
	public:
		AudioEngine();

		~AudioEngine();

		void update();

		AudioHandle play(ID const id, float const volume = -1.0f, float const pan = 0.0f, bool const paused = false);

		AudioHandle play(std::string const& name, float const volume = -1.0f, float const pan = 0.0f, bool const paused = false);

		AudioHandle play_spatial(Entity const entity, float const volume = -1.0f, bool const paused = false);

		AudioHandle play_spatial(ID const id, Entity const entity, float const volume = -1.0f, bool const paused = false);

		AudioHandle play_spatial(std::string const& name, Entity const entity, float const volume = -1.0f, bool const paused = false);

		AudioHandle play_background(ID const id, float const volume = -1.0f, bool const paused = false);

		AudioHandle play_background(std::string const& name, float const volume = -1.0f, bool const paused = false);

		bool is_playing(AudioHandle const handle);

		void set_pause(AudioHandle const handle, bool const paused);

		bool get_pause(AudioHandle const handle);

		void stop(AudioHandle const handle);

		void stop_all();

		ID load_clip(std::string const& name, std::string const& path);

		ID load_clip(std::string const& path);

		AudioClip& at(ID const id);

		AudioClip const& at(ID const id) const;

		AudioClip& at(std::string const& name);

		AudioClip const& at(std::string const& name) const;

		std::string const& get_name(ID const id) const;

		ID get_id(std::string const& name) const;

		void destroy_clip(ID const id);

		void destroy_all_clips();

		void set_listener(Entity const entity);

		void set_scene(Scene const* const scene, Entity const camera = NULL_ENTITY);

	private:
		void update_sound_data(SoundData& data, Entity const entity);

	public:
		friend std::string to_string(AudioEngine const& value);
	};
}