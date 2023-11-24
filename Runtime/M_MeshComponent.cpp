#include "pch.h"
#include "M_MeshComponent.h"

#include "M_Console.h"
#include "M_SerializationData.h"
#include "M_Scene.h"
#include "M_Renderer.h"

using namespace minty;

void minty::MeshComponent::serialize(Writer& writer) const
{
	writer.write("materialId", materialId, ERROR_ID);
	writer.write("meshId", meshId);
	console::todo("MeshComponent::serialize");
}

void minty::MeshComponent::deserialize(Reader const& reader)
{
	SerializationData* data = static_cast<SerializationData*>(reader.get_data());

	materialId = reader.read_id("materialId");
	std::string meshType = reader.read_string("meshType");
	if (meshType.size())
	{
		// if there was a type given, then use that type
		MeshType type = from_string_mesh_type(meshType);

		meshId = data->renderer->get_or_create_mesh(type);
	}
	else
	{
		meshId = reader.read_id("meshId");
	}
	console::todo("MeshComponent::deserialize");
}

std::string minty::to_string(MeshComponent const& value)
{
	return std::format("MeshComponent()");
}
