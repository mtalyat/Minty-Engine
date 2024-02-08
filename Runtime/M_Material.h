#pragma once
#include "M_Object.h"

#include "M_Buffer.h"
#include "M_MaterialBuilder.h"
#include "M_DescriptorSet.h"

#include "M_Color.h"
#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace minty
{
	/// <summary>
	/// Holds graphics information.
	/// </summary>
	class Material
		: public RenderObject
	{
	private:
		// the material template id
		ID _templateId;
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
		Material(MaterialBuilder const& builder, Engine& engine, ID const sceneId);

		/// <summary>
		/// Destroys all of the resources associated with this Material.
		/// </summary>
		void destroy();

		/// <summary>
		/// Gets the MaterialTemplate ID.
		/// </summary>
		/// <returns></returns>
		ID get_template_id() const;

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