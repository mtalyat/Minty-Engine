#include "pch.h"
#include "M_AnimatorComponent.h"

#include "M_Runtime.h"
#include "M_AssetEngine.h"
#include "M_SerializationData.h"
#include "M_AnimationSystem.h"
#include "M_SystemRegistry.h"
#include "M_Scene.h"
#include "M_Reader.h"
#include "M_Writer.h"

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
