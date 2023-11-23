#pragma once
#include "M_Rendering_Object.h"

#include "M_Rendering_MaterialTemplateBuilder.h"

#include "M_Dynamic.h"

#include <vulkan/vulkan.h>
#include <unordered_map>
#include <vector>

namespace minty
{
	class Shader;

	/// <summary>
	/// Holds graphics information.
	/// </summary>
	class MaterialTemplate
		: public rendering::RendererObject
	{
	private:
		std::vector<ID> _shaderPassIds;
		std::unordered_map<std::string, Dynamic> _defaultValues;

	public:
		MaterialTemplate(rendering::MaterialTemplateBuilder const& builder, Renderer& renderer);

		void destroy();

		std::vector<ID> const& get_shader_pass_ids() const;

		Dynamic get_defalt_value(std::string const& name) const;

		std::unordered_map<std::string, Dynamic> const& get_default_values() const;

		friend std::string to_string(MaterialTemplate const& value);
	};
}