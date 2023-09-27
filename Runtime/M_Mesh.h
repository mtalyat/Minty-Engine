#pragma once
#include "M_Object.h"

#include "M_Material.h"
#include <vulkan/vulkan.h>
#include <vector>

namespace minty
{
	class Renderer;

	/// <summary>
	/// Holds data for an object to be rendered to the screen.
	/// </summary>
	class Mesh :
		public Object
	{
	public: // TODO: SET TO PRIVATE LATER
		size_t _vertexCount;
		size_t _vertexSize;
		VkBuffer _vertexBuffer;
		VkDeviceMemory _vertexMemory;

		size_t _indexCount;
		size_t _indexSize;
		VkBuffer _indexBuffer;
		VkDeviceMemory _indexMemory;
	public:
		//Mesh();

		void dispose(Renderer& engine);

		/// <summary>
		/// Sets the vertex data.
		/// </summary>
		/// <param name="vertices">The vertex data.</param>
		/// <param name="deviceSize">The size in bytes of the vertex data.</param>
		void set_vertices(void const* const vertices, size_t const elementCount, size_t const elementSize, Renderer& engine);

		/// <summary>
		/// Sets the index data.
		/// </summary>
		/// <param name="indices">The index data.</param>
		/// <param name="deviceSize">The size in bytes of the index data.</param>
		void set_indices(void const* const indices, size_t const elementCount, size_t const elementSize, Renderer& engine);

		/// <summary>
		/// Creates a cube mesh.
		/// </summary>
		/// <returns>A Mesh with cube data.</returns>
		static Mesh create_cube(Renderer& engine);
		
		/// <summary>
		/// Creates a quad mesh.
		/// </summary>
		/// <returns>A Mesh with quad data.</returns>
		static Mesh create_quad(Renderer& engine);
	private:
		void dispose_vertices(Renderer& engine);

		void dispose_indices(Renderer& engine);
	};
}

