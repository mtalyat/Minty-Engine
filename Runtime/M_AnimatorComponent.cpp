#include "pch.h"
#include "M_AnimatorComponent.h"

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
	SerializationData const* data = static_cast<SerializationData const*>(writer.get_data());
	AnimationSystem const* animationSystem = data->scene->get_system_registry().find<AnimationSystem>();

	MINTY_ASSERT(animationSystem != nullptr);

	writer.write("animator", animator);
	writer.write("animation", animationSystem->get_animation_name(animationId));
	writer.write("time", time);
	writer.write("index", index);
}

void minty::AnimatorComponent::deserialize(Reader const& reader)
{
	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());
	AnimationSystem const* animationSystem = data->scene->get_system_registry().find<AnimationSystem>();

	MINTY_ASSERT(animationSystem != nullptr);

	// make copy of animator for this animatorComponent, since we want our own values and stuff
	String animatorName;
	if (reader.try_read_string("animator", animatorName))
	{
		animator = animationSystem->get_animator(animationSystem->find_animator(animatorName));
		animationId = animator.get_fsm().get_current_value().get<ID>();
	}
}

String minty::to_string(AnimatorComponent const& value)
{
	return std::format("AnimatorComponent()");
}
