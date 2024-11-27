#include "pch.h"
#include "VulkanMesh.h"

#include "Platform/Vulkan/VulkanRenderer.h"

using namespace Minty;

Minty::VulkanMesh::VulkanMesh(MeshBuilder const& builder)
	: Minty::Mesh::Mesh(builder)
{}

void Minty::VulkanMesh::on_bind() const
{
	// bind the buffers
	VulkanRenderer::bind_vertex_buffer(VulkanRenderer::get_command_buffer(), static_cast<VkBuffer>(m_vertexBuffer->get_native()));
	VulkanRenderer::bind_index_buffer(VulkanRenderer::get_command_buffer(), static_cast<VkBuffer>(m_indexBuffer->get_native()));
}
