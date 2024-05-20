#include "pch.h"
#include "rendering/M_Mesh.h"

#include "runtime/M_Runtime.h"
#include "rendering/M_Builtin.h"
#include "rendering/M_RenderEngine.h"
#include "assets/M_AssetEngine.h"
#include "assets/M_Text.h"
#include "libraries/M_GLM.hpp"

using namespace minty;
using namespace minty::Builtin;
using namespace minty;

minty::Mesh::Mesh()
	: Asset()
	, _vertexCount()
	, _vertexSize()
	, _vertexBufferId(INVALID_UUID)
	, _vertexBuffer()
	, _indexCount()
	, _indexSize()
	, _indexBufferId(INVALID_UUID)
	, _indexBuffer()
	, _indexType()
{}

minty::Mesh::Mesh(MeshBuilder const& builder, Runtime& engine)
	: Asset(builder.id, builder.path, engine)
	, _vertexCount()
	, _vertexSize()
	, _vertexBufferId(INVALID_UUID)
	, _vertexBuffer()
	, _indexCount()
	, _indexSize()
	, _indexBufferId(INVALID_UUID)
	, _indexBuffer()
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

Buffer const* minty::Mesh::get_vertex_buffer() const
{
	return _vertexBuffer;
}

uint32_t minty::Mesh::get_index_count() const
{
	return _indexCount;
}

Buffer const* minty::Mesh::get_index_buffer() const
{
	return _indexBuffer;
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
		{ leftTopBack, up, topRight },
		{ leftTopFront, up, bottomRight },
		{ rightTopFront, up, bottomLeft },
		{ rightTopBack, up, topLeft },
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
		{ leftTopBack, up, topRight },
		{ leftTopFront, up, bottomRight },
		{ rightTopFront, up, bottomLeft },
		{ rightTopBack, up, topLeft },

		// down
		{ rightBottomBack, down, topRight },
		{ rightBottomFront, down, bottomRight },
		{ leftBottomFront, down, bottomLeft },
		{ leftBottomBack, down, topLeft },

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
		{ rightBottomBack, down, topRight },
		{ rightBottomFront, down, bottomRight },
		{ leftBottomFront, down, bottomLeft },
		{ leftBottomBack, down, topLeft },

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

void minty::Mesh::create_primitive_sphere(Mesh& mesh)
{
	float const RADIUS = 0.5f;
	int const SECTORS = 36;
	int const STACKS = 36;

	{
		float const PI = glm::pi<float>();

		float const SECTOR_STEP = 2.0f * PI / SECTORS;
		float const STACK_STEP = PI / STACKS;

		std::vector<Vertex3D> vertices;

		float stackAngle, sectorAngle;

		for (int i = 0; i <= STACKS; i++)
		{
			stackAngle = PI / 2.0f - i * STACK_STEP;

			float xy = RADIUS * Math::cos(stackAngle);
			float z = RADIUS * Math::sin(stackAngle);

			for (int j = 0; j <= SECTORS; j++)
			{
				sectorAngle = j * SECTOR_STEP;

				float x = xy * Math::cos(sectorAngle);

				float y = xy * Math::sin(sectorAngle);

				float texCoordX = (x / (RADIUS * 2.0f)) + 0.5f;
				float texCoordY = (y / (RADIUS * 2.0f)) + 0.5f;

				Vector3 pos(x, y, z);
				Vector3 normal = glm::normalize(pos);
				Vector2 texCoord(texCoordX, texCoordY);

				vertices.push_back(
					{
						pos, normal, texCoord
					}
				);
			}
		}

		mesh.set_vertices(vertices);
	}

	{
		std::vector<uint16_t> indices;

		for (int i = 0; i < STACKS; i++)
		{
			for (int j = 0; j < SECTORS; j++)
			{
				int first = (i * (SECTORS + 1)) + j;
				int second = first + SECTORS + 1;

				indices.push_back(static_cast<uint16_t>(first + 1));
				indices.push_back(static_cast<uint16_t>(second));
				indices.push_back(static_cast<uint16_t>(first));
				indices.push_back(static_cast<uint16_t>(first + 1));
				indices.push_back(static_cast<uint16_t>(second + 1));
				indices.push_back(static_cast<uint16_t>(second));
			}
		}

		mesh.set_indices(indices);
	}
}

void minty::Mesh::create_primitive_cylinder(Mesh& mesh)
{
	float const RADIUS = 0.5f;
	float const SIZE = 0.5f;
	int const SECTORS = 36;

	{
		float const PI = glm::pi<float>();
		float const PI2 = PI * 1.5f;

		float const STEP = 2.0f * PI / SECTORS;

		Vector3 const UP(0.0f, -1.0f, 0.0f);
		Vector3 const DOWN(0.0f, 1.0f, 0.0f);

		std::vector<Vertex3D> vertices;

		float angle;

		// get circle points
		std::vector<Vector2> points(SECTORS + 1);
		for (int i = 0; i <= SECTORS; i++)
		{
			angle = i * STEP + PI2;

			points[i].x = RADIUS * Math::cos(angle);
			points[i].y = RADIUS * Math::sin(angle);
		}

		// add top center
		vertices.push_back({
			{0.0f, -SIZE, 0.0f}, UP, {0.5f, 0.5f}
			});
		// add top sides
		for (int i = 0; i <= SECTORS; i++)
		{
			Vector2 point = points.at(i);

			vertices.push_back({
				{point.x, -SIZE, point.y}, UP, { point.x + 0.5f, point.y + 0.5f }
				});
		}

		// add bottom center
		vertices.push_back({
			{0.0f, SIZE, 0.0f}, DOWN, {0.5f, 0.5f}
			});
		// add bottom sides
		for (int i = 0; i <= SECTORS; i++)
		{
			Vector2 point = points.at(i);

			vertices.push_back({
				{point.x, SIZE, point.y}, DOWN, { point.x + 0.5f, point.y + 0.5f }
				});
		}

		// add sides
		for (int i = 0; i <= SECTORS; i++)
		{
			Vector2 point = points.at(i);

			Vector3 normal = glm::normalize(Vector3(point.x, 0.0f, point.y));

			vertices.push_back({
				{point.x, -SIZE, point.y}, normal, { static_cast<float>(i) / SECTORS, 0.0f }
				});
			vertices.push_back({
				{point.x, SIZE, point.y}, normal, { static_cast<float>(i) / SECTORS, 1.0f }
				});
		}

		mesh.set_vertices(vertices);
	}

	{
		std::vector<uint16_t> indices;

		uint16_t center = 0;
		uint16_t offset = center + 1;

		// top
		for (int i = 0; i < SECTORS; i++)
		{
			indices.push_back(offset + 1 + i);
			indices.push_back(offset + i);
			indices.push_back(center);
		}

		center += SECTORS + 2;
		offset = center + 1;

		// bottom
		for (int i = 0; i < SECTORS; i++)
		{
			indices.push_back(center);
			indices.push_back(offset + i);
			indices.push_back(offset + 1 + i);
		}

		// side
		offset += SECTORS + 1;

		for (int i = 0; i < SECTORS * 2; i += 2)
		{
			indices.push_back(offset + i);
			indices.push_back(offset + i + 2);
			indices.push_back(offset + i + 1);
			indices.push_back(offset + i + 2);
			indices.push_back(offset + i + 3);
			indices.push_back(offset + i + 1);
		}

		mesh.set_indices(indices);
	}
}

void minty::Mesh::set_vertices(void const* const vertices, size_t const count, size_t const vertexSize)
{
	// if data already set, get rid of the old data
	dispose_vertices();

	_vertexCount = static_cast<uint32_t>(count);
	_vertexSize = static_cast<uint32_t>(vertexSize);

	if (!vertices || !_vertexCount || !_vertexSize)
	{
		// empty vertex array
		return;
	}

	RenderEngine& renderer = get_runtime().get_render_engine();

	// get buffer size
	VkDeviceSize bufferSize = static_cast<VkDeviceSize>(count * vertexSize);

	// use buffer to copy data into device memory
	Buffer const& stagingBuffer = renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	auto device = renderer.get_device();

	// map data so it can be set
	void* mappedData = renderer.map_buffer(stagingBuffer);

	// copy into staging buffer
	memcpy(mappedData, vertices, static_cast<size_t>(bufferSize));

	// unmap
	renderer.unmap_buffer(stagingBuffer);

	// copy into device memory
	Buffer const& vertexBuffer = renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	_vertexBufferId = vertexBuffer.get_id();
	_vertexBuffer = &vertexBuffer;

	renderer.copy_buffer(stagingBuffer, vertexBuffer, bufferSize);

	// clean up
	renderer.destroy_buffer(stagingBuffer);
}

void minty::Mesh::set_indices(void const* const indices, size_t const count, size_t const indexSize, VkIndexType const type)
{
	// if data already set, get rid of the old data
	dispose_indices();

	_indexCount = static_cast<uint32_t>(count);
	_indexSize = static_cast<uint32_t>(indexSize);
	_indexType = type;

	if (!indices || !_indexCount || !_indexSize)
	{
		// empty vertex array
		return;
	}

	RenderEngine& renderer = get_runtime().get_render_engine();

	// get buffer size
	VkDeviceSize bufferSize = static_cast<VkDeviceSize>(count * indexSize);

	// use buffer to copy data into device memory
	Buffer const& stagingBuffer = renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	auto device = renderer.get_device();

	// copy into staging buffer
	void* mappedData = renderer.map_buffer(stagingBuffer);
	memcpy(mappedData, indices, static_cast<size_t>(bufferSize));
	renderer.unmap_buffer(stagingBuffer);

	// copy into device memory
	Buffer const& indexBuffer = renderer.create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	_indexBufferId = indexBuffer.get_id();
	_indexBuffer = &indexBuffer;

	renderer.copy_buffer(stagingBuffer, indexBuffer, bufferSize);

	// clean up
	renderer.destroy_buffer(stagingBuffer);
}

void minty::Mesh::dispose_vertices()
{
	AssetEngine& assets = get_runtime().get_asset_engine();

	// if buffer not loaded, ignore, it was already disposed
	if (!assets.contains(_vertexBufferId))
	{
		return;
	}

	RenderEngine& renderer = get_runtime().get_render_engine();

	renderer.destroy_buffer(*_vertexBuffer);
	_vertexBufferId = INVALID_UUID;
	_vertexBuffer = nullptr;
	_vertexCount = 0;
	_vertexSize = 0;
}

void minty::Mesh::dispose_indices()
{
	AssetEngine& assets = get_runtime().get_asset_engine();

	// if buffer not loaded, ignore, it was already disposed
	if (!assets.contains(_indexBufferId))
	{
		return;
	}

	RenderEngine& renderer = get_runtime().get_render_engine();

	renderer.destroy_buffer(*_indexBuffer);
	_indexBufferId = INVALID_UUID;
	_indexBuffer = nullptr;
	_indexCount = 0;
	_indexSize = 0;
}

String minty::to_string(MeshType const value)
{
	switch (value)
	{
	case MeshType::Empty:
		return "EMPTY";
	case MeshType::Custom:
		return "CUSTOM";
	case MeshType::Quad:
		return "QUAD";
	case MeshType::Cube:
		return "CUBE";
	case MeshType::Pyramid:
		return "PYRAMID";
	case MeshType::Sphere:
		return "SPHERE";
	case MeshType::Cylinder:
		return "CYLINDER";
	default:
		return "";
	}
}

MeshType minty::from_string_mesh_type(String const& value)
{
	String value2 = Text::to_upper(value);
	if (value2 == "CUSTOM")
	{
		return MeshType::Custom;
	}
	else if (value2 == "QUAD")
	{
		return MeshType::Quad;
	}
	else if (value2 == "CUBE")
	{
		return MeshType::Cube;
	}
	else if (value2 == "PYRAMID")
	{
		return MeshType::Pyramid;
	}
	else if (value2 == "SPHERE")
	{
		return MeshType::Sphere;
	}
	else if (value2 == "CYLINDER")
	{
		return MeshType::Cylinder;
	}
	else
	{
		return MeshType::Empty;
	}
}

String minty::to_string(Mesh const& mesh)
{
	return std::format("Mesh()");
}
