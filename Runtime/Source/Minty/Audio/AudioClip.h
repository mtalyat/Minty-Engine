#pragma once
#include "Minty/Asset/Asset.h"

namespace Minty
{
	struct AudioClipBuilder
	{
		UUID id;

		std::vector<Byte> data;

		float volume = 1.0f;

		bool looping = false;

		float loopPoint = 0.0f;

		bool singleInstance = false;
	};

	/// <summary>
	/// Represents an audio file that can be played.
	/// </summary>
	class AudioClip
		: public Asset
	{
		friend class AudioEngine;

	public:
		AudioClip(AudioClipBuilder const& builder)
			: Asset(builder.id)
		{}

		virtual ~AudioClip() = default;

	public:
		AssetType get_asset_type() const override { return AssetType::AudioClip; }

	public:
		virtual void* get_native() const = 0;

	public:
		static Owner<AudioClip> create(AudioClipBuilder const& builder = {});
	};
}