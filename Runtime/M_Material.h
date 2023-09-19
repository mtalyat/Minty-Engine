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

	class Material :
		public Object
	{
	public: // TODO: TEMP PUBLIC
		ID const _shaderId;
		ID _textureId;
		Color _color;

		std::vector<VkBuffer> _buffers;
		std::vector<VkDeviceMemory> _memories;
		std::vector<void*> _mapped;
	public:
		Material(ID const shaderId, ID const textureId = -1, Color const color = Color::white());

		void setTexture(ID const textureId);

		void setColor(Color const color);

		Color getColor() const;

		void apply() const;

		void dispose(Renderer& renderer);
	};
}