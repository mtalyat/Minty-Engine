#pragma once

#include "M_Base.h"

namespace minty::rendering
{
	/// <summary>
	/// Holds info for a push constant.
	/// </summary>
	struct PushConstantInfo
	{
		/// <summary>
		/// The name of the push constant.
		/// </summary>
		std::string name;
		
		/// <summary>
		/// The shader stage this push constant is in.
		/// </summary>
		VkShaderStageFlags stageFlags;

		/// <summary>
		/// The offset of this push constant in bytes.
		/// </summary>
		uint32_t offset;

		/// <summary>
		/// The size of this push constant in bytes.
		/// </summary>
		uint32_t size;
	};
}