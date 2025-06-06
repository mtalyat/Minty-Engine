#pragma once
#include "Minty/Assets/M_Asset.h"

#include "Minty/Audio/M_Audio.h"

namespace Minty
{
	struct AudioClipBuilder
	{
		UUID id;

		Path path;

		float volume = 1.0f;

		bool looping = false;

		float loopPoint = 0.0f;

		bool singleInstance = false;
	};

	/// <summary>
	/// Holds an audio clip.
	/// </summary>
	class AudioClip
		: public Asset
	{
		friend class AudioEngine;
	private:
		SoLoud::Wav _clip;

	public:
		/// <summary>
		/// Creates an empty AudioClip.
		/// </summary>
		AudioClip();

		AudioClip(AudioClipBuilder const& builder);

	private:
		/// <summary>
		/// Loads this AudioClip with the data from the sound at the given path.
		/// </summary>
		/// <param name="path">The path to the sound file.</param>
		void load(Path const& path);

	public:
		/// <summary>
		/// Sets the base volume of this AudioClip.
		/// </summary>
		/// <param name="value">A volume within the range of [0.0, 1.0].</param>
		void set_volume(float const value);

		/// <summary>
		/// Sets the looping state of this AudioClip.
		/// </summary>
		/// <param name="looping">True for looping, false for non-looping.</param>
		void set_looping(bool const looping);

		/// <summary>
		/// Sets the looping point of this AudioClip.
		/// </summary>
		/// <param name="point">The time to start the loop.</param>
		void set_loop_point(float const point);

		/// <summary>
		/// Sets the single instance state of this AudioClip. 
		/// </summary>
		/// <param name="value">If true, only one copy of this AudioClip can be played at once. If false, any number of copies can be played at once.</param>
		void set_single_instance(bool const value);
	};
}