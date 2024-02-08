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
	/// Holds the graphics information template data for a Material.
	/// </summary>
	class MaterialTemplate
		: public RenderObject
	{
	private:
		// the shader pass IDs for the material
		std::vector<ID> _shaderPassIds;
		// the default values for a new material
		std::unordered_map<String, Dynamic> _defaultValues;

	public:
		/// <summary>
		/// Creates an empty MaterialTemplate.
		/// </summary>
		MaterialTemplate();

		/// <summary>
		/// Creates a new MaterialTemplate.
		/// </summary>
		/// <param name="builder"></param>
		/// <param name="renderer"></param>
		MaterialTemplate(MaterialTemplateBuilder const& builder, Engine& engine, ID const sceneId);

		/// <summary>
		/// Destroys all of the resources associated with this MaterialTemplate.
		/// </summary>
		void destroy();

		/// <summary>
		/// Gets the list of ShaderPass IDs.
		/// </summary>
		/// <returns></returns>
		std::vector<ID> const& get_shader_pass_ids() const;

		/// <summary>
		/// Gets the default value for the value with the given name.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		Dynamic get_defalt_value(String const& name) const;

		/// <summary>
		/// Gets all the default values.
		/// </summary>
		/// <returns></returns>
		std::unordered_map<String, Dynamic> const& get_default_values() const;

		friend String to_string(MaterialTemplate const& value);
	};
}