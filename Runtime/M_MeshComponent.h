#pragma once

#include "M_Base.h"
#include "M_Component.h"
#include "M_Mesh.h"

namespace minty
{
	/// <summary>
	/// Holds data for a Mesh to be drawn within the Scene.
	/// </summary>
	struct MeshComponent
		: public Component
	{
		/// <summary>
		/// The ID of the Mesh to draw.
		/// </summary>
		ID meshId;

		/// <summary>
		/// The ID of the Material to use to draw the Mesh.
		/// </summary>
		ID materialId;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(MeshComponent const& value);
	};
}