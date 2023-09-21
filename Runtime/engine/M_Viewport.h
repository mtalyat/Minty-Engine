#pragma once
#include "M_Object.h"

#include <vulkan/vulkan.h>

namespace minty
{
	struct Viewport :
		public Object
	{
		VkViewport view;
		VkRect2D scissor;

		/// <summary>
		/// Creates an empty Viewport.
		/// </summary>
		Viewport();

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
		/// Sets the offset for both the view and the scissor.
		/// </summary>
		/// <param name="x">The new X position.</param>
		/// <param name="y">The new Y position.</param>
		void setOffset(int const x, int const y);

		/// <summary>
		/// Sets the offset for the view.
		/// </summary>
		/// <param name="x">The new X position.</param>
		/// <param name="y">The new Y position.</param>
		void setViewOffset(int const x, int const y);

		/// <summary>
		/// Sets the offset for the scissor.
		/// </summary>
		/// <param name="x">The new X position.</param>
		/// <param name="y">The new Y position.</param>
		void setScissorOffset(int const x, int const y);

		/// <summary>
		/// Sets the extent (size) for both the view and the scissor.
		/// </summary>
		/// <param name="width">The new width.</param>
		/// <param name="height">The new height.</param>
		void setExtent(unsigned int const width, unsigned int const height);

		/// <summary>
		/// Sets the extent (size) for the view.
		/// </summary>
		/// <param name="width">The new width.</param>
		/// <param name="height">The new height.</param>
		void setViewExtent(unsigned int const width, unsigned int const height);

		/// <summary>
		/// Sets the extent (size) for the scissor.
		/// </summary>
		/// <param name="width">The new width.</param>
		/// <param name="height">The new height.</param>
		void setScissorExtent(unsigned int const width, unsigned int const height);
	};
}

