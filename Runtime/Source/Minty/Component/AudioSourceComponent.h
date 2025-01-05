#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Audio/AudioClip.h"

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
		Handle handle = INVALID_HANDLE;

		/// <summary>
		/// The volume of this source.
		/// </summary>
		Float volume = 1.0f;
		/// <summary>
		/// The attenuation of this source.
		/// </summary>
		Float attenuation = 1.0f;
		/// <summary>
		/// The near distance of this source. All listeners within this distance will hear 100% volume.
		/// </summary>
		Float nearDistance = 1.0f;
		/// <summary>
		/// The far distance of this source. All listeners outside of this distance will hear 0% volume.
		/// </summary>
		Float farDistance = 10000.0f;

		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}