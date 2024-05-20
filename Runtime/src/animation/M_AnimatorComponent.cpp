#include "pch.h"
#include "animation/M_AnimatorComponent.h"

#include "runtime/M_Runtime.h"
#include "assets/M_AssetEngine.h"
#include "serialization/M_SerializationData.h"
#include "animation/M_AnimationSystem.h"
#include "systems/M_SystemRegistry.h"
#include "scenes/M_Scene.h"
#include "serialization/M_Reader.h"
#include "serialization/M_Writer.h"

using namespace minty;

void minty::AnimatorComponent::reset()
{
	index = 0;
	time = 0.0f;
}

void minty::AnimatorComponent::serialize(Writer& writer) const
{
	writer.write("animator", animator.get_id());
}

void minty::AnimatorComponent::deserialize(Reader const& reader)
{
	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());
	AssetEngine& assets = data->scene->get_runtime().get_asset_engine();

	UUID id(INVALID_UUID);
	if (reader.try_read_uuid("animator", id))
	{
		// make a copy of the animator for this component
		animator = assets.at<Animator>(id);
		animation = assets.get<Animation>(animator.get_fsm().get_current_value().get<UUID>());
	}
}

String minty::to_string(AnimatorComponent const& value)
{
	return std::format("AnimatorComponent()");
}
