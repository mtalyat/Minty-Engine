#include "pch.h"
#include "M_Mesh.h"

#include "M_RenderEngine.h"

void minty::Mesh::setVertices(void const* const vertices, size_t const deviceSize, RenderEngine& engine)
{
	if (_vertexSet)
	{
		// if data already set, get rid of the old data
		disposeVertices(engine);
	}

	// get buffer size
	VkDeviceSize bufferSize = static_cast<VkDeviceSize>(deviceSize);

	// use buffer to copy data into device memory
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	engine.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	// copy into staging buffer
	void* data;
	vkMapMemory(engine.device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices, static_cast<size_t>(bufferSize));
	vkUnmapMemory(engine.device, stagingBufferMemory);

	// copy into device memory
	engine.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexMemory);

	engine.copyBuffer(stagingBuffer, _vertexBuffer, bufferSize);

	// clean up
	vkDestroyBuffer(engine.device, stagingBuffer, nullptr);
	vkFreeMemory(engine.device, stagingBufferMemory, nullptr);
}

void minty::Mesh::setIndices(void const* const indices, size_t const deviceSize, RenderEngine& engine)
{
	if (_indexSet)
	{
		// if data already set, get rid of the old data
		disposeIndices(engine);
	}

	// get buffer size
	VkDeviceSize bufferSize = static_cast<VkDeviceSize>(deviceSize);

	// use buffer to copy data into device memory
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	engine.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	// copy into staging buffer
	void* data;
	vkMapMemory(engine.device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices, (size_t)bufferSize);
	vkUnmapMemory(engine.device, stagingBufferMemory);

	// copy into device memory
	engine.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer, _indexMemory);

	engine.copyBuffer(stagingBuffer, _indexBuffer, bufferSize);

	// clean up
	vkDestroyBuffer(engine.device, stagingBuffer, nullptr);
	vkFreeMemory(engine.device, stagingBufferMemory, nullptr);
}

void minty::Mesh::dispose(RenderEngine& engine)
{
	// maybe swap order?
	disposeVertices(engine);
	disposeIndices(engine);
}

void minty::Mesh::disposeVertices(RenderEngine& engine)
{
	vkDestroyBuffer(engine.device, _vertexBuffer, nullptr);
	vkFreeMemory(engine.device, _vertexMemory, nullptr);
}

void minty::Mesh::disposeIndices(RenderEngine& engine)
{
	vkDestroyBuffer(engine.device, _indexBuffer, nullptr);
	vkFreeMemory(engine.device, _indexMemory, nullptr);
}
