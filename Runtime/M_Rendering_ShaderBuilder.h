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
			uint32_t count;
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

		size_t _materialSize;

	public:
		ShaderBuilder();

#pragma region Set

		template<class T>
		void set_material_type();

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

		size_t get_material_size() const;

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

		void emplace_uniform_constant(std::string const& name, VkShaderStageFlags const stageFlags, uint32_t const set, uint32_t const binding, VkDescriptorType const type, VkDeviceSize const size, uint32_t const count);

		void emplace_uniform_constant(std::string const& name, VkShaderStageFlags const stageFlags, uint32_t const set, uint32_t const binding, VkDescriptorType const type, VkDeviceSize const size, std::vector<ID> const& ids);

#pragma endregion
	};

	template<class T>
	void ShaderBuilder::set_material_type()
	{
		_materialSize = sizeof(T);
	}

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
		// get offset, which is the combined sizes of the attributes w the same binding
		uint32_t offset = 0;

		for (auto const& data : _vertexAttributes)
		{
			if (data.description.binding == binding)
			{
				offset += data.size;
			}
		}

		VkVertexInputAttributeDescription desc =
		{
			.location = static_cast<uint32_t>(_vertexAttributes.size()),
			.binding = binding,
			.format = format,
			.offset = offset,
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
}