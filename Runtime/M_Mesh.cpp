#include "pch.h"
#include "M_Mesh.h"

#include "M_Basic_Vertex.h"
#include "M_Renderer.h"
#include "glm.hpp"

using namespace minty;
using namespace minty::basic;
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

void minty::Mesh::clear()
{
	set_vertices(nullptr, 0, 0);
	set_indices(nullptr, 0, 0);
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

bool minty::Mesh::empty() const
{
	return _vertexCount == 0;
}

void minty::Mesh::create_primitive_quad(Mesh& mesh)
{
	// create mesh data
	const float SIZE = 0.5f;

	Vector3 leftTopBack = { -SIZE, 0.0f, -SIZE };
	Vector3 leftTopFront = { -SIZE, 0.0f, SIZE };
	Vector3 rightTopBack = { SIZE, 0.0f, -SIZE };
	Vector3 rightTopFront = { SIZE, 0.0f, SIZE };

	Vector3 up = { 0.0f, -1.0f, 0.0f };

	Vector2 topLeft = { 0.0f, 0.0f };
	Vector2 topRight = { 1.0f, 0.0f };
	Vector2 bottomLeft = { 0.0f, 1.0f };
	Vector2 bottomRight = { 1.0f, 1.0f };

	std::vector<Vertex3D> vertices =
	{
		// up
		{ leftTopBack, up, bottomLeft},
		{ leftTopFront, up, topLeft },
		{ rightTopFront, up, topRight },
		{ rightTopBack, up, bottomRight },
	};

	std::vector<uint16_t> indices =
	{
		0, 1, 2, 0, 2, 3
	};

	// set mesh data
	mesh.set_vertices(vertices);
	mesh.set_indices(indices);
}

void minty::Mesh::create_primitive_cube(Mesh& mesh)
{
	// create mesh data
	const float SIZE = 0.5f;

	Vector3 leftBottomBack = { -SIZE, SIZE, -SIZE };
	Vector3 leftBottomFront = { -SIZE, SIZE, SIZE };
	Vector3 leftTopBack = { -SIZE, -SIZE, -SIZE };
	Vector3 leftTopFront = { -SIZE, -SIZE, SIZE };
	Vector3 rightBottomBack = { SIZE, SIZE, -SIZE };
	Vector3 rightBottomFront = { SIZE, SIZE, SIZE };
	Vector3 rightTopBack = { SIZE, -SIZE, -SIZE };
	Vector3 rightTopFront = { SIZE, -SIZE, SIZE };

	Vector3 up = { 0.0f, -1.0f, 0.0f };
	Vector3 down = { 0.0f, 1.0f, 0.0f };
	Vector3 left = { -1.0f, 0.0f, 0.0f };
	Vector3 right = { 1.0f, 0.0f, 0.0f };
	Vector3 forward = { 0.0f, 0.0f, 1.0f };
	Vector3 backward = { 0.0f, 0.0f, -1.0f };

	Vector2 topLeft = { 0.0f, 0.0f };
	Vector2 topRight = { 1.0f, 0.0f };
	Vector2 bottomLeft = { 0.0f, 1.0f };
	Vector2 bottomRight = { 1.0f, 1.0f };

	std::vector<Vertex3D> vertices =
	{
		// up
		{ leftTopBack, up, bottomLeft },
		{ leftTopFront, up, topLeft },
		{ rightTopFront, up, topRight },
		{ rightTopBack, up, bottomRight },

		// down
		{ rightBottomBack, down, bottomLeft },
		{ rightBottomFront, down, topLeft },
		{ leftBottomFront, down, topRight },
		{ leftBottomBack, down, bottomRight },

		// right
		{ rightBottomBack, right, bottomLeft },
		{ rightTopBack, right, topLeft },
		{ rightTopFront, right, topRight },
		{ rightBottomFront, right, bottomRight },

		// left
		{ leftBottomFront, left, bottomLeft },
		{ leftTopFront, left, topLeft },
		{ leftTopBack, left, topRight },
		{ leftBottomBack, left, bottomRight },

		// front
		{ rightBottomFront, forward, bottomLeft },
		{ rightTopFront, forward, topLeft },
		{ leftTopFront, forward, topRight },
		{ leftBottomFront, forward, bottomRight },

		// back
		{ leftBottomBack, backward, bottomLeft },
		{ leftTopBack, backward, topLeft },
		{ rightTopBack, backward, topRight },
		{ rightBottomBack, backward, bottomRight },
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
	mesh.set_vertices(vertices);
	mesh.set_indices(indices);
}

void minty::Mesh::create_primitive_pyramid(Mesh& mesh)
{
	// create mesh data
	const float SIZE = 0.5f;

	Vector3 leftBottomBack = { -SIZE, SIZE, -SIZE };
	Vector3 leftBottomFront = { -SIZE, SIZE, SIZE };
	Vector3 rightBottomBack = { SIZE, SIZE, -SIZE };
	Vector3 rightBottomFront = { SIZE, SIZE, SIZE };
	Vector3 top = { 0.0f, -SIZE, 0.0f };

	Vector3 down = { 0.0f, 1.0f, 0.0f };
	Vector3 forward = glm::normalize(Vector3({ 0.0f, 1.0f, 2.0f }));
	Vector3 backward = glm::normalize(Vector3({ 0.0f, 1.0f, -2.0f }));
	Vector3 left = glm::normalize(Vector3({ -2.0f, 1.0f, 0.0f }));
	Vector3 right = glm::normalize(Vector3({ 2.0f, 1.0f, 0.0f }));

	Vector2 topLeft = { 0.0f, 0.0f };
	Vector2 topRight = { 1.0f, 0.0f };
	Vector2 topCenter = { 0.5f, 0.0f };
	Vector2 bottomLeft = { 0.0f, 1.0f };
	Vector2 bottomRight = { 1.0f, 1.0f };

	std::vector<Vertex3D> vertices =
	{
		// up
		{ rightBottomBack, down, bottomLeft },
		{ rightBottomFront, down, topLeft },
		{ leftBottomFront, down, topRight },
		{ leftBottomBack, down, bottomRight },

		// forward
		{ rightBottomFront, forward, bottomLeft },
		{ top, forward, topCenter },
		{ leftBottomFront, forward, bottomRight },

		// backward
		{ leftBottomBack, backward, bottomLeft },
		{ top, backward, topCenter },
		{ rightBottomBack, backward, bottomRight },

		// left
		{ leftBottomFront, left, bottomLeft },
		{ top, left, topCenter },
		{ leftBottomBack, left, bottomRight },

		// right
		{ rightBottomBack, right, bottomLeft },
		{ top, right, topCenter },
		{ rightBottomFront, right, bottomRight },
	};

	std::vector<uint16_t> indices =
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6,
		7, 8, 9,
		10, 11, 12,
		13, 14, 15,
	};

	// set mesh data
	mesh.set_vertices(vertices);
	mesh.set_indices(indices);
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

std::string minty::to_string(MeshType const value)
{
	switch (value)
	{
	case MeshType::Empty:
		return "Empty";
	case MeshType::Custom:
		return "Custom";
	case MeshType::Quad:
		return "Quad";
	case MeshType::Cube:
		return "Cube";
	case MeshType::Pyramid:
		return "Pyramid";
	default:
		return error::ERROR_TEXT;
	}
}

MeshType minty::from_string_mesh_type(std::string const& value)
{
	if (value == "Custom")
	{
		return MeshType::Custom;
	}
	else if (value == "Quad")
	{
		return MeshType::Quad;
	}
	else if (value == "Cube")
	{
		return MeshType::Cube;
	}
	else if (value == "Pyramid")
	{
		return MeshType::Pyramid;
	}
	else
	{
		return MeshType::Empty;
	}
}

std::string minty::to_string(Mesh const& mesh)
{
	return std::format("Mesh()");
}
