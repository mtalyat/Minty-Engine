#pragma once
#include "M_Object.h"

#include "M_Register.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>

namespace minty::rendering
{
	class ShaderBuilder :
		public Object
	{
	public:
		struct UniformConstantInfo
		{
			VkDescriptorType type;
			std::string name;
			uint32_t set;
			uint32_t binding;
			uint32_t descriptorCount;
			VkDeviceSize size;
			VkShaderStageFlags stageFlags;
			std::vector<ID> ids;
		};

		struct PushConstantInfo
		{
			std::string name;
			VkPushConstantRange range;
		};
	private:
		struct VertexInputAttribute
		{
			VkVertexInputAttributeDescription description;
			size_t size;
		};

		std::string _vertexEnterPoint; // main
		std::string _fragmentEnterPoint; // main

		VkPrimitiveTopology _topology;
		VkPolygonMode _polygonMode;
		VkCullModeFlags _cullMode;
		VkFrontFace _frontFace;
		float _lineWidth; // use if polygone mode != fill

		std::vector<VkVertexInputBindingDescription> _vertexBindings;
		std::vector<VertexInputAttribute> _vertexAttributes;

		std::vector<PushConstantInfo> _pushConstants;
		std::vector<UniformConstantInfo> _uniformConstants;

	public:
		ShaderBuilder();

#pragma region Set

		void set_vertex_enter_point(std::string const& enterPoint);

		void set_fragment_enter_point(std::string const& enterPoint);

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

#pragma endregion

#pragma region Get

		std::string const& get_vertex_enter_point() const;

		std::string const& get_fragment_enter_point() const;

		VkPrimitiveTopology get_topology() const;

		VkPolygonMode get_polygon_mode() const;

		VkCullModeFlags get_cull_mode() const;

		VkFrontFace get_front_face() const;

		float get_line_width() const;

		std::vector<VkVertexInputBindingDescription> const& get_vertex_bindings() const;

		std::vector<VkVertexInputAttributeDescription> get_vertex_attributes() const;

		std::vector<PushConstantInfo> const& get_push_constant_infos() const;

		uint32_t get_descriptor_set_layout_count() const;

		std::vector<VkDescriptorSetLayoutBinding> get_descriptor_set_layout_bindings(uint32_t const set) const;

		std::vector<UniformConstantInfo> get_uniform_constant_infos(uint32_t const set) const;

		uint32_t get_uniform_constant_count() const;

		uint32_t get_uniform_constant_count(uint32_t const set) const;

#pragma endregion

#pragma region Emplace

		template<class T>
		void emplace_vertex_binding(uint32_t const binding, VkVertexInputRate const inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX);

		template<class T>
		void emplace_vertex_attribute(uint32_t const binding, VkFormat const format);

		template<class T>
		void emplace_push_constant(std::string const& name, VkShaderStageFlags const stageFlags);

		template<class T>
		void emplace_uniform_constant(std::string const& name, VkShaderStageFlags const stageFlags, uint32_t const set, uint32_t const binding, VkDescriptorType const type, uint32_t const descriptorCount = 1);

		template<class T>
		void emplace_uniform_constant(std::string const& name, VkShaderStageFlags const stageFlags, uint32_t const set, uint32_t const binding, VkDescriptorType const type, std::vector<ID> const& ids);

#pragma endregion
	};

	template<class T>
	void ShaderBuilder::emplace_vertex_binding(uint32_t const binding, VkVertexInputRate const inputRate)
	{
		_vertexBindings.push_back(VkVertexInputBindingDescription
			{
				.binding = binding,
				.stride = sizeof(T),
				.inputRate = inputRate,
			});
	}
	template<class T>
	void ShaderBuilder::emplace_vertex_attribute(uint32_t const binding, VkFormat const format)
	{
		VkVertexInputAttributeDescription desc =
		{
			.location = static_cast<uint32_t>(_vertexAttributes.size()),
			.binding = binding,
			.format = format,
			.offset = 0,
		};

		_vertexAttributes.push_back(VertexInputAttribute
			{
				.description = desc,
				.size = sizeof(T),
			});
	}

	template<class T>
	void ShaderBuilder::emplace_push_constant(std::string const& name, VkShaderStageFlags const stageFlags)
	{
		// get offset using previous element
		uint32_t offset = 0;

		if (_pushConstants.size())
		{
			offset = _pushConstants.back().range.offset + _pushConstants.back().range.size;
		}

		VkPushConstantRange range
		{
			.stageFlags = stageFlags,
			.offset = offset,
			.size = sizeof(T),
		};

		return _pushConstants.push_back(PushConstantInfo
			{
				.name = name,
				.range = range
			});
	}

	template<class T>
	void ShaderBuilder::emplace_uniform_constant(std::string const& name, VkShaderStageFlags const stageFlags, uint32_t const set, uint32_t const binding, VkDescriptorType const type, uint32_t const descriptorCount)
	{
		_uniformConstants.push_back(UniformConstantInfo
			{
				.type = type,
				.name = name,
				.set = set,
				.binding = binding,
				.descriptorCount = descriptorCount,
				.size = static_cast<VkDeviceSize>(sizeof(T) * descriptorCount),
				.stageFlags = stageFlags,
				.ids = {},
			});
	}

	template<class T>
	void ShaderBuilder::emplace_uniform_constant(std::string const& name, VkShaderStageFlags const stageFlags, uint32_t const set, uint32_t const binding, VkDescriptorType const type, std::vector<ID> const& ids)
	{
		_uniformConstants.push_back(UniformConstantInfo
			{
				.type = type,
				.name = name,
				.set = set,
				.binding = binding,
				.descriptorCount = static_cast<uint32_t>(ids.size()),
				.size = static_cast<VkDeviceSize>(sizeof(T) * ids.size()),
				.stageFlags = stageFlags,
				.ids = ids,
			});
	}
}