#include "pch.h"
#include "M_Mesh.h"

#include "M_Renderer.h"
#include "M_Vertex.h"

void minty::Mesh::setVertices(void const* const vertices, size_t const elementCount, size_t const elementSize, Renderer& engine)
{
	if (_vertexCount > 0)
	{
		// if data already set, get rid of the old data
		disposeVertices(engine);
	}

	_vertexCount = elementCount;
	_vertexSize = elementSize;

	// get buffer size
	VkDeviceSize bufferSize = static_cast<VkDeviceSize>(elementCount * elementSize);

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

void minty::Mesh::setIndices(void const* const indices, size_t const elementCount, size_t const elementSize, Renderer& engine)
{
	if (_indexCount > 0)
	{
		// if data already set, get rid of the old data
		disposeIndices(engine);
	}

	_indexCount = elementCount;
	_indexSize = elementSize;

	// get buffer size
	VkDeviceSize bufferSize = static_cast<VkDeviceSize>(elementCount * elementSize);

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

void minty::Mesh::setMaterial(ID const materialId)
{
	_materialId = materialId;
}

void minty::Mesh::dispose(Renderer& engine)
{
	// maybe swap order?
	disposeVertices(engine);
	disposeIndices(engine);
}

minty::Mesh minty::Mesh::createCube(Renderer& engine)
{
	// create mesh data
	const float SIZE = 0.5f;

	glm::vec3 leftBottomBack = { -SIZE, -SIZE, -SIZE };
	glm::vec3 leftBottomFront = { -SIZE, -SIZE, SIZE };
	glm::vec3 leftTopBack = { -SIZE, SIZE, -SIZE };
	glm::vec3 leftTopFront = { -SIZE, SIZE, SIZE };
	glm::vec3 rightBottomBack = { SIZE, -SIZE, -SIZE };
	glm::vec3 rightBottomFront = { SIZE, -SIZE, SIZE };
	glm::vec3 rightTopBack = { SIZE, SIZE, -SIZE };
	glm::vec3 rightTopFront = { SIZE, SIZE, SIZE };

	glm::vec3 color = { 1.0f, 1.0f, 1.0f };

	glm::vec2 topLeft = { 0.0f, 0.0f };
	glm::vec2 topRight = { 1.0f, 0.0f };
	glm::vec2 bottomLeft = { 0.0f, 1.0f };
	glm::vec2 bottomRight = { 1.0f, 1.0f };

	std::vector<Vertex> vertices =
	{
		// up?
		{ leftTopBack, color, bottomLeft },
		{ leftTopFront, color, topLeft },
		{ rightTopFront, color, topRight },
		{ rightTopBack, color, bottomRight },

		// down?
		{ rightBottomBack, color, bottomLeft },
		{ rightBottomFront, color, topLeft },
		{ leftBottomFront, color, topRight },
		{ leftBottomBack, color, bottomRight },

		// right?
		{ rightBottomBack, color, bottomLeft },
		{ rightTopBack, color, topLeft },
		{ rightTopFront, color, topRight },
		{ rightBottomFront, color, bottomRight },

		// left?
		{ leftBottomFront, color, bottomLeft },
		{ leftTopFront, color, topLeft },
		{ leftTopBack, color, topRight },
		{ leftBottomBack, color, bottomRight },

		// front?
		{ rightBottomFront, color, bottomLeft },
		{ rightTopFront, color, topLeft },
		{ leftTopFront, color, topRight },
		{ leftBottomFront, color, bottomRight },

		// back?
		{ leftBottomBack, color, bottomLeft },
		{ leftTopBack, color, topLeft },
		{ rightTopBack, color, topRight },
		{ rightBottomBack, color, bottomRight },
	};

	std::vector<uint16_t> indices =
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
	};

	// set mesh data
	Mesh mesh;
	mesh.setVertices(static_cast<void const*>(vertices.data()), vertices.size(), sizeof(Vertex), engine);
	mesh.setIndices(static_cast<void const*>(indices.data()), indices.size(), sizeof(uint16_t), engine);

	// done
	return mesh;
}

minty::Mesh minty::Mesh::createQuad(Renderer& engine)
{
	return Mesh();
}

void minty::Mesh::disposeVertices(Renderer& engine)
{
	vkDestroyBuffer(engine.device, _vertexBuffer, nullptr);
	vkFreeMemory(engine.device, _vertexMemory, nullptr);
	_vertexCount = 0;
}

void minty::Mesh::disposeIndices(Renderer& engine)
{
	vkDestroyBuffer(engine.device, _indexBuffer, nullptr);
	vkFreeMemory(engine.device, _indexMemory, nullptr);
	_indexCount = 0;
}
