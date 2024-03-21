#pragma once

#include "M_Object.h"
#include "M_Vulkan.h"

namespace minty
{
	/// <summary>
	/// Holds info for a push constant.
	/// </summary>
	struct PushConstantInfo
		: public Object
	{
		/// <summary>
		/// The name of the push constant.
		/// </summary>
		String name;
		
		/// <summary>
		/// The shader stage this push constant is in.
		/// </summary>
		VkShaderStageFlagBits stageFlags;

		/// <summary>
		/// The offset of this push constant in bytes.
		/// </summary>
		uint32_t offset;

		/// <summary>
		/// The size of this push constant in bytes.
		/// </summary>
		uint32_t size;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}