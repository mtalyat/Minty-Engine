#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Asset/AssetManager.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Render/Material.h"
#include "Minty/Render/Mesh.h"
#include "Minty/Render/Renderer.h"

namespace Minty
{
	struct MeshComponent
		: public Component
	{
		/// <summary>
		/// The type of mesh.
		/// </summary>
		MeshType type;

		/// <summary>
		/// The Mesh to draw.
		/// </summary>
		Ref<Mesh> mesh = nullptr;

		/// <summary>
		/// The ID of the Material to use to draw the Mesh.
		/// </summary>
		Ref<Material> material = nullptr;

		void serialize(Writer& writer) const override
		{
			writer.write("type", type);
			if (mesh.get())
			{
				writer.write("mesh", mesh->id());
			}
			if (material.get())
			{
				writer.write("material", material->id());
			}
		}

		void deserialize(Reader& reader) override
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
	};
}
