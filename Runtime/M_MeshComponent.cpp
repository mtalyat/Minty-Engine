#include "pch.h"
#include "M_MeshComponent.h"

#include "M_Runtime.h"
#include "M_AssetEngine.h"
#include "M_Mesh.h"
#include "M_Console.h"
#include "M_SerializationData.h"
#include "M_Scene.h"
#include "M_RenderEngine.h"
#include "M_RenderSystem.h"
#include "M_File.h"
#include "M_SystemRegistry.h"
#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

void minty::MeshComponent::serialize(Writer& writer) const
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

void minty::MeshComponent::deserialize(Reader const& reader)
{
	SerializationData data = *static_cast<SerializationData const*>(reader.get_data());

	RenderEngine& renderer = data.scene->get_runtime().get_render_engine();
	AssetEngine& assets = data.scene->get_runtime().get_asset_engine();

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

String minty::to_string(MeshComponent const& value)
{
	return std::format("MeshComponent()");
}
