#pragma once

#include "M_Component.h"

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
		ID meshId = ERROR_ID;

		/// <summary>
		/// The ID of the Material to use to draw the Mesh.
		/// </summary>
		ID materialId = ERROR_ID;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(MeshComponent const& value);
	};
}