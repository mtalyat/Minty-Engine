#pragma once
#include "M_Rendering_Object.h"

#include "M_Rendering_MaterialBuilder.h"

#include "M_Color.h"
#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace minty
{
	class Shader;

	/// <summary>
	/// Holds graphics information.
	/// </summary>
	class Material
		: public rendering::RendererObject
	{
	private:
		ID _shaderId;

		ID _id;

		void* _data;
	public:
		Material(ID const id, void const* const data, rendering::MaterialBuilder const& builder, Renderer& renderer);

		void destroy();

		ID get_shader_id() const;

		void set(void const* const data);

		void* get() const;

		friend std::string to_string(Material const& value);
	};
}