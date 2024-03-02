#pragma once

#include "M_Component.h"

namespace minty
{
	class Mesh;
	class Material;

	/// <summary>
	/// Holds data for a Mesh to be drawn within the Scene.
	/// </summary>
	struct MeshComponent
		: public Component
	{
		/// <summary>
		/// The Mesh to draw.
		/// </summary>
		Mesh* mesh = nullptr;

		/// <summary>
		/// The ID of the Material to use to draw the Mesh.
		/// </summary>
		Material* material = nullptr;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(MeshComponent const& value);
	};
}