#include "pch.h"
#include "M_SpriteComponent.h"

#include "M_Runtime.h"
#include "M_AssetEngine.h"
#include "M_SerializationData.h"
#include "M_RenderSystem.h"
#include "M_SystemRegistry.h"
#include "M_Scene.h"
#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

void minty::SpriteComponent::serialize(Writer& writer) const
{
	SerializationData const* data = static_cast<SerializationData const*>(writer.get_data());
	RenderSystem const* renderSystem = data->scene->get_system_registry().find<RenderSystem>();

	writer.write("sprite", sprite ? sprite->get_id() : UUID(INVALID_UUID));
	writer.write("size", size);
	writer.write("order", order);
}

void minty::SpriteComponent::deserialize(Reader const& reader)
{
	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());
	AssetEngine& assets = data->scene->get_runtime().get_asset_engine();

	UUID spriteId(INVALID_UUID);
	if (reader.try_read_uuid("sprite", spriteId))
	{
		sprite = assets.get<Sprite>(spriteId);
	}
	reader.try_read_object("size", size);
	reader.try_read_int("order", order);
}

String minty::to_string(SpriteComponent const& value)
{
	return std::format("SpriteComponent()");
}
