#pragma once

#include "M_Base.h"

#include "M_Audio.h"

namespace minty
{
	class AudioClip
	{
		friend class AudioEngine;
	private:
		SoLoud::Wav _clip;

	public:
		AudioClip();

		void load(std::string const& path);

		void set_volume(float const value);

		void set_looping(bool const looping);

		void set_loop_point(float const point);

		void set_single_instance(bool const value);
	};
}