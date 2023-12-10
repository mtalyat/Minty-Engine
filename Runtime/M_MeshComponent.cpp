#include "pch.h"
#include "M_MeshComponent.h"

#include "M_Console.h"
#include "M_SerializationData.h"
#include "M_Scene.h"
#include "M_Renderer.h"
#include "M_File.h"

using namespace minty;

void minty::MeshComponent::serialize(Writer& writer) const
{
	console::todo("MeshComponent::serialize");
}

void minty::MeshComponent::deserialize(Reader const& reader)
{
	SerializationData* data = static_cast<SerializationData*>(reader.get_data());

	// load meta data
	materialId = data->renderer->find_material(reader.read_string("material"));

	// load mesh data
	MeshType meshType = from_string_mesh_type(reader.read_string("type"));

	switch (meshType)
	{
	case MeshType::Empty:
	case MeshType::Custom:
	{
		// use the asset path to get the mesh ( or use the name if no path given )
		std::string path = reader.read_string("path", reader.read_string("name"));

		// check if mesh with name is loaded
		std::string name = file::name(path);
		meshId = data->renderer->find_mesh(name);

		if (meshId == ERROR_ID)
		{
			// mesh is not loaded, so load it
			meshId = data->renderer->load_mesh(path);
		}
		break;
	}
	default:
		// if there was a type given, then use that type
		meshId = data->renderer->get_or_create_mesh(meshType);
		break;
	}
}

std::string minty::to_string(MeshComponent const& value)
{
	return std::format("MeshComponent()");
}
