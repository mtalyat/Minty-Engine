#pragma once
#include "Minty/Types/M_Object.h"

#include "Minty/Libraries/M_Vulkan.h"

namespace Minty
{
	class Viewport :
		public Object
	{
	private:
		VkViewport _view;
		VkRect2D _scissor;

	public:
		/// <summary>
		/// Creates an empty Viewport.
		/// </summary>
		Viewport();

		Viewport(float const x, float const y, float const width, float const height, float const minDepth, float const maxDepth);

		/// <summary>
		/// Creates a Viewport using the given position, size and depth.
		/// </summary>
		/// <param name="x">The X position of the Viewport.</param>
		/// <param name="y">The Y position of the Viewport.</param>
		/// <param name="width">The width of the Viewport.</param>
		/// <param name="height">The height of the Viewport.</param>
		/// <param name="minDepth">The minimum depth of the Viewport.</param>
		/// <param name="maxDepth">The maximum depth of the Viewport.</param>
		Viewport(int const x, int const y, unsigned int const width, unsigned int const height, float const minDepth, float const maxDepth);

		/// <summary>
		/// Binds this Viewport.
		/// </summary>
		/// <param name="commandBuffer">The CommandBuffer to use to bind this Viewport.</param>
		void bind(VkCommandBuffer const commandBuffer) const;

		/// <summary>
		/// Sets the offset for both the view and the scissor.
		/// </summary>
		/// <param name="x">The new X position.</param>
		/// <param name="y">The new Y position.</param>
		void set_offset(int const x, int const y);

		/// <summary>
		/// Sets the offset for the view.
		/// </summary>
		/// <param name="x">The new X position.</param>
		/// <param name="y">The new Y position.</param>
		void set_view_offset(int const x, int const y);

		/// <summary>
		/// Sets the offset for the scissor.
		/// </summary>
		/// <param name="x">The new X position.</param>
		/// <param name="y">The new Y position.</param>
		void set_scissor_offset(int const x, int const y);

		/// <summary>
		/// Sets the extent (size) for both the view and the scissor.
		/// </summary>
		/// <param name="width">The new width.</param>
		/// <param name="height">The new height.</param>
		void set_extent(unsigned int const width, unsigned int const height);

		/// <summary>
		/// Sets the extent (size) for the view.
		/// </summary>
		/// <param name="width">The new width.</param>
		/// <param name="height">The new height.</param>
		void set_view_extent(unsigned int const width, unsigned int const height);

		/// <summary>
		/// Sets the extent (size) for the scissor.
		/// </summary>
		/// <param name="width">The new width.</param>
		/// <param name="height">The new height.</param>
		void set_scissor_extent(unsigned int const width, unsigned int const height);

		VkViewport get_view() const { return _view; }

		VkRect2D get_scissor() const { return _scissor; }
	};
}

