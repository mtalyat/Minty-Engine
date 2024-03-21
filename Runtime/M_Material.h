#pragma once
#include "M_Asset.h"

#include "M_DescriptorSet.h"
#include <vector>

namespace minty
{
	class MaterialTemplate;

	/// <summary>
	/// Holds data to create a new Material.
	/// </summary>
	struct MaterialBuilder
	{
		UUID id;

		Path path;

		/// <summary>
		/// The MaterialTemplate.
		/// </summary>
		MaterialTemplate* materialTemplate;

		/// <summary>
		/// The values to set.
		/// </summary>
		std::unordered_map<String, Dynamic> values;
	};

	/// <summary>
	/// Holds graphics information.
	/// </summary>
	class Material
		: public Asset
	{
	private:
		// the material template id
		MaterialTemplate* _template;
		std::vector<DescriptorSet> _passDescriptorSets;

	public:
		/// <summary>
		/// Creates an empty Material.
		/// </summary>
		Material();

		/// <summary>
		/// Creates a new Material.
		/// </summary>
		/// <param name="builder"></param>
		/// <param name="renderer"></param>
		Material(MaterialBuilder const& builder, Runtime& engine);

		~Material();

		/// <summary>
		/// Destroys all of the resources associated with this Material.
		/// </summary>
		void destroy();

		/// <summary>
		/// Gets the MaterialTemplate ID.
		/// </summary>
		/// <returns></returns>
		MaterialTemplate* get_template() const;

		/// <summary>
		/// Gets the DescriptorSet for the given ShaderPass index.
		/// </summary>
		/// <param name="pass"></param>
		/// <returns></returns>
		DescriptorSet const& get_descriptor_set(uint32_t const pass) const;

	public:
		friend String to_string(Material const& value);
	};
}