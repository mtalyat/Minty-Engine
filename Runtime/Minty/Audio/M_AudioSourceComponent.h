#pragma once
#include "Minty/Components/M_Component.h"

#include "Minty/Audio/M_Audio.h"

namespace Minty
{
	class AudioClip;

	/// <summary>
	/// Holds data for an Entity that can emit audio within the Scene.
	/// </summary>
	struct AudioSourceComponent
		: public Component
	{
		/// <summary>
		/// The clip to be played.
		/// </summary>
		Ref<AudioClip> clip;

		/// <summary>
		/// The handle to the sound being played, if any.
		/// </summary>
		AudioHandle handle = ERROR_AUDIO_HANDLE;

		/// <summary>
		/// The volume of this source.
		/// </summary>
		float volume = 1.0f;
		/// <summary>
		/// The attenuation of this source.
		/// </summary>
		float attenuation;
		/// <summary>
		/// The near distance of this source. All listeners within this distance will hear 100% volume.
		/// </summary>
		float nearDistance;
		/// <summary>
		/// The far distance of this source. All listeners outside of this distance will hear 0% volume.
		/// </summary>
		float farDistance;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}