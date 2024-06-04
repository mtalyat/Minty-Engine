#pragma once
#include "Minty/Assets/M_Asset.h"

#include "Minty/Types/M_Color.h"
#include "Minty/Rendering/M_PixelFormat.h"
#include "Minty/Libraries/M_Vulkan.h"

namespace Minty
{
	/// <summary>
	/// Holds data to create a new Texture.
	/// </summary>
	struct TextureBuilder
	{
		UUID id = UUID::create();

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

	/// <summary>
	/// Holds data for an image on the GPU.
	/// </summary>
	class Texture :
		public Asset
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
		/// Creates an empty Texture.
		/// </summary>
		Texture() = default;

		/// <summary>
		/// Creates a new Texture.
		/// </summary>
		/// <param name="builder"></param>
		/// <param name="renderer"></param>
		Texture(TextureBuilder const& builder);

		~Texture();

		/// <summary>
		/// Destroys all of the resources associated with this Texture.
		/// </summary>
		void destroy();

		/// <summary>
		/// Gets the width of this Texture in pixels.
		/// </summary>
		/// <returns></returns>
		int get_width() const { return _width; }

		/// <summary>
		/// Gets the height of this Texture in pixels.
		/// </summary>
		/// <returns></returns>
		int get_height() const { return _height; }

		/// <summary>
		/// Gets the format of this Texture.
		/// </summary>
		/// <returns></returns>
		VkFormat get_format() const { return _format; }

		/// <summary>
		/// Gets the image of this Texture.
		/// </summary>
		/// <returns></returns>
		VkImage get_image() const { return _image; }

		/// <summary>
		/// Gets the image view of this Texture.
		/// </summary>
		/// <returns></returns>
		VkImageView get_image_view() const { return _view; }

		/// <summary>
		/// Gets the device memory of this Texture.
		/// </summary>
		/// <returns></returns>
		VkDeviceMemory get_device_memory() const { return _memory; }

		/// <summary>
		/// Gets the sampler of this Texture.
		/// </summary>
		/// <returns></returns>
		VkSampler get_sampler() const { return _sampler; }
	};
}

