#pragma once
#include "M_Base.h"

#include "M_Register.h"

#include "vulkan.h"
#include <vector>
#include <unordered_map>

namespace minty
{
	/// <summary>
	/// Holds data to create a new ShaderPass.
	/// </summary>
	struct ShaderPassBuilder
	{
		/// <summary>
		/// Holds info for a stage in the shader.
		/// </summary>
		struct ShaderStageInfo
		{
			VkShaderStageFlagBits stage;
			std::vector<char> code;
			String entry;
		};

		/// <summary>
		/// The name of the ShaderPass.
		/// </summary>
		String name;

		/// <summary>
		/// The ID of the Shader.
		/// </summary>
		ID shaderId;

		/// <summary>
		/// The topology for this ShaderPass. The type of primitive shapes that the GPU interprets the vertex data as (points, lines, triangles...)
		/// </summary>
		VkPrimitiveTopology topology;
		/// <summary>
		/// The polygon mode for this ShaderPass. The way each primitive shape is drawn (line, triangle...)
		/// </summary>
		VkPolygonMode polygonMode;
		/// <summary>
		/// The cull mode for this ShaderPass. The side that will not be drawn.
		/// </summary>
		VkCullModeFlags cullMode;
		/// <summary>
		/// The front face for this ShaderPass. The direction vertices must travel for it to be considered a front side.
		/// </summary>
		VkFrontFace frontFace;
		/// <summary>
		/// The width of the line if the polygon mode is not fill.
		/// </summary>
		float lineWidth;

		/// <summary>
		/// The vertex bindings.
		/// </summary>
		std::vector<VkVertexInputBindingDescription> vertexBindings;
		/// <summary>
		/// The vertex attributes.
		/// </summary>
		std::vector<VkVertexInputAttributeDescription> vertexAttributes;

		/// <summary>
		/// The shader stages.
		/// </summary>
		std::vector<ShaderStageInfo> stages;
	};
}