#pragma once

#include "M_Base.h"
#include "M_Component.h"
#include "M_Mesh.h"

namespace minty
{
	struct MeshComponent
		: public Component
	{
		Mesh* mesh;
		ID materialId;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::string to_string(MeshComponent const& value);
	};
}