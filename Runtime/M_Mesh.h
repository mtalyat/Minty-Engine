#pragma once
#include "M_Rendering_Object.h"

#include "M_Material.h"
#include <vulkan/vulkan.h>
#include <vector>

namespace minty
{
	/// <summary>
	/// Holds data for an object to be rendered to the screen.
	/// </summary>
	class Mesh :
		public rendering::RendererObject
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
		Mesh(Renderer& renderer);

		~Mesh();

#pragma region Set

		template<class T>
		void set_vertices(std::vector<T> const& vertices);

		template<class T>
		void set_vertices(T* const vertices, size_t const count);

		template<class T>
		void set_indices(std::vector<T> const& indices);

		template<class T>
		void set_indices(T* const indices, size_t const count);

#pragma endregion

#pragma region Get

		uint32_t get_vertex_count() const;

		ID get_vertex_buffer_id() const;

		uint32_t get_index_count() const;

		ID get_index_buffer_id() const;

		VkIndexType get_index_type() const;

#pragma endregion

	private:
		void set_vertices(void const* const vertices, size_t const count, size_t const vertexSize);

		void set_indices(void const* const indices, size_t const count, size_t const indexSize, VkIndexType const type = VkIndexType::VK_INDEX_TYPE_UINT16);

		void dispose_vertices();

		void dispose_indices();

	public:
		friend std::string to_string(Mesh const& mesh);
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

