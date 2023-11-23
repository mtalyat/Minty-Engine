#pragma once
#include "M_Rendering_Object.h"

#include "M_Rendering_Buffer.h"
#include "M_Rendering_MaterialBuilder.h"
#include "M_Rendering_DescriptorSet.h"

#include "M_Color.h"
#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace minty
{
	namespace rendering
	{
		class MaterialBuilder;
	}

	/// <summary>
	/// Holds graphics information.
	/// </summary>
	class Material
		: public rendering::RendererObject
	{
	private:
		ID _templateId;
		std::vector<rendering::DescriptorSet> _passDescriptorSets;

	public:
		Material(rendering::MaterialBuilder const& builder, Renderer& renderer);

		void destroy();

		ID get_template_id() const;

		rendering::DescriptorSet const& get_descriptor_set(uint32_t const pass) const;

	public:
		friend std::string to_string(Material const& value);
	};
}