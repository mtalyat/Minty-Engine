#pragma once
#include "M_Asset.h"

#include "M_DescriptorSet.h"

#include "M_Vulkan.h"

namespace minty
{
	class Shader;

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

		UUID id;

		Path path;

		Shader* shader;

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
	
	/// <summary>
	/// Holds data for a ShaderPass.
	/// </summary>
	class ShaderPass
		: public Asset
	{
	private:
		Shader* _shader;

		VkPipeline _pipeline;

		DescriptorSet _descriptorSet;

	public:
		/// <summary>
		/// Creates an empty ShaderPass.
		/// </summary>
		ShaderPass();

		/// <summary>
		/// Creates a new ShaderPass.
		/// </summary>
		/// <param name="builder"></param>
		/// <param name="renderer"></param>
		ShaderPass(ShaderPassBuilder const& builder, Runtime& engine);

		/// <summary>
		/// Destroys all of the resources associated with this ShaderPass.
		/// </summary>
		void destroy();

		/// <summary>
		/// Gets the Shader ID for this ShaderPass.
		/// </summary>
		/// <returns></returns>
		Shader* get_shader() const;

		/// <summary>
		/// Gets the pipeline for this ShaderPass.
		/// </summary>
		/// <returns></returns>
		VkPipeline get_pipeline() const;

		/// <summary>
		/// Gets the descriptor set for this ShaderPass.
		/// </summary>
		/// <returns></returns>
		DescriptorSet const& get_descriptor_set() const;

	private:
		// creates the graphics pipeline for this ShaderPass
		void create_pipeline(ShaderPassBuilder const& builder);
	};
}