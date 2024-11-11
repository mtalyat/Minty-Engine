#pragma once
#include "Minty/Asset/Asset.h"

#include "Minty/Data/Container.h"
#include "Minty/Data/Cargo.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/Types.h"
#include "Minty/Render/Format.h"
#include "Minty/Render/Scissor.h"
#include "Minty/Render/ShaderModule.h"
#include "Minty/Render/Viewport.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Minty
{
	class Material;

	enum class ShaderStage
	{
		Undefined,
		Vertex,
		Fragment
	};

	enum class ShaderInputType
	{
		Undefined,
		Sample,
		CombinedImageSampler,
		SampledImage,
		StorageImage,
		UniformTexelBuffer,
		StorageTexelBuffer,
		UniformBuffer,
		StorageBuffer,
		UniformBufferDynamic,
		StorageBufferDynamic,
		PushConstant, // push constant
	};

	enum class ShaderPrimitiveTopology
	{
		Undefined,
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip,
		TriangleFan,
	};

	enum class ShaderCullMode
	{
		None,
		Front,
		Back,
		Both,
	};

	enum class ShaderFrontFace
	{
		Undefined,
		CounterClockwise,
		Clockwise,
	};

	enum class ShaderPolygonMode
	{
		Undefined,
		Fill,
		Line,
		Point,
	};

	enum class ShaderInputRate
	{
		Undefined,
		Vertex,
		Instance
	};

	struct ShaderInput
	{
		UInt binding = 0;
		ShaderStage stage; // stage this descriptor is in
		ShaderInputType type; // type of this descriptor
		UInt count = 1; // count, if there is an array, otherwise 1
		Bool frequent = false; // if true, the uniform is updated once a frame, otherwise, only periodically
		String name = ""; // the name of the descriptor
		Cargo cargo = {};
		UInt size = 0; // size in bytes
		UInt offset = 0; // offset in bytes
	};

	/// <summary>
	/// Represents an attribute (in variable) within a vertex Shader.
	/// </summary>
	struct ShaderAttribute
	{
		UInt location; // location
		Type type; // type determines size
		// offset determined from location and size from types
	};

	struct ShaderBinding
	{
		UInt binding; // index of binding
		ShaderInputRate rate; // vertex or instance
		std::vector<ShaderAttribute> attributes; // individual attributes
	};

	struct ShaderVertexInput
	{
		std::vector<ShaderBinding> bindings; // each binding
	};

	struct ShaderBuilder
	{
		UUID id = {};

		Ref<ShaderModule> fragmentShaderModule;
		Ref<ShaderModule> vertexShaderModule;

		Ref<Viewport> viewport;
		Ref<Scissor> scissor;

		ShaderPrimitiveTopology primitiveTopology = ShaderPrimitiveTopology::TriangleList;
		ShaderPolygonMode polygonMode = ShaderPolygonMode::Fill;
		ShaderFrontFace frontFace = ShaderFrontFace::Clockwise;
		ShaderCullMode cullMode = ShaderCullMode::None;
		Float lineWidth = 1.0f;

		ShaderVertexInput vertexInput; // vertex data
		std::vector<ShaderInput> inputs; // constant data
	};

	/// <summary>
	/// Represents a Shader on the GPU.
	/// </summary>
	class Shader
		: public Asset
	{
	private:
		Ref<Viewport> m_viewport;
		Ref<Scissor> m_scissor;

		std::unordered_map<String, ShaderInput> m_inputs;
		std::unordered_set<Material*> m_materials;

	protected:
		Shader(const ShaderBuilder& builder)
			: Asset(builder.id)
			, m_viewport(builder.viewport)
			, m_scissor(builder.scissor)
			, m_inputs()
			, m_materials()
		{
			// copy inputs into map
			for (ShaderInput const& input : builder.inputs)
			{
				m_inputs.emplace(input.name, input);
			}
		}

	public:
		virtual ~Shader() = default;

	public:
		virtual void on_bind() = 0;

	public:
		Ref<Viewport> get_viewport() const { return m_viewport; }

		Ref<Scissor> get_scissor() const { return m_scissor; }

		void set_viewport(const Ref<Viewport> viewport) { m_viewport = viewport; }

		void set_scissor(const Ref<Scissor> scissor) { m_scissor = scissor; }

		Bool has_input(String const& name) const { return m_inputs.contains(name); }

		ShaderInput const& get_input(String const& name) const { return m_inputs.at(name); }

		std::vector<ShaderInput> get_inputs() const;

		/// <summary>
		/// Sets the input for all Materials associated with this Shader.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="data"></param>
		void set_global_input(String const& name, void const* const data);

		void register_material(Material& material);
		void unregister_material(Material& material);

	public:
		AssetType get_type() const override { return AssetType::Shader; }

	public:
		static Owner<Shader> create(const ShaderBuilder& builder = {});
	};
}