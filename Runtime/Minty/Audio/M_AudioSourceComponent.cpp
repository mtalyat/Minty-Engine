#include "pch.h"
#include "Minty/Audio/M_AudioSourceComponent.h"

#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Audio/M_AudioSystem.h"
#include "Minty/Systems/M_SystemRegistry.h"

#include "Minty/Serialization/M_SerializationData.h"
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

using namespace Minty;

void Minty::AudioSourceComponent::serialize(Writer& writer) const
{
	SerializationData data = *static_cast<SerializationData const*>(writer.get_data());

	AudioSystem const* audioSystem = data.scene->get_system_registry().find<AudioSystem>();

	writer.write("volume", volume);
	writer.write("clip", clip ? clip->get_id() : UUID(INVALID_UUID));
	writer.write("attenuation", attenuation);
	writer.write("near", nearDistance);
	writer.write("far", farDistance);
}

void Minty::AudioSourceComponent::deserialize(Reader const& reader)
{
	SerializationData data = *static_cast<SerializationData const*>(reader.get_data());
	AssetEngine& assets = AssetEngine::instance();
	
	volume = reader.read_float("volume", 1.0f);
	clip = assets.get<AudioClip>(reader.read_uuid("clip"));
	attenuation = reader.read_float("attenuation", 1.0f);
	nearDistance = reader.read_float("near", 1.0f);
	farDistance = reader.read_float("far", 100.0f);
}