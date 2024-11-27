#include "pch.h"
#include "Mesh.h"

#include "Minty/Core/Math.h"

#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanMesh.h"
#endif

using namespace Minty;

Minty::Mesh::Mesh(MeshBuilder const& builder)
    : Asset(builder.id)
	, m_vertexData()
    , m_vertexStride(0)
    , m_vertexCount(0)
    , m_vertexBuffer(nullptr)
    , m_indexData()
    , m_indexStride(0)
    , m_indexCount(0)
    , m_indexBuffer(nullptr)
{
	switch (builder.type)
	{
	case MeshType::Empty:
		break;
	case MeshType::Custom:
		initialize_custom(builder);
		break;
	case MeshType::Quad:
		initialize_quad();
		break;
	case MeshType::Cube:
		initialize_cube();
		break;
	case MeshType::Pyramid:
		initialize_pyramid();
		break;
	case MeshType::Sphere:
		initialize_sphere();
		break;
	case MeshType::Cylinder:
		initialize_cylinder();
		break;
	default:
		MINTY_ABORT("Unknown MeshType.");
	}
}

void Minty::Mesh::initialize_custom(MeshBuilder const& builder)
{
	m_vertexData = ConstantContainer(builder.vertexData, builder.vertexCount * builder.vertexStride);
	m_vertexStride = builder.vertexStride;
	m_vertexCount = builder.vertexCount;

	BufferBuilder vertexBufferBuilder{};
	vertexBufferBuilder.data = builder.vertexData;
	vertexBufferBuilder.size = builder.vertexCount * builder.vertexStride;
	vertexBufferBuilder.usage = BufferUsage::VERTEX;
	m_vertexBuffer = Buffer::create(vertexBufferBuilder);

	m_indexData = ConstantContainer(builder.indexData, builder.indexCount * builder.indexStride);
	m_indexStride = builder.indexStride;
	m_indexCount = builder.indexCount;

	BufferBuilder indexBufferBuilder{};
	indexBufferBuilder.data = builder.indexData;
	indexBufferBuilder.size = builder.indexCount * builder.indexStride;
	indexBufferBuilder.usage = BufferUsage::INDEX;
	m_indexBuffer = Buffer::create(indexBufferBuilder);
}

#define DEFAULT_VERTEX_SIZE 8

void Minty::Mesh::initialize_quad()
{
	// create mesh data
#define SIZE 0.5f

#define LEFT_TOP_BACK -SIZE, 0.0f, -SIZE
#define LEFT_TOP_FRONT -SIZE, 0.0f, SIZE
#define RIGHT_TOP_BACK SIZE, 0.0f, -SIZE
#define RIGHT_TOP_FRONT SIZE, 0.0f, SIZE

#define UP 0.0f, -1.0f, 0.0f

#define TOP_LEFT 0.0f, 0.0f
#define TOP_RIGHT 1.0f, 0.0f
#define BOTTOM_LEFT 0.0f, 1.0f
#define BOTTOM_RIGHT 1.0f, 1.0f

	std::vector<Float> vertices =
	{
		LEFT_TOP_BACK, UP, TOP_RIGHT,
		LEFT_TOP_FRONT, UP, BOTTOM_RIGHT,
		RIGHT_TOP_FRONT, UP, BOTTOM_LEFT,
		RIGHT_TOP_BACK, UP, TOP_LEFT
	};

	std::vector<UShort> indices =
	{
		0, 1, 2, 0, 2, 3
	};

	MeshBuilder builder{};
	builder.vertexCount = static_cast<UInt>(vertices.size() / DEFAULT_VERTEX_SIZE);
	builder.vertexStride = sizeof(Float) * DEFAULT_VERTEX_SIZE;
	builder.vertexData = vertices.data();
	builder.indexCount = static_cast<UInt>(vertices.size());
	builder.indexStride = sizeof(UShort);
	builder.indexData = indices.data();

	initialize_custom(builder);

#undef SIZE
#undef LEFT_TOP_BACK 
#undef LEFT_TOP_FRONT
#undef RIGHT_TOP_BACK
#undef RIGHT_TOP_FRONT
#undef UP
#undef TOP_LEFT
#undef TOP_RIGHT
#undef BOTTOM_LEFT
#undef BOTTOM_RIGHT
}

void Minty::Mesh::initialize_cube()
{
#define SIZE 0.5f

#define LEFT_BOTTOM_BACK -SIZE, SIZE, -SIZE
#define LEFT_BOTTOM_FRONT -SIZE, SIZE, SIZE
#define LEFT_TOP_BACK -SIZE, -SIZE, -SIZE
#define LEFT_TOP_FRONT -SIZE, -SIZE, SIZE
#define RIGHT_BOTTOM_BACK SIZE, SIZE, -SIZE
#define RIGHT_BOTTOM_FRONT SIZE, SIZE, SIZE
#define RIGHT_TOP_BACK SIZE, -SIZE, -SIZE
#define RIGHT_TOP_FRONT SIZE, -SIZE, SIZE

#define UP 0.0f, -1.0f, 0.0f
#define DOWN 0.0f, 1.0, 0.0f
#define LEFT -1.0f, 0.0f, 0.0f
#define RIGHT 1.0f, 0.0f, 0.0f
#define FORWARD 0.0f, 0.0f, 1.0f
#define BACKWARD 0.0f, 0.0f, -1.0f

#define TOP_LEFT 0.0f, 0.0f
#define TOP_RIGHT 1.0f, 0.0f
#define BOTTOM_LEFT 0.0f, 1.0f
#define BOTTOM_RIGHT 1.0f, 1.0f

	std::vector<Float> vertices =
	{
		// UP
		LEFT_TOP_BACK, UP, TOP_RIGHT,
		LEFT_TOP_FRONT, UP, BOTTOM_RIGHT, 
		RIGHT_TOP_FRONT, UP, BOTTOM_LEFT,
		RIGHT_TOP_BACK, UP, TOP_LEFT,

		// DOWN
		RIGHT_BOTTOM_BACK, DOWN, TOP_RIGHT,
		RIGHT_BOTTOM_FRONT, DOWN, BOTTOM_RIGHT, 
		LEFT_BOTTOM_FRONT, DOWN, BOTTOM_LEFT,
 		LEFT_BOTTOM_BACK, DOWN, TOP_LEFT,
 
		// RIGHT
		RIGHT_BOTTOM_BACK, RIGHT, BOTTOM_LEFT,
		RIGHT_TOP_BACK, RIGHT, TOP_LEFT,
		RIGHT_TOP_FRONT, RIGHT, TOP_RIGHT,
		RIGHT_BOTTOM_FRONT, RIGHT, BOTTOM_RIGHT, 

		// LEFT
		LEFT_BOTTOM_FRONT, LEFT, BOTTOM_LEFT,
		LEFT_TOP_FRONT, LEFT, TOP_LEFT,
		LEFT_TOP_BACK, LEFT, TOP_RIGHT,
		LEFT_BOTTOM_BACK, LEFT, BOTTOM_RIGHT, 

		// FRONT
		RIGHT_BOTTOM_FRONT, FORWARD, BOTTOM_LEFT,
		RIGHT_TOP_FRONT, FORWARD, TOP_LEFT,
		LEFT_TOP_FRONT, FORWARD, TOP_RIGHT,
		LEFT_BOTTOM_FRONT, FORWARD, BOTTOM_RIGHT, 

		// BACK
		LEFT_BOTTOM_BACK, BACKWARD, BOTTOM_LEFT,
		LEFT_TOP_BACK, BACKWARD, TOP_LEFT,
		RIGHT_TOP_BACK, BACKWARD, TOP_RIGHT,
		RIGHT_BOTTOM_BACK, BACKWARD, BOTTOM_RIGHT, 
	};

	std::vector<UShort> indices =
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
	};

	MeshBuilder builder{};
	builder.vertexCount = static_cast<UInt>(vertices.size() / DEFAULT_VERTEX_SIZE);
	builder.vertexStride = sizeof(Float) * DEFAULT_VERTEX_SIZE;
	builder.vertexData = vertices.data();
	builder.indexCount = static_cast<UInt>(vertices.size());
	builder.indexStride = sizeof(UShort);
	builder.indexData = indices.data();

	initialize_custom(builder);

#undef SIZE
#undef LEFT_BOTTOM_BACK
#undef LEFT_BOTTOM_FRONT 
#undef LEFT_TOP_BACK
#undef LEFT_TOP_FRONT
#undef RIGHT_BOTTOM_BACK 
#undef RIGHT_BOTTOM_FRONT
#undef RIGHT_TOP_BACK
#undef RIGHT_TOP_FRONT
#undef UP
#undef DOWN
#undef LEFT
#undef RIGHT
#undef FORWARD
#undef BACKWARD
#undef TOP_LEFT
#undef TOP_RIGHT
#undef BOTTOM_LEFT
#undef BOTTOM_RIGHT
}

void Minty::Mesh::initialize_pyramid()
{
#define SIZE 0.5f

#define LEFT_BOTTOM_BACK -SIZE, SIZE, -SIZE
#define LEFT_BOTTOM_FRONT -SIZE, SIZE, SIZE
#define RIGHT_BOTTOM_BACK SIZE, SIZE, -SIZE
#define RIGHT_BOTTOM_FRONT SIZE, SIZE, SIZE
#define TOP 0.0f, -SIZE, 0.0f

#define DOWN 0.0f, 1.0, 0.0f
#define LEFT -0.6667f, 0.3333f, 0.0f
#define RIGHT 0.6667f, 0.3333f, 0.0f
#define FORWARD 0.0f, 0.3333f, 0.6667f
#define BACKWARD 0.0f, 0.3333f, 0.6667f

#define TOP_LEFT 0.0f, 0.0f
#define TOP_RIGHT 1.0f, 0.0f
#define TOP_CENTER 0.5f, 0.0f
#define BOTTOM_LEFT 0.0f, 1.0f
#define BOTTOM_RIGHT 1.0f, 1.0f

	std::vector<Float> vertices =
	{
		// DOWN
		RIGHT_BOTTOM_BACK, DOWN, TOP_RIGHT,
		RIGHT_BOTTOM_FRONT, DOWN, BOTTOM_RIGHT,
		LEFT_BOTTOM_FRONT, DOWN, BOTTOM_LEFT,
		LEFT_BOTTOM_BACK, DOWN, TOP_LEFT,

		// FORWARD
		RIGHT_BOTTOM_FRONT, FORWARD, BOTTOM_LEFT,
		TOP, FORWARD, TOP_CENTER,
		LEFT_BOTTOM_FRONT, FORWARD, BOTTOM_RIGHT,

		// BACKWARD
		LEFT_BOTTOM_BACK, BACKWARD, BOTTOM_LEFT,
		TOP, BACKWARD, TOP_CENTER,
		RIGHT_BOTTOM_BACK, BACKWARD, BOTTOM_RIGHT,

		// LEFT
		LEFT_BOTTOM_FRONT, LEFT, BOTTOM_LEFT,
		TOP, LEFT, TOP_CENTER,
		LEFT_BOTTOM_BACK, LEFT, BOTTOM_RIGHT,

		// RIGHT
		RIGHT_BOTTOM_BACK, RIGHT, BOTTOM_LEFT,
		TOP, RIGHT, TOP_CENTER,
		RIGHT_BOTTOM_FRONT, RIGHT, BOTTOM_RIGHT
	};

	std::vector<UShort> indices =
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6,
		7, 8, 9,
		10, 11, 12,
		13, 14, 15,
	};

	MeshBuilder builder{};
	builder.vertexCount = static_cast<UInt>(vertices.size() / DEFAULT_VERTEX_SIZE);
	builder.vertexStride = sizeof(Float) * DEFAULT_VERTEX_SIZE;
	builder.vertexData = vertices.data();
	builder.indexCount = static_cast<UInt>(vertices.size());
	builder.indexStride = sizeof(UShort);
	builder.indexData = indices.data();

	initialize_custom(builder);

#undef SIZE
#undef LEFT_BOTTOM_BACK
#undef LEFT_BOTTOM_FRONT 
#undef RIGHT_BOTTOM_BACK 
#undef RIGHT_BOTTOM_FRONT
#undef TOP
#undef DOWN
#undef LEFT
#undef RIGHT
#undef FORWARD
#undef BACKWARD
#undef TOP_LEFT
#undef TOP_RIGHT
#undef TOP_CENTER
#undef BOTTOM_LEFT
#undef BOTTOM_RIGHT
}

void Minty::Mesh::initialize_sphere()
{
	Float const RADIUS = 0.5f;
	Int const SECTORS = 36;
	Int const STACKS = 36;

	Float const PI = Math::PI;

	Float const SECTOR_STEP = 2.0f * PI / SECTORS;
	Float const STACK_STEP = PI / STACKS;

	std::vector<Float> vertices;

	Float stackAngle, sectorAngle;

	for (Int i = 0; i <= STACKS; i++)
	{
		stackAngle = PI / 2.0f - i * STACK_STEP;

		Float xy = RADIUS * Math::cos(stackAngle);
		Float z = RADIUS * Math::sin(stackAngle);

		for (Int j = 0; j <= SECTORS; j++)
		{
			sectorAngle = j * SECTOR_STEP;

			Float x = xy * Math::cos(sectorAngle);

			Float y = xy * Math::sin(sectorAngle);

			Float texCoordX = (x / (RADIUS * 2.0f)) + 0.5f;
			Float texCoordY = (y / (RADIUS * 2.0f)) + 0.5f;

			Float3 pos(x, y, z);
			Float3 normal = glm::normalize(pos);
			Float2 texCoord(texCoordX, texCoordY);

			vertices.push_back(pos.x);
			vertices.push_back(pos.y);
			vertices.push_back(pos.z);
			vertices.push_back(normal.x);
			vertices.push_back(normal.y);
			vertices.push_back(normal.z);
			vertices.push_back(texCoord.x);
			vertices.push_back(texCoord.y);
		}
	}
	
	std::vector<UShort> indices;

	for (Int i = 0; i < STACKS; i++)
	{
		for (Int j = 0; j < SECTORS; j++)
		{
			Int first = (i * (SECTORS + 1)) + j;
			Int second = first + SECTORS + 1;

			indices.push_back(static_cast<UShort>(first + 1));
			indices.push_back(static_cast<UShort>(second));
			indices.push_back(static_cast<UShort>(first));
			indices.push_back(static_cast<UShort>(first + 1));
			indices.push_back(static_cast<UShort>(second + 1));
			indices.push_back(static_cast<UShort>(second));
		}
	}

	MeshBuilder builder{};
	builder.vertexCount = static_cast<UInt>(vertices.size() / DEFAULT_VERTEX_SIZE);
	builder.vertexStride = sizeof(Float) * DEFAULT_VERTEX_SIZE;
	builder.vertexData = vertices.data();
	builder.indexCount = static_cast<UInt>(vertices.size());
	builder.indexStride = sizeof(UShort);
	builder.indexData = indices.data();

	initialize_custom(builder);
}

void Minty::Mesh::initialize_cylinder()
{
	Float const RADIUS = 0.5f;
	Float const SIZE = 0.5f;
	Int const SECTORS = 36;

	Float const PI = glm::pi<Float>();
	Float const PI2 = PI * 1.5f;

	Float const STEP = 2.0f * PI / SECTORS;

	Float3 const UP(0.0f, -1.0f, 0.0f);
	Float3 const DOWN(0.0f, 1.0f, 0.0f);

	std::vector<Float> vertices;

	Float angle;

	// get circle points
	std::vector<Float2> points(SECTORS + 1);
	for (Int i = 0; i <= SECTORS; i++)
	{
		angle = i * STEP + PI2;

		points[i].x = RADIUS * Math::cos(angle);
		points[i].y = RADIUS * Math::sin(angle);
	}

	// add top center
	vertices.push_back(0.0f);
	vertices.push_back(-SIZE);
	vertices.push_back(0.0f);
	vertices.push_back(UP.x);
	vertices.push_back(UP.y);
	vertices.push_back(UP.z);
	vertices.push_back(0.5f);
	vertices.push_back(0.5f);

	// add top sides
	for (Int i = 0; i <= SECTORS; i++)
	{
		Float2 point = points.at(i);

		vertices.push_back(point.x);
		vertices.push_back(-SIZE);
		vertices.push_back(point.y);
		vertices.push_back(UP.x);
		vertices.push_back(UP.y);
		vertices.push_back(UP.z);
		vertices.push_back(point.x + 0.5f);
		vertices.push_back(point.y + 0.5f);
	}

	// add bottom center
	vertices.push_back(0.0f);
	vertices.push_back(SIZE);
	vertices.push_back(0.0f);
	vertices.push_back(DOWN.x);
	vertices.push_back(DOWN.y);
	vertices.push_back(DOWN.z);
	vertices.push_back(0.5f);
	vertices.push_back(0.5f);

	// add bottom sides
	for (Int i = 0; i <= SECTORS; i++)
	{
		Float2 point = points.at(i);

		vertices.push_back(point.x);
		vertices.push_back(SIZE);
		vertices.push_back(point.y);
		vertices.push_back(DOWN.x);
		vertices.push_back(DOWN.y);
		vertices.push_back(DOWN.z);
		vertices.push_back(point.x + 0.5f);
		vertices.push_back(point.y + 0.5f);
	}

	// add sides
	for (Int i = 0; i <= SECTORS; i++)
	{
		Float2 point = points.at(i);

		Float3 normal = glm::normalize(Float3(point.x, 0.0f, point.y));

		vertices.push_back(point.x);
		vertices.push_back(-SIZE);
		vertices.push_back(point.y);
		vertices.push_back(normal.x);
		vertices.push_back(normal.y);
		vertices.push_back(normal.z);
		vertices.push_back(static_cast<Float>(i) / SECTORS);
		vertices.push_back(0.0f);

		vertices.push_back(point.x);
		vertices.push_back(SIZE);
		vertices.push_back(point.y);
		vertices.push_back(normal.x);
		vertices.push_back(normal.y);
		vertices.push_back(normal.z);
		vertices.push_back(static_cast<Float>(i) / SECTORS);
		vertices.push_back(1.0f);
	}

	std::vector<UShort> indices;

	UShort center = 0;
	UShort offset = center + 1;

	// top
	for (Int i = 0; i < SECTORS; i++)
	{
		indices.push_back(offset + 1 + i);
		indices.push_back(offset + i);
		indices.push_back(center);
	}

	center += SECTORS + 2;
	offset = center + 1;

	// bottom
	for (Int i = 0; i < SECTORS; i++)
	{
		indices.push_back(center);
		indices.push_back(offset + i);
		indices.push_back(offset + 1 + i);
	}

	// side
	offset += SECTORS + 1;

	for (Int i = 0; i < SECTORS * 2; i += 2)
	{
		indices.push_back(offset + i);
		indices.push_back(offset + i + 2);
		indices.push_back(offset + i + 1);
		indices.push_back(offset + i + 2);
		indices.push_back(offset + i + 3);
		indices.push_back(offset + i + 1);
	}

	MeshBuilder builder{};
	builder.vertexCount = static_cast<UInt>(vertices.size() / DEFAULT_VERTEX_SIZE);
	builder.vertexStride = sizeof(Float) * DEFAULT_VERTEX_SIZE;
	builder.vertexData = vertices.data();
	builder.indexCount = static_cast<UInt>(vertices.size());
	builder.indexStride = sizeof(UShort);
	builder.indexData = indices.data();

	initialize_custom(builder);
}

#undef DEFAULT_VERTEX_SIZE

Owner<Mesh> Minty::Mesh::create(MeshBuilder const& builder)
{
#if defined(MINTY_VULKAN)
    return Owner<VulkanMesh>(builder);
#else
    return nullptr;
#endif
}