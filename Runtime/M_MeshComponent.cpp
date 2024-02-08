#include "pch.h"
#include "M_MeshComponent.h"

#include "M_Console.h"
#include "M_SerializationData.h"
#include "M_Scene.h"
#include "M_RenderEngine.h"
#include "M_RenderSystem.h"
#include "M_File.h"

using namespace minty;

void minty::MeshComponent::serialize(Writer& writer) const
{
	console::todo("MeshComponent::serialize");
}

void minty::MeshComponent::deserialize(Reader const& reader)
{
	SerializationData data = *static_cast<SerializationData const*>(reader.get_data());

	RenderSystem* renderSystem = data.scene->get_system_registry().find<RenderSystem>();

	MINTY_ASSERT(renderSystem != nullptr, "MeshComponent::deserialize(): renderSystem cannot be null.");
	if (!renderSystem) return;

	// load meta data
	String materialName;
	if (reader.try_read_string("material", materialName))
	{
		materialId = renderSystem->find_material(materialName);
	}

	// load mesh data
	String meshTypeName;
	if (reader.try_read_string("type", meshTypeName))
	{
		MeshType meshType = from_string_mesh_type(meshTypeName);

		switch (meshType)
		{
		case MeshType::Empty:
		case MeshType::Custom:
		{
			// use the asset path to get the mesh ( or use the name if no path given )
			String path = reader.read_string("path", reader.read_string("name"));

			// check if mesh with name is loaded
			String name = Path(path).stem().string();
			meshId = renderSystem->find_mesh(name);

			if (meshId == ERROR_ID)
			{
				// mesh is not loaded, so load it using the path
				meshId = renderSystem->load_mesh(path);
			}

			if (meshId == ERROR_ID)
			{
				console::error(std::format("Could not load_animation mesh with path \"{}\" and name \"{}\".", path, name));
			}
			break;
		}
		default:
			// if there was a type given, then use that type
			meshId = renderSystem->get_or_create_mesh(meshType);
			break;
		}
	}
}

String minty::to_string(MeshComponent const& value)
{
	return std::format("MeshComponent(meshId = {}, materialId = {})", value.meshId, value.materialId);
}
