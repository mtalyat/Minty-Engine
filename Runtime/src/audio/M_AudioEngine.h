#pragma once
#include "runtime/M_Engine.h"

#include "types/M_Vector.h"
#include "audio/M_Audio.h"

namespace minty
{
	class Scene;
	class AudioClip;

	/// <summary>
	/// Handles audio for the game engine.
	/// </summary>
	class AudioEngine
		: public Engine
	{
	private:
		SoLoud::Soloud _engine;
	public:
		/// <summary>
		/// Creates a new AudioEngine.
		/// </summary>
		AudioEngine(Runtime& runtime);

		virtual ~AudioEngine();

		void set_listener_parameters(Vector3 const position, Vector3 const forward, Vector3 const up, Vector3 const velocity);

		void set_listener_position(Vector3 const position);

		void set_listener_forward(Vector3 const forward);

		void set_listener_up(Vector3 const up);

		void set_listener_velocity(Vector3 const velocity);

		bool is_valid_handle(AudioHandle const handle);

		void set_source_parameters(AudioHandle const handle, Vector3 const position, Vector3 const velocity);

		void set_source_position(AudioHandle const handle, Vector3 const position);

		void set_source_velocity(AudioHandle const handle, Vector3 const velocity);

		void set_source_attenuation(AudioHandle const handle, unsigned int const model, float const attenuation);

		void set_source_min_max_distance(AudioHandle const handle, float const nearDistance, float const farDistance);

		/// <summary>
		/// Updates all changes to the Audio Engine.
		/// </summary>
		void apply();

		AudioHandle play(AudioClip& clip, float const volume = -1.0f, float const pan = 0.0f, bool const paused = false, unsigned int const bus = 0);

		AudioHandle play_spatial(AudioClip& clip, Vector3 const position, Vector3 const velocity = Vector3(), float const volume = -1.0f, bool const paused = false, unsigned int const bus = 0);

		AudioHandle play_background(AudioClip& clip, float const volume = -1.0f, float const paused = false, unsigned int const bus = 0);

		void set_pause(AudioHandle const handle, bool const paused);

		void set_pause_all(bool const paused);

		bool get_pause(AudioHandle const handle);

		void stop(AudioHandle const handle);

		void stop_all();

	public:
		friend String to_string(AudioEngine const& value);
	};
}