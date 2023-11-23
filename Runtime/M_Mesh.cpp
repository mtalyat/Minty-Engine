#include "pch.h"
#include "M_Mesh.h"

#include "M_Renderer.h"
#include "glm.hpp"

using namespace minty;
using namespace minty::rendering;

minty::Mesh::Mesh(Renderer& renderer)
	: rendering::RendererObject::RendererObject(renderer)
	, _vertexCount()
	, _vertexSize()
	, _vertexBufferId(ERROR_ID)
	, _indexCount()
	, _indexSize()
	, _indexBufferId(ERROR_ID)
	, _indexType()
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

ID minty::Mesh::get_vertex_buffer_id() const
{
	return _vertexBufferId;
}

uint32_t minty::Mesh::get_index_count() const
{
	return _indexCount;
}

ID minty::Mesh::get_index_buffer_id() const
{
	return _indexBufferId;
}

VkIndexType minty::Mesh::get_index_type() const
{
	return _indexType;
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
	ID stagingBufferId = _renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	auto device = _renderer.get_device();

	// map data so it can be set
	void* mappedData = _renderer.map_buffer(stagingBufferId);

	// copy into staging buffer
	memcpy(mappedData, vertices, static_cast<size_t>(bufferSize));

	// unmap
	_renderer.unmap_buffer(stagingBufferId);

	//void* data;
	//vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	//memcpy(data, vertices, static_cast<size_t>(bufferSize));
	//vkUnmapMemory(device, stagingBufferMemory);

	// copy into device memory
	_vertexBufferId = _renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	_renderer.copy_buffer(stagingBufferId, _vertexBufferId, bufferSize);

	// clean up
	_renderer.destroy_buffer(stagingBufferId);
}

void minty::Mesh::set_indices(void const* const indices, size_t const count, size_t const indexSize, VkIndexType const type)
{
	// if data already set, get rid of the old data
	dispose_indices();

	_indexCount = static_cast<uint32_t>(count);
	_indexSize = static_cast<uint32_t>(indexSize);
	_indexType = type;

	// get buffer size
	VkDeviceSize bufferSize = static_cast<VkDeviceSize>(count * indexSize);

	// use buffer to copy data into device memory
	//VkBuffer stagingBuffer;
	//VkDeviceMemory stagingBufferMemory;
	//_renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
	ID stagingBufferId = _renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	auto device = _renderer.get_device();

	// copy into staging buffer
	void* mappedData = _renderer.map_buffer(stagingBufferId);
	memcpy(mappedData, indices, static_cast<size_t>(bufferSize));
	_renderer.unmap_buffer(stagingBufferId);
	//void* data;
	//vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	//memcpy(data, indices, (size_t)bufferSize);
	//vkUnmapMemory(device, stagingBufferMemory);

	// copy into device memory
	//_renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer, _indexMemory);
	_indexBufferId = _renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	_renderer.copy_buffer(stagingBufferId, _indexBufferId, bufferSize);

	// clean up
	_renderer.destroy_buffer(stagingBufferId);
}

void minty::Mesh::dispose_vertices()
{
	if (!_vertexCount)
	{
		return;
	}

	auto device = _renderer.get_device();

	_renderer.destroy_buffer(_vertexBufferId);
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

	_renderer.destroy_buffer(_indexBufferId);
	_indexCount = 0;
	_indexSize = 0;
}

std::string minty::to_string(Mesh const& mesh)
{
	return std::format("Mesh()");
}
