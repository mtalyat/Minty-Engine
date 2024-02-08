#include "pch.h"
#include "M_SpriteComponent.h"

#include "M_SerializationData.h"
#include "M_RenderSystem.h"

using namespace minty;

void minty::SpriteComponent::serialize(Writer& writer) const
{
	SerializationData const* data = static_cast<SerializationData const*>(writer.get_data());
	RenderSystem const* renderSystem = data->scene->get_system_registry().find<RenderSystem>();

	writer.write("sprite", renderSystem->get_sprite_name(spriteId));
	writer.write("size", size, Vector2(1.0f, 1.0f));
	writer.write("order", order, 0);
}

void minty::SpriteComponent::deserialize(Reader const& reader)
{
	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());
	RenderSystem const* renderSystem = data->scene->get_system_registry().find<RenderSystem>();

	String spriteName;
	if (reader.try_read_string("sprite", spriteName))spriteId = renderSystem->find_sprite(spriteName);
	reader.try_read_object("size", size);
	reader.try_read_int("order", order);
}

String minty::to_string(SpriteComponent const& value)
{
	return std::format("SpriteComponent(id = {}, layer = {})", value.spriteId, value.order);
}
