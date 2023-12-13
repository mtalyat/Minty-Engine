#pragma once
#include "M_Component.h"

#include "M_Audio.h"

namespace minty
{
	struct AudioSource
		: public Component
	{
		ID clipId;

		float attenuation;
		float nearDistance;
		float farDistance;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}