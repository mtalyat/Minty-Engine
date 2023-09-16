#pragma once
#include "M_Object.h"

#include <vulkan/vulkan.h>

namespace minty
{
	class RenderEngine;

	class Texture :
		public Object
	{
	public: // TODO: TESTING, SET TO PRIVATE LATER
		VkImage image;
		VkDeviceMemory memory;
		VkImageView view;

	public:

		void dispose(RenderEngine& engine);

		/// <summary>
		/// Loads a texture from a file at the given path.
		/// </summary>
		/// <param name="path">An image file to load.</param>
		/// <returns>The loaded texture/</returns>
		static Texture load(std::string const& path, RenderEngine& engine);
	};
}

