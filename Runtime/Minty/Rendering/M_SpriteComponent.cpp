#include "pch.h"
#include "Minty/Rendering/M_SpriteComponent.h"

#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Serialization/M_SerializationData.h"
#include "Minty/Rendering/M_RenderSystem.h"
#include "Minty/Systems/M_SystemRegistry.h"
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

using namespace Minty;

void Minty::SpriteComponent::serialize(Writer& writer) const
{
	SerializationData const* data = static_cast<SerializationData const*>(writer.get_data());
	RenderSystem const* renderSystem = data->scene->get_system_registry().find<RenderSystem>();

	writer.write("sprite", sprite ? sprite->get_id() : UUID(INVALID_UUID));
	writer.write("size", size);
	writer.write("layer", layer);
	writer.write("order", order);
	writer.write("color", color);
}

void Minty::SpriteComponent::deserialize(Reader const& reader)
{
	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());
	AssetEngine& assets = AssetEngine::instance();

	UUID spriteId(INVALID_UUID);
	if (reader.try_read_uuid("sprite", spriteId))
	{
		sprite = assets.get<Sprite>(spriteId);
	}
	reader.try_read_object("size", size);
	reader.try_read_int("layer", layer);
	reader.try_read_int("order", order);
	reader.try_read_color("color", color);
}