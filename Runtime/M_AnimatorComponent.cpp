#include "pch.h"
#include "M_AnimatorComponent.h"

#include "M_SerializationData.h"
#include "M_AnimationSystem.h"

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

	MINTY_ASSERT(animationSystem != nullptr, "AnimatorComponent::serialize(): animationSystem cannot be null.");
	if (!animationSystem) return;

	writer.write("animator", animator);
	writer.write("animation", animationSystem->get_animation_name(animationId));
	writer.write("time", time);
	writer.write("index", index);
}

void minty::AnimatorComponent::deserialize(Reader const& reader)
{
	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());
	AnimationSystem const* animationSystem = data->scene->get_system_registry().find<AnimationSystem>();

	MINTY_ASSERT(animationSystem != nullptr, "AnimatorComponent::serialize(): animationSystem cannot be null.");
	if (!animationSystem) return;

	// make copy of animator for this animatorComponent, since we want our own values and stuff
	animator = animationSystem->get_animator(animationSystem->find_animator(reader.read_string("animator")));
	animationId = animator.get_fsm().get_current_value().get<ID>();
	//String animationName = reader.read_string("animation");
	//animationId = animationSystem->find_animation(animationName);
	//animator.get_fsm().set_current_state(animationName);

	// don't care about stuff (sorry not sorry)
}

String minty::to_string(AnimatorComponent const& value)
{
	return std::format("AnimatorComponent()");
}
