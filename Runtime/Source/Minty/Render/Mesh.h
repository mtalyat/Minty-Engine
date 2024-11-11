#pragma once
#include "Minty/Asset/Asset.h"

#include "Minty/Data/ConstantContainer.h"
#include "Minty/Render/Buffer.h"

namespace Minty
{
	/// <summary>
	/// The type of mesh.
	/// </summary>
	enum class MeshType
	{
		/// <summary>
		/// The mesh is empty and has no data.
		/// </summary>
		Empty,

		/// <summary>
		/// The mesh is a custom model.
		/// </summary>
		Custom,

		/// <summary>
		/// The mesh is a flat quad (square).
		/// </summary>
		Quad,

		/// <summary>
		/// The mesh is a cube.
		/// </summary>
		Cube,

		/// <summary>
		/// The mesh is a pyramid.
		/// </summary>
		Pyramid,

		/// <summary>
		/// The mesh is a sphere.
		/// </summary>
		Sphere,

		/// <summary>
		/// The mesh is a cylinder.
		/// </summary>
		Cylinder,
	};

	struct MeshBuilder
	{
		UUID id = {};

		// the type of mesh
		MeshType type = MeshType::Custom;

		void* vertexData = nullptr;	// pointer to data
		UInt vertexCount = 0;				// number of vertices
		Size vertexStride = 0;				// size of a vertex in bytes

		void* indexData = nullptr;	// pointer to data
		UInt indexCount = 0;				// number of indices
		Size indexStride = 0;				// size of one index in bytes
	};

	class Mesh
		: public Asset
	{
	protected:
		ConstantContainer m_vertexData;
		Size m_vertexStride;
		UInt m_vertexCount;
		Owner<Buffer> m_vertexBuffer;

		ConstantContainer m_indexData;
		Size m_indexStride;
		UInt m_indexCount;
		Owner<Buffer> m_indexBuffer;

	protected:
		Mesh(const MeshBuilder& builder);

	public:
		virtual ~Mesh() = default;

	public:
		virtual void on_bind() const = 0;

	public:
		UInt get_vertex_count() const { return m_vertexCount; }

		UInt get_index_count() const { return m_indexCount; }

		Bool empty() const { return get_vertex_count() == 0; }

	private:


	private:
		void initialize_custom(const MeshBuilder& builder);
		void initialize_quad();
		void initialize_cube();
		void initialize_pyramid();
		void initialize_sphere();
		void initialize_cylinder();

	public:
		AssetType get_type() const override { return AssetType::Mesh; }

	public:
		static Owner<Mesh> create(const MeshBuilder& builder = {});
	};
}