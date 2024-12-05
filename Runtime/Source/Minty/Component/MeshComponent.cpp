#include "pch.h"
#include "MeshComponent.h"

using namespace Minty;

void Minty::MeshComponent::serialize(Writer& writer) const
{
	writer.write("type", type);
	if (type == MeshType::Custom)
	{
		if (mesh == nullptr)
		{
			writer.write("mesh", UUID());
		}
		else
		{
			writer.write("mesh", mesh->id());
		}
	}
	if (material == nullptr)
	{
		writer.write("material", UUID());
	}
	else
	{
		writer.write("material", material->id());
	}
}

void Minty::MeshComponent::deserialize(Reader& reader)
{
	UUID temp;

	if (reader.read("type", type))
	{
		mesh = Renderer::get_or_create_mesh(type);

		// if null, must be custom
		if (mesh == nullptr)
		{
			// type == Custom, load the custom Mesh
			if (reader.read("mesh", temp))
			{
				mesh = AssetManager::get<Mesh>(temp);
			}
		}
	}
	else
	{
		// no type: assume custom Mesh
		if (reader.read("mesh", temp))
		{
			mesh = AssetManager::get<Mesh>(temp);
		}
	}

	if (reader.read("material", temp))
	{
		material = AssetManager::get<Material>(temp);
	}
}
