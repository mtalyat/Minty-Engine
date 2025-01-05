#include "pch.h"
#include "AudioSourceComponent.h"

#include "Minty/Entity/EntitySerializationData.h"

using namespace Minty;

void Minty::AudioSourceComponent::serialize(Writer& writer) const
{
	EntitySerializationData data = *static_cast<EntitySerializationData const*>(writer.get_data());

	
}

void Minty::AudioSourceComponent::deserialize(Reader& reader)
{
	EntitySerializationData data = *static_cast<EntitySerializationData const*>(reader.get_data());
}
