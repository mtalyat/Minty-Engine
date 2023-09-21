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
		ID const shaderID;
		ID textureID;
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