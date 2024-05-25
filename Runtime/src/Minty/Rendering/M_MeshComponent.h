#pragma once
#include "Minty/Components/M_Component.h"

#include "Minty/Rendering/M_Mesh.h"

namespace Minty
{
	class Material;

	/// <summary>
	/// Holds data for a Mesh to be drawn within the Scene.
	/// </summary>
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

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}