#pragma once
#include "M_Asset.h"

#include "M_Dynamic.h"
#include <unordered_map>
#include <vector>

namespace minty
{
	class Shader;
	class ShaderPass;

	/// <summary>
	/// Holds data to create a new MaterialTemplate.
	/// </summary>
	struct MaterialTemplateBuilder
	{
		UUID id;

		Path path;

		std::vector<ShaderPass*> shaderPasses;

		/// <summary>
		/// The default values.
		/// </summary>
		std::unordered_map<String, Dynamic> defaultValues;
	};

	/// <summary>
	/// Holds the graphics information template data for a Material.
	/// </summary>
	class MaterialTemplate
		: public Asset
	{
	private:
		// the shader pass IDs for the material
		std::vector<ShaderPass*> _shaderPasses;
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
		MaterialTemplate(MaterialTemplateBuilder const& builder, Runtime& engine);

		/// <summary>
		/// Destroys all of the resources associated with this MaterialTemplate.
		/// </summary>
		void destroy();

		/// <summary>
		/// Gets the list of ShaderPass IDs.
		/// </summary>
		/// <returns></returns>
		std::vector<ShaderPass*> const& get_shader_passes() const;

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