#include "pch.h"
#include "Minty/Animation/M_AnimatorComponent.h"

#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Serialization/M_SerializationData.h"
#include "Minty/Animation/M_AnimationSystem.h"
#include "Minty/Systems/M_SystemRegistry.h"
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

using namespace Minty;

void Minty::AnimatorComponent::reset()
{
	index = 0;
	time = 0.0f;
}

void Minty::AnimatorComponent::serialize(Writer& writer) const
{
	writer.write("animator", animator.get_id());
}

void Minty::AnimatorComponent::deserialize(Reader const& reader)
{
	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());
	AssetEngine& assets = AssetEngine::instance();

	UUID id(INVALID_UUID);
	if (reader.try_read_uuid("animator", id))
	{
		// make a copy of the animator for this component
		animator = *assets.at<Animator>(id);
		animation = assets.get<Animation>(animator.get_fsm().get_current_value().get<UUID>());
	}
}

String Minty::to_string(AnimatorComponent const& value)
{
	return std::format("AnimatorComponent()");
}
