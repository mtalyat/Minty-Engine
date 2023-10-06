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
		friend class MaterialInstance;

	private:
		ID _shaderId;

		std::unordered_map<std::string, MaterialData> _values;

	public:
		/// <summary>
		/// Creates a new Material that corresponds to the given Shader ID.
		/// </summary>
		/// <param name="shaderId">The shader ID.</param>
		/// <param name="textureId">The inital Texture ID used for this Material.</param>
		/// <param name="color">The initial Color used for this Material.</param>
		Material(rendering::MaterialBuilder const& builder, Renderer& renderer);

		~Material();

		void set(std::string const& name, void* const value, size_t const size);

		ID get_shader_id() const;

		void bind(VkCommandBuffer const commandBuffer) const;
	};
}