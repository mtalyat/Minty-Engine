#pragma once
#include "M_Object.h"

#include "M_Texture.h"
#include "M_Shader.h"
#include "M_Color.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <array>

namespace minty
{
	class Renderer;

	/// <summary>
	/// Holds graphics information.
	/// </summary>
	struct Material :
		public Object
	{
		/// <summary>
		/// The ID of the Shader that this Material uses.
		/// </summary>
		ID const shaderID;

		/// <summary>
		/// The ID of the Texture that this Material uses.
		/// </summary>
		ID textureID;

		/// <summary>
		/// The color of this Material.
		/// </summary>
		Color color;

		/// <summary>
		/// Creates a new Material that corresponds to the given Shader ID.
		/// </summary>
		/// <param name="shaderId">The shader ID.</param>
		/// <param name="textureId">The inital Texture ID used for this Material.</param>
		/// <param name="color">The initial Color used for this Material.</param>
		Material(ID const shaderId, ID const textureID = -1, Color const color = Color::white());
	};
}