#pragma once

#include "M_Object.h"
#include "M_Register.h"

#include "M_AudioClip.h"
#include "M_Audio.h"
#include <unordered_map>

namespace minty
{
	class AudioEngine
		: public Object
	{
	private:
		SoLoud::Soloud _engine;
		Register<AudioClip> _clips;
		std::unordered_map<ID, SoLoud::handle> _playing;

	public:
		AudioEngine();

		~AudioEngine();

		void play(ID const id);

		void play(std::string const& name);

		void stop(ID const id);

		void stop_all();

		ID load_clip(std::string const& name, std::string const& path);

		ID load_clip(std::string const& path);

		AudioClip& at(ID const id);

		AudioClip const& at(ID const id) const;

		AudioClip& at(std::string const& name);

		AudioClip const& at(std::string const& name) const;

		void destroy_clip(ID const id);

		void destroy_all_clips();

	public:
		friend std::string to_string(AudioEngine const& value);
	};
}