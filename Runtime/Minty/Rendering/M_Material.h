#pragma once
#include "Minty/Assets/M_Asset.h"

#include "Minty/Rendering/M_DescriptorSet.h"
#include <vector>

namespace Minty
{
	class MaterialTemplate;

	/// <summary>
	/// Holds data to create a new Material.
	/// </summary>
	struct MaterialBuilder
	{
		UUID id = UUID::create();

		Path path;

		/// <summary>
		/// The MaterialTemplate.
		/// </summary>
		Ref<MaterialTemplate> materialTemplate;

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
		Ref<MaterialTemplate> _template;
		std::vector<DescriptorSet> _passDescriptorSets;

	public:
		/// <summary>
		/// Creates an empty Material.
		/// </summary>
		Material() = default;

		/// <summary>
		/// Creates a new Material.
		/// </summary>
		/// <param name="builder"></param>
		/// <param name="renderer"></param>
		Material(MaterialBuilder const& builder);

		~Material();

		/// <summary>
		/// Destroys all of the resources associated with this Material.
		/// </summary>
		void destroy();

		/// <summary>
		/// Gets the MaterialTemplate ID.
		/// </summary>
		/// <returns></returns>
		Ref<MaterialTemplate> get_template() const { return _template; }

		/// <summary>
		/// Gets the DescriptorSet for the given ShaderPass index.
		/// </summary>
		/// <param name="pass"></param>
		/// <returns></returns>
		DescriptorSet const& get_descriptor_set(uint32_t const pass) const;
	};
}