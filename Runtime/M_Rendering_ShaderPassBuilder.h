#pragma once
#include "M_Base.h"

#include "M_Register.h"

#include "vulkan.h"
#include <vector>
#include <unordered_map>

namespace minty::rendering
{
	struct ShaderPassBuilder
	{
		struct ShaderStageInfo
		{
			VkShaderStageFlagBits stage;
			std::vector<char> code;
			std::string entry;
		};

		std::string name;
		ID shaderId;

		VkPrimitiveTopology topology;
		VkPolygonMode polygonMode;
		VkCullModeFlags cullMode;
		VkFrontFace frontFace;
		float lineWidth; // use if polygon mode != fill

		std::vector<VkVertexInputBindingDescription> vertexBindings;
		std::vector<VkVertexInputAttributeDescription> vertexAttributes;

		std::vector<ShaderStageInfo> stages;
	};
}