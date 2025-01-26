#pragma once
#include "Minty/Component/ScriptObjectComponent.h"

#include "Minty/Asset/AssetManager.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Render/Material.h"
#include "Minty/Render/Mesh.h"
#include "Minty/Render/Renderer.h"

namespace Minty
{
	/// <summary>
	/// Holds the Mesh for an Entity.
	/// </summary>
	struct MeshComponent
		: public ScriptObjectComponent
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

		void serialize(Writer& writer) const override;

		void deserialize(Reader& reader) override;
	};
}
