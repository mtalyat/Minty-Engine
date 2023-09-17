#pragma once
#include "M_Object.h"

#include "M_Material.h"
#include <vulkan/vulkan.h>
#include <vector>

namespace minty
{
	class Renderer;

	class Mesh :
		public Object
	{
	public: // TODO: SET TO PRIVATE LATER
		bool _vertexSet;
		VkBuffer _vertexBuffer;
		VkDeviceMemory _vertexMemory;

		bool _indexSet;
		VkBuffer _indexBuffer;
		VkDeviceMemory _indexMemory;

		Material* _material;

	public:
		//Mesh();

		void dispose(Renderer& engine);

		/// <summary>
		/// Sets the vertex data.
		/// </summary>
		/// <param name="vertices">The vertex data.</param>
		/// <param name="deviceSize">The size in bytes of the vertex data.</param>
		void setVertices(void const* const vertices, size_t const deviceSize, Renderer& engine);

		/// <summary>
		/// Sets the index data.
		/// </summary>
		/// <param name="indices">The index data.</param>
		/// <param name="deviceSize">The size in bytes of the index data.</param>
		void setIndices(void const* const indices, size_t const deviceSize, Renderer& engine);

		void setMaterial(Material* const material);

		/// <summary>
		/// Creates a cube mesh.
		/// </summary>
		/// <returns>A Mesh with cube data.</returns>
		static Mesh createCube(Renderer& engine);
		
		/// <summary>
		/// Creates a quad mesh.
		/// </summary>
		/// <returns>A Mesh with quad data.</returns>
		static Mesh createQuad(Renderer& engine);
	private:
		void disposeVertices(Renderer& engine);

		void disposeIndices(Renderer& engine);
	};
}

