#include "pch.h"
#include "M_SpriteComponent.h"

#include "M_SerializationData.h"

using namespace minty;

void minty::SpriteComponent::serialize(Writer& writer) const
{
	SerializationData const* data = static_cast<SerializationData const*>(writer.get_data());
	RenderEngine const& renderEngine = data->scene->get_engine()->get_render_engine();

	writer.write("sprite", renderEngine.get_sprite_name(spriteId));
	writer.write_object("size", size, Vector2());
	writer.write("order", order, 0);
}

void minty::SpriteComponent::deserialize(Reader const& reader)
{
	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());
	RenderEngine const& renderEngine = data->scene->get_engine()->get_render_engine();

	spriteId = renderEngine.find_sprite(reader.read_string("sprite"));
	reader.read_object("size", size, Vector2());
	order = reader.read_int("order");
}

String minty::to_string(SpriteComponent const& value)
{
	return std::format("SpriteComponent(id = {}, layer = {})", value.spriteId, value.order);
}
