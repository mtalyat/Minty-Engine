#include "pch.h"
#include "M_Mesh.h"

#include "M_Renderer.h"
#include "glm.hpp"

using namespace minty;
using namespace minty::rendering;

minty::Mesh::Mesh(Renderer& renderer)
	: rendering::RendererObject::RendererObject(renderer)
{}

minty::Mesh::~Mesh()
{
	dispose_vertices();
	dispose_indices();
}

uint32_t minty::Mesh::get_vertex_count() const
{
	return _vertexCount;
}

VkBuffer minty::Mesh::get_vertex_buffer() const
{
	return _vertexBuffer;
}

uint32_t minty::Mesh::get_index_count() const
{
	return _indexCount;
}

VkBuffer minty::Mesh::get_index_buffer() const
{
	return _indexBuffer;
}

void minty::Mesh::set_vertices(void const* const vertices, size_t const count, size_t const vertexSize)
{
	// if data already set, get rid of the old data
	dispose_vertices();

	_vertexCount = static_cast<uint32_t>(count);
	_vertexSize = static_cast<uint32_t>(vertexSize);

	// get buffer size
	VkDeviceSize bufferSize = static_cast<VkDeviceSize>(count * vertexSize);

	// use buffer to copy data into device memory
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	_renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	auto device = _renderer.get_device();

	// copy into staging buffer
	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices, static_cast<size_t>(bufferSize));
	vkUnmapMemory(device, stagingBufferMemory);

	// copy into device memory
	_renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexMemory);

	_renderer.copy_buffer(stagingBuffer, _vertexBuffer, bufferSize);

	// clean up
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void minty::Mesh::set_indices(void const* const indices, size_t const count, size_t const indexSize)
{
	// if data already set, get rid of the old data
	dispose_indices();

	_indexCount = static_cast<uint32_t>(count);
	_indexSize = static_cast<uint32_t>(indexSize);

	// get buffer size
	VkDeviceSize bufferSize = static_cast<VkDeviceSize>(count * indexSize);

	// use buffer to copy data into device memory
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	_renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	auto device = _renderer.get_device();

	// copy into staging buffer
	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices, (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	// copy into device memory
	_renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer, _indexMemory);

	_renderer.copy_buffer(stagingBuffer, _indexBuffer, bufferSize);

	// clean up
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void minty::Mesh::dispose_vertices()
{
	if (!_vertexCount)
	{
		return;
	}

	auto device = _renderer.get_device();

	vkDestroyBuffer(device, _vertexBuffer, nullptr);
	vkFreeMemory(device, _vertexMemory, nullptr);
	_vertexCount = 0;
	_vertexSize = 0;
}

void minty::Mesh::dispose_indices()
{
	if (!_indexCount)
	{
		return;
	}

	auto device = _renderer.get_device();

	vkDestroyBuffer(device, _indexBuffer, nullptr);
	vkFreeMemory(device, _indexMemory, nullptr);
	_indexCount = 0;
	_indexSize = 0;
}
