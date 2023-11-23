#pragma once
#include "M_Object.h"

#include "M_Register.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>

namespace minty
{
	class Renderer;
}

namespace minty::rendering
{
	class ShaderPassBuilder
		: public Object
	{
	public:
		struct ShaderStageInfo
		{
			VkShaderStageFlagBits stage;
			std::vector<char> code;
			std::string entry;
		};
	private:
		struct VertexInputAttribute
		{
			VkVertexInputAttributeDescription description;
			size_t size;
		};

	private:
		ID _shaderId;

		std::vector<ShaderStageInfo> _stages;

		VkPrimitiveTopology _topology;
		VkPolygonMode _polygonMode;
		VkCullModeFlags _cullMode;
		VkFrontFace _frontFace;
		float _lineWidth; // use if polygone mode != fill

		std::vector<VkVertexInputBindingDescription> _vertexBindings;
		std::vector<VertexInputAttribute> _vertexAttributes;

	public:
		ShaderPassBuilder(ID const shaderId);

#pragma region Set

		void emplace_stage(VkShaderStageFlagBits const stage, std::string const& path, Renderer const& renderer, std::string const entry = "main");

		void emplace_stage(VkShaderStageFlagBits const stage, std::vector<char> const& code, std::string const entry = "main");

		void set_topology(VkPrimitiveTopology const topology);

		/// <summary>
		/// Sets the polygon mode. This determines how the polygons are drawn. <para/> DEFAULT: FILL
		/// </summary>
		void set_polygon_mode(VkPolygonMode const polygonMode);

		void set_cull_mode(VkCullModeFlags const cullModeFlags);

		void set_front_face(VkFrontFace const frontFace);

		/// <summary>
		/// Sets the width of lines/points being drawn, if Polygon is set to LINE or POINT. <para/> DEFAULT: 1.0f
		/// </summary>
		/// <param name="width">The width of the lines to be drawn.</param>
		void set_line_width(float const width);

		void emplace_vertex_binding(uint32_t const binding, uint32_t const stride, VkVertexInputRate const inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX);

		void emplace_vertex_attribute(uint32_t const binding, uint32_t const location, VkDeviceSize const size, VkDeviceSize const offset, VkFormat const format);

#pragma endregion

#pragma region Get

		ID get_shader_id() const;

		std::vector<ShaderStageInfo> const& get_stages() const;

		VkPrimitiveTopology get_topology() const;

		VkPolygonMode get_polygon_mode() const;

		VkCullModeFlags get_cull_mode() const;

		VkFrontFace get_front_face() const;

		float get_line_width() const;

		std::vector<VkVertexInputBindingDescription> const& get_vertex_bindings() const;

		std::vector<VkVertexInputAttributeDescription> get_vertex_attributes() const;

#pragma endregion	
	};
}