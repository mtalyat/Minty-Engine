#include "pch.h"
#include "M_AudioSourceComponent.h"

#include "M_AudioSystem.h"
#include "M_SystemRegistry.h"

#include "M_SerializationData.h"
#include "M_Scene.h"
#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

void minty::AudioSourceComponent::serialize(Writer& writer) const
{
	SerializationData data = *static_cast<SerializationData const*>(writer.get_data());

	AudioSystem const* audioSystem = data.scene->get_system_registry().find<AudioSystem>();

	writer.write("volume", volume);
	writer.write("clip", audioSystem->get_name(clipId));
	writer.write("attenuation", attenuation);
	writer.write("near", nearDistance);
	writer.write("far", farDistance);
}

void minty::AudioSourceComponent::deserialize(Reader const& reader)
{
	SerializationData data = *static_cast<SerializationData const*>(reader.get_data());

	AudioSystem const* audioSystem = data.scene->get_system_registry().find<AudioSystem>();

	volume = reader.read_float("volume", 1.0f);
	clipId = audioSystem->get_id(reader.read_string("clip"));
	attenuation = reader.read_float("attenuation", 1.0f);
	nearDistance = reader.read_float("near", 1.0f);
	farDistance = reader.read_float("far", 100.0f);
}

String minty::to_string(AudioSourceComponent const& value)
{
	return std::format("AudioSourceComponent(clipId = {})", value.clipId);
}
