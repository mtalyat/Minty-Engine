#pragma once
#include "M_Object.h"

#include "M_Texture.h"
#include "M_Shader.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <array>

namespace minty
{
	class Renderer;

	class Material :
		public Object
	{
	public: // TODO: TEMP PUBLIC
		ID const _shaderId;
		ID _textureId;

	public:
		Material(ID const shaderId, ID const textureId = -1);

		void setTexture(ID const textureId);

		void dispose(Renderer& engine);
	};
}