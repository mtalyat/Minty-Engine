#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Audio/Attenuation.h"
#include "Minty/Core/Color.h"
#include "Minty/Core/Encoding.h"
#include "Minty/Core/Macros.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/String.h"
#include "Minty/Core/Type.h"
#include "Minty/Core/UUID.h"
#include "Minty/FSM/Conditional.h"
#include "Minty/Render/CoordinateMode.h"
#include "Minty/Render/Format.h"
#include "Minty/Render/Mesh.h"
#include "Minty/Render/Perspective.h"
#include "Minty/Render/Shader.h"
#include "Minty/UI/AnchorMode.h"

namespace Minty
{
	template<typename T>
	String to_string(const T& obj)
	{
		return std::to_string(obj);
	}

	template<>
	inline String to_string(const Float& obj)
	{
		String string = std::to_string(obj);

		// remove trailing zeros
		if (string.find('.') != std::string::npos)
		{
			// remove zeros
			string.erase(string.find_last_not_of('0') + 1, std::string::npos);

			// if last one is decimal place, remove that too
			if (string.back() == '.')
			{
				string.pop_back();
			}
		}

		return string;
	}

	template<>
	inline String to_string(const Double& obj)
	{
		String string = std::to_string(obj);

		// remove trailing zeros
		if (string.find('.') != std::string::npos)
		{
			// remove zeros
			string.erase(string.find_last_not_of('0') + 1, std::string::npos);

			// if last one is decimal place, remove that too
			if (string.back() == '.')
			{
				string.pop_back();
			}
		}

		return string;
	}

	template<>
	inline String to_string(const Bool& obj)
	{
		return obj ? "true" : "false";
	}

	template<>
	inline String to_string(const Char& obj)
	{
		return String(1, obj);
	}

	template<>
	inline String to_string(const UUID& obj)
	{
		std::stringstream ss;
		ss << std::hex << std::setfill('0') << std::setw(sizeof(uint64_t) * 2) << obj.data();
		return ss.str();
	}

	template<>
	inline String to_string(Type const& obj)
	{
		switch (obj)
		{
		case Type::UUID: return "UUID";
		case Type::Bool: return "Bool";
		case Type::Bool2: return "Bool2";
		case Type::Bool3: return "Bool3";
		case Type::Bool4: return "Bool4";
		case Type::Char: return "Char";
		case Type::Byte: return "Byte";
		case Type::Short: return "Short";
		case Type::UShort: return "UShort";
		case Type::Int: return "Int";
		case Type::Int2: return "Int2";
		case Type::Int3: return "Int3";
		case Type::Int4: return "Int4";
		case Type::UInt: return "UInt";
		case Type::UInt2: return "UInt2";
		case Type::UInt3: return "UInt3";
		case Type::UInt4: return "UInt4";
		case Type::Long: return "Long";
		case Type::ULong: return "ULong";
		case Type::Size: return "Size";
		case Type::Float: return "Float";
		case Type::Float2: return "Float2";
		case Type::Float3: return "Float3";
		case Type::Float4: return "Float4";
		case Type::Double: return "Double";
		case Type::String: return "String";
		case Type::Matrix2: return "Matrix2";
		case Type::Matrix3: return "Matrix3";
		case Type::Matrix4: return "Matrix4";
		case Type::Quaternion: return "Quaternion";
		case Type::Asset: return "Asset";
		case Type::Undefined: return "Undefined";

		default: return "";
		}
	}

	template<>
	inline String to_string(TypeID const& obj)
	{
		return obj.name();
	}

	template<>
	inline String to_string(TypeInfo const& obj)
	{
		return obj.name();
	}

	template<>
	inline String to_string(String const& obj)
	{
		return obj;
	}

	template<>
	inline String to_string(Bool2 const& obj)
	{
		return std::format("({}, {})", to_string(obj.x), to_string(obj.y));
	}

	template<>
	inline String to_string(Bool3 const& obj)
	{
		return std::format("({}, {}, {})", to_string(obj.x), to_string(obj.y), to_string(obj.z));
	}

	template<>
	inline String to_string(Bool4 const& obj)
	{
		return std::format("({}, {}, {}, {})", to_string(obj.x), to_string(obj.y), to_string(obj.z), to_string(obj.w));
	}

	template<>
	inline String to_string(Int2 const& obj)
	{
		return std::format("({}, {})", to_string(obj.x), to_string(obj.y));
	}

	template<>
	inline String to_string(Int3 const& obj)
	{
		return std::format("({}, {}, {})", to_string(obj.x), to_string(obj.y), to_string(obj.z));
	}

	template<>
	inline String to_string(Int4 const& obj)
	{
		return std::format("({}, {}, {}, {})", to_string(obj.x), to_string(obj.y), to_string(obj.z), to_string(obj.w));
	}

	template<>
	inline String to_string(UInt2 const& obj)
	{
		return std::format("({}, {})", to_string(obj.x), to_string(obj.y));
	}

	template<>
	inline String to_string(UInt3 const& obj)
	{
		return std::format("({}, {}, {})", to_string(obj.x), to_string(obj.y), to_string(obj.z));
	}

	template<>
	inline String to_string(UInt4 const& obj)
	{
		return std::format("({}, {}, {}, {})", to_string(obj.x), to_string(obj.y), to_string(obj.z), to_string(obj.w));
	}

	template<>
	inline String to_string(Float2 const& obj)
	{
		return std::format("({}, {})", to_string(obj.x), to_string(obj.y));
	}

	template<>
	inline String to_string(Float3 const& obj)
	{
		return std::format("({}, {}, {})", to_string(obj.x), to_string(obj.y), to_string(obj.z));
	}

	template<>
	inline String to_string(Float4 const& obj)
	{
		return std::format("({}, {}, {}, {})", to_string(obj.x), to_string(obj.y), to_string(obj.z), to_string(obj.w));
	}

	template<>
	inline String to_string(ShaderStage const& obj)
	{
		switch (obj)
		{
		case ShaderStage::Undefined: return "Undefined";
		case ShaderStage::Vertex: return "Vertex";
		case ShaderStage::Fragment: return "Fragment";

		default: return "";
		}
	}

	template<>
	inline String to_string(ShaderInputType const& obj)
	{
		switch (obj)
		{
		case ShaderInputType::Undefined: return "Undefined";
		case ShaderInputType::Sample: return "Sample";
		case ShaderInputType::CombinedImageSampler: return "CombinedImageSampler";
		case ShaderInputType::SampledImage: return "SampledImage";
		case ShaderInputType::StorageImage: return "StorageImage";
		case ShaderInputType::UniformTexelBuffer: return "UniformTexelBuffer";
		case ShaderInputType::StorageTexelBuffer: return "StorageTexelBuffer";
		case ShaderInputType::UniformBuffer: return "UniformBuffer";
		case ShaderInputType::StorageBuffer: return "StorageBuffer";
		case ShaderInputType::UniformBufferDynamic: return "UniformBufferDynamic";
		case ShaderInputType::StorageBufferDynamic: return "StorageBufferDynamic";
		case ShaderInputType::PushConstant: return "PushConstant";

		default: return "";
		}
	}

	template<>
	inline String to_string(Format const& obj)
	{
		switch (obj)
		{
		case Format::UNDEFINED: return "UNDEFINED";
		case Format::R4G4_UNORM_PACK8: return "R4G4_UNORM_PACK8";
		case Format::R4G4B4A4_UNORM_PACK16: return "R4G4B4A4_UNORM_PACK16";
		case Format::B4G4R4A4_UNORM_PACK16: return "B4G4R4A4_UNORM_PACK16";
		case Format::R5G6B5_UNORM_PACK16: return "R5G6B5_UNORM_PACK16";
		case Format::B5G6R5_UNORM_PACK16: return "B5G6R5_UNORM_PACK16";
		case Format::R5G5B5A1_UNORM_PACK16: return "R5G5B5A1_UNORM_PACK16";
		case Format::B5G5R5A1_UNORM_PACK16: return "B5G5R5A1_UNORM_PACK16";
		case Format::A1R5G5B5_UNORM_PACK16: return "A1R5G5B5_UNORM_PACK16";
		case Format::R8_UNORM: return "R8_UNORM";
		case Format::R8_SNORM: return "R8_SNORM";
		case Format::R8_USCALED: return "R8_USCALED";
		case Format::R8_SSCALED: return "R8_SSCALED";
		case Format::R8_UINT: return "R8_UINT";
		case Format::R8_SINT: return "R8_SINT";
		case Format::R8_SRGB: return "R8_SRGB";
		case Format::R8G8_UNORM: return "R8G8_UNORM";
		case Format::R8G8_SNORM: return "R8G8_SNORM";
		case Format::R8G8_USCALED: return "R8G8_USCALED";
		case Format::R8G8_SSCALED: return "R8G8_SSCALED";
		case Format::R8G8_UINT: return "R8G8_UINT";
		case Format::R8G8_SINT: return "R8G8_SINT";
		case Format::R8G8_SRGB: return "R8G8_SRGB";
		case Format::R8G8B8_UNORM: return "R8G8B8_UNORM";
		case Format::R8G8B8_SNORM: return "R8G8B8_SNORM";
		case Format::R8G8B8_USCALED: return "R8G8B8_USCALED";
		case Format::R8G8B8_SSCALED: return "R8G8B8_SSCALED";
		case Format::R8G8B8_UINT: return "R8G8B8_UINT";
		case Format::R8G8B8_SINT: return "R8G8B8_SINT";
		case Format::R8G8B8_SRGB: return "R8G8B8_SRGB";
		case Format::B8G8R8_UNORM: return "B8G8R8_UNORM";
		case Format::B8G8R8_SNORM: return "B8G8R8_SNORM";
		case Format::B8G8R8_USCALED: return "B8G8R8_USCALED";
		case Format::B8G8R8_SSCALED: return "B8G8R8_SSCALED";
		case Format::B8G8R8_UINT: return "B8G8R8_UINT";
		case Format::B8G8R8_SINT: return "B8G8R8_SINT";
		case Format::B8G8R8_SRGB: return "B8G8R8_SRGB";
		case Format::R8G8B8A8_UNORM: return "R8G8B8A8_UNORM";
		case Format::R8G8B8A8_SNORM: return "R8G8B8A8_SNORM";
		case Format::R8G8B8A8_USCALED: return "R8G8B8A8_USCALED";
		case Format::R8G8B8A8_SSCALED: return "R8G8B8A8_SSCALED";
		case Format::R8G8B8A8_UINT: return "R8G8B8A8_UINT";
		case Format::R8G8B8A8_SINT: return "R8G8B8A8_SINT";
		case Format::R8G8B8A8_SRGB: return "R8G8B8A8_SRGB";
		case Format::B8G8R8A8_UNORM: return "B8G8R8A8_UNORM";
		case Format::B8G8R8A8_SNORM: return "B8G8R8A8_SNORM";
		case Format::B8G8R8A8_USCALED: return "B8G8R8A8_USCALED";
		case Format::B8G8R8A8_SSCALED: return "B8G8R8A8_SSCALED";
		case Format::B8G8R8A8_UINT: return "B8G8R8A8_UINT";
		case Format::B8G8R8A8_SINT: return "B8G8R8A8_SINT";
		case Format::B8G8R8A8_SRGB: return "B8G8R8A8_SRGB";
		case Format::A8B8G8R8_UNORM_PACK32: return "A8B8G8R8_UNORM_PACK32";
		case Format::A8B8G8R8_SNORM_PACK32: return "A8B8G8R8_SNORM_PACK32";
		case Format::A8B8G8R8_USCALED_PACK32: return "A8B8G8R8_USCALED_PACK32";
		case Format::A8B8G8R8_SSCALED_PACK32: return "A8B8G8R8_SSCALED_PACK32";
		case Format::A8B8G8R8_UINT_PACK32: return "A8B8G8R8_UINT_PACK32";
		case Format::A8B8G8R8_SINT_PACK32: return "A8B8G8R8_SINT_PACK32";
		case Format::A8B8G8R8_SRGB_PACK32: return "A8B8G8R8_SRGB_PACK32";
		case Format::A2R10G10B10_UNORM_PACK32: return "A2R10G10B10_UNORM_PACK32";
		case Format::A2R10G10B10_SNORM_PACK32: return "A2R10G10B10_SNORM_PACK32";
		case Format::A2R10G10B10_USCALED_PACK32: return "A2R10G10B10_USCALED_PACK32";
		case Format::A2R10G10B10_SSCALED_PACK32: return "A2R10G10B10_SSCALED_PACK32";
		case Format::A2R10G10B10_UINT_PACK32: return "A2R10G10B10_UINT_PACK32";
		case Format::A2R10G10B10_SINT_PACK32: return "A2R10G10B10_SINT_PACK32";
		case Format::A2B10G10R10_UNORM_PACK32: return "A2B10G10R10_UNORM_PACK32";
		case Format::A2B10G10R10_SNORM_PACK32: return "A2B10G10R10_SNORM_PACK32";
		case Format::A2B10G10R10_USCALED_PACK32: return "A2B10G10R10_USCALED_PACK32";
		case Format::A2B10G10R10_SSCALED_PACK32: return "A2B10G10R10_SSCALED_PACK32";
		case Format::A2B10G10R10_UINT_PACK32: return "A2B10G10R10_UINT_PACK32";
		case Format::A2B10G10R10_SINT_PACK32: return "A2B10G10R10_SINT_PACK32";
		case Format::R16_UNORM: return "R16_UNORM";
		case Format::R16_SNORM: return "R16_SNORM";
		case Format::R16_USCALED: return "R16_USCALED";
		case Format::R16_SSCALED: return "R16_SSCALED";
		case Format::R16_UINT: return "R16_UINT";
		case Format::R16_SINT: return "R16_SINT";
		case Format::R16_SFLOAT: return "R16_SFLOAT";
		case Format::R16G16_UNORM: return "R16G16_UNORM";
		case Format::R16G16_SNORM: return "R16G16_SNORM";
		case Format::R16G16_USCALED: return "R16G16_USCALED";
		case Format::R16G16_SSCALED: return "R16G16_SSCALED";
		case Format::R16G16_UINT: return "R16G16_UINT";
		case Format::R16G16_SINT: return "R16G16_SINT";
		case Format::R16G16_SFLOAT: return "R16G16_SFLOAT";
		case Format::R16G16B16_UNORM: return "R16G16B16_UNORM";
		case Format::R16G16B16_SNORM: return "R16G16B16_SNORM";
		case Format::R16G16B16_USCALED: return "R16G16B16_USCALED";
		case Format::R16G16B16_SSCALED: return "R16G16B16_SSCALED";
		case Format::R16G16B16_UINT: return "R16G16B16_UINT";
		case Format::R16G16B16_SINT: return "R16G16B16_SINT";
		case Format::R16G16B16_SFLOAT: return "R16G16B16_SFLOAT";
		case Format::R16G16B16A16_UNORM: return "R16G16B16A16_UNORM";
		case Format::R16G16B16A16_SNORM: return "R16G16B16A16_SNORM";
		case Format::R16G16B16A16_USCALED: return "R16G16B16A16_USCALED";
		case Format::R16G16B16A16_SSCALED: return "R16G16B16A16_SSCALED";
		case Format::R16G16B16A16_UINT: return "R16G16B16A16_UINT";
		case Format::R16G16B16A16_SINT: return "R16G16B16A16_SINT";
		case Format::R16G16B16A16_SFLOAT: return "R16G16B16A16_SFLOAT";
		case Format::R32_UINT: return "R32_UINT";
		case Format::R32_SINT: return "R32_SINT";
		case Format::R32_SFLOAT: return "R32_SFLOAT";
		case Format::R32G32_UINT: return "R32G32_UINT";
		case Format::R32G32_SINT: return "R32G32_SINT";
		case Format::R32G32_SFLOAT: return "R32G32_SFLOAT";
		case Format::R32G32B32_UINT: return "R32G32B32_UINT";
		case Format::R32G32B32_SINT: return "R32G32B32_SINT";
		case Format::R32G32B32_SFLOAT: return "R32G32B32_SFLOAT";
		case Format::R32G32B32A32_UINT: return "R32G32B32A32_UINT";
		case Format::R32G32B32A32_SINT: return "R32G32B32A32_SINT";
		case Format::R32G32B32A32_SFLOAT: return "R32G32B32A32_SFLOAT";
		case Format::R64_UINT: return "R64_UINT";
		case Format::R64_SINT: return "R64_SINT";
		case Format::R64_SFLOAT: return "R64_SFLOAT";
		case Format::R64G64_UINT: return "R64G64_UINT";
		case Format::R64G64_SINT: return "R64G64_SINT";
		case Format::R64G64_SFLOAT: return "R64G64_SFLOAT";
		case Format::R64G64B64_UINT: return "R64G64B64_UINT";
		case Format::R64G64B64_SINT: return "R64G64B64_SINT";
		case Format::R64G64B64_SFLOAT: return "R64G64B64_SFLOAT";
		case Format::R64G64B64A64_UINT: return "R64G64B64A64_UINT";
		case Format::R64G64B64A64_SINT: return "R64G64B64A64_SINT";
		case Format::R64G64B64A64_SFLOAT: return "R64G64B64A64_SFLOAT";

		default: return "";
		}
	}

	template<>
	inline String to_string(ShaderPrimitiveTopology const& obj)
	{
		switch (obj)
		{
		case ShaderPrimitiveTopology::PointList: return "PointList";
		case ShaderPrimitiveTopology::LineList: return "LineList";
		case ShaderPrimitiveTopology::LineStrip: return "LineStrip";
		case ShaderPrimitiveTopology::TriangleList: return "TriangleList";
		case ShaderPrimitiveTopology::TriangleStrip: return "TriangleStrip";
		case ShaderPrimitiveTopology::TriangleFan: return "TriangleFan";

		default: return "";
		}
	}

	template<>
	inline String to_string(ShaderCullMode const& obj)
	{
		switch (obj)
		{
		case ShaderCullMode::None: return "None";
		case ShaderCullMode::Front: return "Front";
		case ShaderCullMode::Back: return "Back";
		case ShaderCullMode::Both: return "Both";

		default: return "";
		}
	}

	template<>
	inline String to_string(ShaderFrontFace const& obj)
	{
		switch (obj)
		{
		case ShaderFrontFace::Undefined: return "Undefined";
		case ShaderFrontFace::CounterClockwise: return "CounterClockwise";
		case ShaderFrontFace::Clockwise: return "Clockwise";

		default: return "";
		}
	}

	template<>
	inline String to_string(ShaderPolygonMode const& obj)
	{
		switch (obj)
		{
		case ShaderPolygonMode::Undefined: return "Undefined";
		case ShaderPolygonMode::Fill: return "Fill";
		case ShaderPolygonMode::Line: return "Line";
		case ShaderPolygonMode::Point: return "Point";

		default: return "";
		}
	}

	template<>
	inline String to_string(AssetType const& obj)
	{
		switch (obj)
		{
		case AssetType::None: return "None";
		case AssetType::Meta: return "Meta";
		case AssetType::Wrap: return "Wrap";
		case AssetType::Text: return "Text";
		case AssetType::Script: return "Script";
		case AssetType::AudioClip: return "AudioClip";
		case AssetType::Animation: return "Animation";
		case AssetType::Animator: return "Animator";
		case AssetType::Texture: return "Texture";
		case AssetType::ShaderCode: return "ShaderCode";
		case AssetType::ShaderModule: return "ShaderModule";
		case AssetType::Shader: return "Shader";
		//case AssetType::ShaderPass: return "ShaderPass";
		case AssetType::MaterialTemplate: return "MaterialTemplate";
		case AssetType::Material: return "Material";
		case AssetType::Sprite: return "Sprite";
		case AssetType::FontVariant: return "FontVariant";
		case AssetType::Font: return "Font";
		case AssetType::Mesh: return "Mesh";
		case AssetType::Scene: return "Scene";

		default: return "";
		}
	}

	template<>
	inline String to_string(Path const& path)
	{
		String string = path.generic_string();

		// if a space is in the path, add double quotes
		if (string.find(' ') != std::string::npos)
		{
			return std::format("\"{}\"", string);
		}

		// normal is fine
		return string;
	}

	template<>
	inline String to_string(MeshType const& obj)
	{
		switch (obj)
		{
		case MeshType::Empty: return "Empty";
		case MeshType::Custom: return "Custom";
		case MeshType::Quad: return "Quad";
		case MeshType::Cube: return "Cube";
		case MeshType::Pyramid: return "Pyramid";
		case MeshType::Sphere: return "Sphere";
		case MeshType::Cylinder: return "Cylinder";

		default: return "";
		}
	}

	template<>
	inline String to_string(AnchorMode const& obj)
	{
		if (obj == AnchorMode::All)
		{
			return "All";
		}

		String string = "";

		if (static_cast<Bool>(obj & AnchorMode::Top))
		{
			string += "Top";
		}
		else if (static_cast<Bool>(obj & AnchorMode::Middle))
		{
			string += "Middle";
		}
		else if (static_cast<Bool>(obj & AnchorMode::Bottom))
		{
			string += "Bottom";
		}

		if (static_cast<Bool>(obj & AnchorMode::Left))
		{
			string += "Left";
		}
		else if (static_cast<Bool>(obj & AnchorMode::Center))
		{
			string += "Center";
		}
		else if (static_cast<Bool>(obj & AnchorMode::Right))
		{
			string += "Right";
		}

		return string;
	}

	template<>
	inline String to_string(Perspective const& obj)
	{
		switch (obj)
		{
		case Perspective::Perspective: return "Perspective";
		case Perspective::Orthographic: return "Orthographic";

		default: return "";
		}
	}

	template<>
	inline String to_string(Color const& color)
	{
		if (color == Color::red())
		{
			return "Red";
		}
		else if (color == Color::green())
		{
			return "Green";
		}
		else if (color == Color::blue())
		{
			return "Blue";
		}
		else if (color == Color::white())
		{
			return "White";
		}
		else if (color == Color::black())
		{
			return "Black";
		}
		else if (color == Color::yellow())
		{
			return "Yellow";
		}
		else if (color == Color::cyan())
		{
			return "Cyan";
		}
		else if (color == Color::magenta())
		{
			return "Magenta";
		}
		else
		{
			Int value = static_cast<Int>(color);
			return std::format("#{}", Encoding::encode_base16(value));
		}
	}

	template<>
	inline String to_string(Conditional const& obj)
	{
		switch (obj)
		{
		case Conditional::Equal: return "==";
		case Conditional::NotEqual: return "!=";
		case Conditional::GreaterThan: return ">";
		case Conditional::GreaterThanOrEqualTo: return ">=";
		case Conditional::LessThan: return "<";
		case Conditional::LessThanOrEqualTo: return "<=";

		default: return "";
		}
	}

	template<>
	inline String to_string(Attenuation const& obj)
	{
		switch (obj)
		{
		case Attenuation::None: return "None";
		case Attenuation::InverseDistance: return "InverseDistance";
		case Attenuation::LinearDistance: return "LinearDistance";
		case Attenuation::ExponentialDistance: return "ExponentialDistance";

		default: return "";
		}
	}

	template<>
	inline String to_string(ShaderInputRate const& obj)
	{
		switch (obj)
		{
		case ShaderInputRate::Undefined: return "Undefined";
		case ShaderInputRate::Vertex: return "Vertex";
		case ShaderInputRate::Instance: return "Instance";

		default: return "";
		}
	}

	template<>
	inline String to_string(CoordinateMode const& obj)
	{
		switch (obj)
		{
		case CoordinateMode::Normalized: return "Normalized";
		case CoordinateMode::Pixel: return "Pixel";

		default: return "";
		}
	}
}