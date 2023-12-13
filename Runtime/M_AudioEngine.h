#pragma once

#include "M_Object.h"
#include "M_Register.h"

#include "M_AudioClip.h"
#include "M_Audio.h"
#include "M_EntityRegistry.h"
#include <unordered_map>

namespace minty
{
	class Scene;

	class AudioEngine
		: public Object
	{
	private:
		struct SoundSource
		{
			Entity entity;
			SoLoud::handle handle;
		};
		struct SoundData
		{
			Vector3 position;
			Vector3 velocity;
		};
	private:
		SoLoud::Soloud _engine;
		Register<AudioClip> _clips;
		std::unordered_map<ID, SoundSource> _playing;

		Scene const* _scene;
		EntityRegistry* _registry;
		Entity _listener;
		SoundData _listenerData;
	public:
		AudioEngine();

		~AudioEngine();

		void update();

		void play(ID const id, Entity const entity = NULL_ENTITY);

		void play(std::string const& name, Entity const entity = NULL_ENTITY);

		void play(Entity const entity);

		void play_background(ID const id);

		void play_background(std::string const& name);

		bool is_playing(ID const id) const;

		bool is_playing(std::string const& name) const;

		void stop(ID const id);

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