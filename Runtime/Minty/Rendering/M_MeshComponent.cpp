#include "pch.h"
#include "Minty/Rendering/M_MeshComponent.h"

#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Rendering/M_Mesh.h"
#include "Minty/Tools/M_Console.h"
#include "Minty/Serialization/M_SerializationData.h"
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Rendering/M_RenderSystem.h"
#include "Minty/Files/M_File.h"
#include "Minty/Systems/M_SystemRegistry.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

using namespace Minty;

void Minty::MeshComponent::serialize(Writer& writer) const
{
	writer.write("type", to_string(type));
	switch (type)
	{
		case MeshType::Custom:
			writer.write("mesh", mesh ? mesh->get_id() : UUID(INVALID_UUID));
			break;
		default:
			break;
	}
	writer.write("material", material ? material->get_id() : UUID(INVALID_UUID));
}

void Minty::MeshComponent::deserialize(Reader const& reader)
{
	SerializationData data = *static_cast<SerializationData const*>(reader.get_data());

	RenderEngine& renderer = RenderEngine::instance();
	AssetEngine& assets = AssetEngine::instance();

	// load meta data
	UUID id(INVALID_UUID);
	if (reader.try_read_uuid("material", id))
	{
		material = assets.get<Material>(id);
	}

	// load mesh data
	String meshTypeName;
	if (reader.try_read_string("type", meshTypeName))
	{
		type = from_string_mesh_type(meshTypeName);

		id = renderer.get_or_create_mesh(type);

		if (id.valid())
		{
			// had a built in type
			mesh = assets.get<Mesh>(id);
			return;
		}
	}

	// custom type or empty
	if (reader.try_read_uuid("mesh", id))
	{
		mesh = assets.get<Mesh>(id);
	}
}