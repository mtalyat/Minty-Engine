#pragma once
#include "M_Rendering_Object.h"

#include "M_Rendering_TextureBuilder.h"
#include "M_Rendering_PixelFormat.h"
#include <vulkan/vulkan.h>

namespace minty
{
	/// <summary>
	/// Holds data for an image on the GPU.
	/// </summary>
	class Texture :
		public rendering::RenderObject
	{
	private:
		int _width;
		int _height;
		VkFormat _format;
		VkImage _image;
		VkImageView _view;
		VkDeviceMemory _memory;
		VkSampler _sampler;

	public:
		/// <summary>
		/// Creates a new Texture.
		/// </summary>
		/// <param name="builder"></param>
		/// <param name="renderer"></param>
		Texture(rendering::TextureBuilder const& builder, RenderEngine& renderer);

		/// <summary>
		/// Destroys all of the resources associated with this Texture.
		/// </summary>
		void destroy();

		/// <summary>
		/// Gets the width of this Texture in pixels.
		/// </summary>
		/// <returns></returns>
		int get_width() const;

		/// <summary>
		/// Gets the height of this Texture in pixels.
		/// </summary>
		/// <returns></returns>
		int get_height() const;

		/// <summary>
		/// Gets the format of this Texture.
		/// </summary>
		/// <returns></returns>
		VkFormat get_format() const;

		/// <summary>
		/// Gets the image of this Texture.
		/// </summary>
		/// <returns></returns>
		VkImage get_image() const;

		/// <summary>
		/// Gets the image view of this Texture.
		/// </summary>
		/// <returns></returns>
		VkImageView get_image_view() const;

		/// <summary>
		/// Gets the device memory of this Texture.
		/// </summary>
		/// <returns></returns>
		VkDeviceMemory get_device_memory() const;

		/// <summary>
		/// Gets the sampler of this Texture.
		/// </summary>
		/// <returns></returns>
		VkSampler get_sampler() const;
		
	public:
		friend String to_string(Texture const& value);
	};
}

