#pragma once
#include "Minty/Render/Mesh.h"

#include "Minty/Library/Vulkan.h"

namespace Minty
{
	class VulkanMesh
		: public Mesh
	{
	private:

	public:
		VulkanMesh(const MeshBuilder& builder);

		~VulkanMesh() = default;

	public:
		void on_bind() const override;
	};
}