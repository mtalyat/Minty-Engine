#include "pch.h"
#include "M_AudioSourceComponent.h"

#include "M_SerializationData.h"

using namespace minty;

void minty::AudioSource::serialize(Writer& writer) const
{
	SerializationData* data = static_cast<SerializationData*>(writer.get_data());

	AudioEngine* audio = data->scene->get_engine()->get_audio_engine();

	writer.write("volume", volume, 1.0f);
	writer.write("clip", audio->get_name(clipId));
	writer.write("attenuation", attenuation);
	writer.write("near", nearDistance);
	writer.write("far", farDistance);
}

void minty::AudioSource::deserialize(Reader const& reader)
{
	SerializationData* data = static_cast<SerializationData*>(reader.get_data());

	AudioEngine* audio = data->scene->get_engine()->get_audio_engine();

	volume = reader.read_float("volume", 1.0f);
	clipId = audio->get_id(reader.read_string("clip"));
	attenuation = reader.read_float("attenuation", 1.0f);
	nearDistance = reader.read_float("near", 1.0f);
	farDistance = reader.read_float("far", 100.0f);
}
