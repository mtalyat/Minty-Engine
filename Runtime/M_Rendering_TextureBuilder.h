#pragma once
#include "M_Base.h"

#include "M_Color.h"
#include "M_Rendering_PixelFormat.h"
#include "vulkan.h"

namespace minty::rendering
{
	/// <summary>
	/// Holds data to create a new Texture.
	/// </summary>
	struct TextureBuilder
	{
		/// <summary>
		/// The name of the Texure.
		/// </summary>
		String name;
		/// <summary>
		/// The path to the Texture on the disk.
		/// </summary>
		Path path;
		/// <summary>
		/// The raw pixel data for the Texture.
		/// </summary>
		Color* pixelData;
		/// <summary>
		/// The width of the Texture in pixels.
		/// </summary>
		int width;
		/// <summary>
		/// The height of the Texture in pixels.
		/// </summary>
		int height;
		/// <summary>
		/// The format to read the pixels in.
		/// </summary>
		PixelFormat pixelFormat;
		/// <summary>
		/// The filter for the Texture. How to get the color when between pixels.
		/// </summary>
		VkFilter filter;
		/// <summary>
		/// The format for the Texture. How the pixels are interpreted.
		/// </summary>
		VkFormat format;
		/// <summary>
		/// The address mode for the Texture. What to do when the texcoord is outside of [0.0, 1.0].
		/// </summary>
		VkSamplerAddressMode addressMode;
		/// <summary>
		/// The mipmap mode for the Texture. 
		/// </summary>
		VkSamplerMipmapMode mipmapMode;
	};
}

