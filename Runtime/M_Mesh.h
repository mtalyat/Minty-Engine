#pragma once
#include "M_Object.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace minty
{
	class RenderEngine;

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

	public:
		//Mesh();

		/// <summary>
		/// Sets the vertex data.
		/// </summary>
		/// <param name="vertices">The vertex data.</param>
		/// <param name="deviceSize">The size in bytes of the vertex data.</param>
		void setVertices(void const* const vertices, size_t const deviceSize, RenderEngine& engine);

		/// <summary>
		/// Sets the index data.
		/// </summary>
		/// <param name="indices">The index data.</param>
		/// <param name="deviceSize">The size in bytes of the index data.</param>
		void setIndices(void const* const indices, size_t const deviceSize, RenderEngine& engine);
		
		void dispose(RenderEngine& engine);
	private:
		void disposeVertices(RenderEngine& engine);

		void disposeIndices(RenderEngine& engine);
	};
}
