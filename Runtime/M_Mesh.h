#pragma once
#include "M_Rendering_Object.h"

#include "M_Material.h"
#include <vulkan/vulkan.h>
#include <vector>

namespace minty
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

	String to_string(MeshType const value);
	MeshType from_string_mesh_type(String const& value);

	/// <summary>
	/// Holds data for a model to be rendered to the screen.
	/// </summary>
	class Mesh :
		public rendering::RenderObject
	{
	private:
		uint32_t _vertexCount;
		uint32_t _vertexSize;
		ID _vertexBufferId;

		uint32_t _indexCount;
		uint32_t _indexSize;
		ID _indexBufferId;
		VkIndexType _indexType;
	public:
		/// <summary>
		/// Creates an empty Mesh.
		/// </summary>
		Mesh();

		/// <summary>
		/// Creates a new Mesh.
		/// </summary>
		/// <param name="renderer"></param>
		Mesh(RenderEngine& renderer);

		~Mesh();

#pragma region Set

		/// <summary>
		/// Sets the vertex data for this Mesh.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="vertices"></param>
		template<class T>
		void set_vertices(std::vector<T> const& vertices);

		/// <summary>
		/// Sets the vertex data for this Mesh.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="vertices"></param>
		/// <param name="count"></param>
		template<class T>
		void set_vertices(T* const vertices, size_t const count);

		/// <summary>
		/// Sets the index data for this Mesh.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="indices"></param>
		template<class T>
		void set_indices(std::vector<T> const& indices);

		/// <summary>
		/// Sets the index data for this Mesh.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="indices"></param>
		/// <param name="count"></param>
		template<class T>
		void set_indices(T* const indices, size_t const count);

		/// <summary>
		/// Clears the vertex and index data for this Mesh.
		/// </summary>
		void clear();

#pragma endregion

#pragma region Get

		/// <summary>
		/// Gets the number of vertices in this Mesh.
		/// </summary>
		/// <returns></returns>
		uint32_t get_vertex_count() const;

		/// <summary>
		/// Gets the ID of the vertex buffer.
		/// </summary>
		/// <returns></returns>
		ID get_vertex_buffer_id() const;

		/// <summary>
		/// Gets the number of indices in this Mesh.
		/// </summary>
		/// <returns></returns>
		uint32_t get_index_count() const;

		/// <summary>
		/// Gets the ID of the index buffer.
		/// </summary>
		/// <returns></returns>
		ID get_index_buffer_id() const;

		/// <summary>
		/// Gets the index type for this Mesh.
		/// </summary>
		/// <returns></returns>
		VkIndexType get_index_type() const;

		/// <summary>
		/// Checks if this Mesh is empty.
		/// </summary>
		/// <returns></returns>
		bool empty() const;

#pragma endregion

#pragma region Primitives

		/// <summary>
		/// Replaces the given Mesh's data with the data for a primitive quad.
		/// </summary>
		/// <param name="mesh">The mesh to modify.</param>
		static void create_primitive_quad(Mesh& mesh);

		/// <summary>
		/// Replaces the given Mesh's data with the data for a primitive cube.
		/// </summary>
		/// <param name="mesh">The mesh to modify.</param>
		static void create_primitive_cube(Mesh& mesh);

		/// <summary>
		/// Replaces the given Mesh's data with the data for a primitive pyramid.
		/// </summary>
		/// <param name="mesh">The mesh to modify.</param>
		static void create_primitive_pyramid(Mesh& mesh);

		/// <summary>
		/// Replaces the given Mesh's data with the data for a primitive sphere.
		/// </summary>
		/// <param name="mesh">The mesh to modify.</param>
		static void create_primitive_sphere(Mesh& mesh);

		/// <summary>
		/// Replaces the given Mesh's data with the data for a primitive cylinder.
		/// </summary>
		/// <param name="mesh">The mesh to modify.</param>
		static void create_primitive_cylinder(Mesh& mesh);

#pragma endregion


	private:
		void set_vertices(void const* const vertices, size_t const count, size_t const vertexSize);

		void set_indices(void const* const indices, size_t const count, size_t const indexSize, VkIndexType const type = VkIndexType::VK_INDEX_TYPE_UINT16);

		void dispose_vertices();

		void dispose_indices();

	public:
		friend String to_string(Mesh const& mesh);
	};

	template<class T>
	void Mesh::set_vertices(std::vector<T> const& vertices)
	{
		set_vertices(static_cast<void const* const>(vertices.data()), vertices.size(), sizeof(T));
	}

	template<class T>
	void Mesh::set_vertices(T* const vertices, size_t const count)
	{
		set_vertices(static_cast<void const* const>(vertices), count, sizeof(T));
	}

	template<class T>
	void Mesh::set_indices(std::vector<T> const& indices)
	{
		set_indices(static_cast<void const* const>(indices.data()), indices.size(), sizeof(T));
	}

	template<class T>
	void Mesh::set_indices(T* const indices, size_t const count)
	{
		set_indices(static_cast<void const* const>(indices), count, sizeof(T));
	}
}

