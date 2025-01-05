#include "pch.h"
#include "AnimatorComponent.h"

#include "Minty/Asset/AssetManager.h"
#include "Minty/Entity/EntitySerializationData.h"

using namespace Minty;

void Minty::AnimatorComponent::reset()
{
	index = 0;
	time = 0.0f;
}

void Minty::AnimatorComponent::serialize(Writer& writer) const
{
	writer.write("animator", animator->id());
}

void Minty::AnimatorComponent::deserialize(Reader& reader)
{
	EntitySerializationData const* data = static_cast<EntitySerializationData const*>(reader.get_data());

	UUID id{};
	if (reader.read("animator", id))
	{
		// make a copy of the animator for this component
		animator = AssetManager::clone<Animator>(id);

		MINTY_ASSERT_MESSAGE(animator != nullptr, "Null animator.");

		// reference the animation, if there is one
		if (animator->get_fsm().has_current_state())
		{
			animation = AssetManager::get<Animation>(animator->get_fsm().get_current_value().get<UUID>());
		}
	}
}