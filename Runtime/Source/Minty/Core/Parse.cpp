#include "pch.h"
#include "Parse.h"

#include "Minty/Core/String.h"
#include "Minty/Core/Encoding.h"

using namespace Minty;
using namespace Minty::Parse;

Bool is_unsigned_integer(String const& string)
{
	if (!string.length()) return false; // nothing in string

	for (Char const c : string)
	{
		if (!isdigit(c)) return false; // not a digit
	}

	// all digits
	return true;
}

Bool is_signed_integer(String const& string)
{
	if (!string.length()) return false; // nothing in string

	Size i = string.at(0) == '-' ? 1 : 0;

	// check if just the '-'
	if (i == 1 && string.length() == 1) return false;

	Char c;
	for (; i < string.size(); i++)
	{
		c = string.at(i);

		if (!isdigit(c)) return false; // not a digit
	}

	// all digits or - at beginning
	return true;
}

Bool is_float(String const& string)
{
	if (!string.length()) return false; // nothing in string

	Char c;
	Bool decimal = false;
	Bool digit = false;
	for (Size i = 0; i < string.size(); i++)
	{
		c = string.at(i);

		switch (c)
		{
		case '-':
			if (i > 0) return false; // - not at beginning
			break;
		case '.':
			if (decimal) return false; // second .
			decimal = true;
			break;
		default:
			if (!isdigit(c)) return false; // not a digit
			digit = true;
			break;
		}
	}

	// can only be a number if there was a digit
	return digit;
}

Bool Minty::Parse::to_bool(String const& string)
{
	// check for "true" or "false" as well
	String upper = to_upper(string);
	if (upper == "TRUE")
	{
		return true;
	}
	else if (upper == "FALSE")
	{
		return false;
	}

	// by default, parse the Int to a Bool
	return static_cast<Bool>(std::stoi(string));
}

Bool Minty::Parse::try_bool(String const& string, Bool& value)
{
	// check for "true" or "false" as well
	String upper = to_upper(string);
	if (upper == "TRUE")
	{
		return true;
	}
	else if (upper == "FALSE")
	{
		return false;
	}

	// by default, parse the Int to a Bool
	if (is_signed_integer(string))
	{
		value = static_cast<Bool>(std::stoi(string));
		return true;
	}

	return false;
}
Bool2 Minty::Parse::to_bool2(String const& string)
{
	std::istringstream ss(string);
	char discard;
	Bool2 result{};
	ss >> discard >> result.x >> discard >> result.y >> discard;
	return result;
}

Bool Minty::Parse::try_bool2(String const& string, Bool2& value)
{
	std::istringstream ss(string);
	char discard;
	Bool2 result{};
	if (ss >> discard >> result.x >> discard >> result.y >> discard)
	{
		value = result;
		return true;
	}
	return false;
}

Bool3 Minty::Parse::to_bool3(String const& string)
{
	std::istringstream ss(string);
	char discard;
	Bool3 result{};
	ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard;
	return result;
}

Bool Minty::Parse::try_bool3(String const& string, Bool3& value)
{
	std::istringstream ss(string);
	char discard;
	Bool3 result{};
	if (ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard)
	{
		value = result;
		return true;
	}
	return false;
}

Bool4 Minty::Parse::to_bool4(String const& string)
{
	std::istringstream ss(string);
	char discard;
	Bool4 result{};
	ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard >> result.w >> discard;
	return result;
}

Bool Minty::Parse::try_bool4(String const& string, Bool4& value)
{
	std::istringstream ss(string);
	char discard;
	Bool4 result{};
	if (ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard >> result.w >> discard)
	{
		value = result;
		return true;
	}
	return false;
}

Byte Minty::Parse::to_byte(String const& string)
{
	return static_cast<Byte>(std::stoi(string));
}

Bool Minty::Parse::try_byte(String const& string, Byte& value)
{
	if (is_unsigned_integer(string))
	{
		value = static_cast<Byte>(std::stoi(string));
		return true;
	}

	return false;
}

Char Minty::Parse::to_char(String const& string)
{
	return static_cast<Char>(std::stoi(string));
}

Bool Minty::Parse::try_char(String const& string, Char& value)
{
	if (is_unsigned_integer(string))
	{
		value = static_cast<Char>(std::stoi(string));
		return true;
	}

	return false;
}

Short Minty::Parse::to_short(String const& string)
{
	return static_cast<Short>(std::stoi(string));
}

Bool Minty::Parse::try_short(String const& string, Short& value)
{
	if (is_signed_integer(string))
	{
		value = static_cast<Short>(std::stoi(string));
		return true;
	}

	return false;
}

UShort Minty::Parse::to_ushort(String const& string)
{
	return static_cast<UShort>(std::stoi(string));
}

Bool Minty::Parse::try_ushort(String const& string, UShort& value)
{
	if (is_signed_integer(string))
	{
		value = static_cast<UShort>(std::stoi(string));
		return true;
	}

	return false;
}

Int Minty::Parse::to_int(String const& string)
{
	return std::stoi(string);
}

Bool Minty::Parse::try_int(String const& string, Int& value)
{
	if (is_signed_integer(string))
	{
		value = std::stoi(string);
		return true;
	}

	return false;
}

Int2 Minty::Parse::to_int2(String const& string)
{
	std::istringstream ss(string);
	char discard;
	Int2 result{};
	ss >> discard >> result.x >> discard >> result.y >> discard;
	return result;
}

Bool Minty::Parse::try_int2(String const& string, Int2& value)
{
	std::istringstream ss(string);
	char discard;
	Int2 result{};
	if (ss >> discard >> result.x >> discard >> result.y >> discard)
	{
		value = result;
		return true;
	}
	return false;
}

Int3 Minty::Parse::to_int3(String const& string)
{
	std::istringstream ss(string);
	char discard;
	Int3 result{};
	ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard;
	return result;
}

Bool Minty::Parse::try_int3(String const& string, Int3& value)
{
	std::istringstream ss(string);
	char discard;
	Int3 result{};
	if (ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard)
	{
		value = result;
		return true;
	}
	return false;
}

Int4 Minty::Parse::to_int4(String const& string)
{
	std::istringstream ss(string);
	char discard;
	Int4 result{};
	ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard >> result.w >> discard;
	return result;
}

Bool Minty::Parse::try_int4(String const& string, Int4& value)
{
	std::istringstream ss(string);
	char discard;
	Int4 result{};
	if (ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard >> result.w >> discard)
	{
		value = result;
		return true;
	}
	return false;
}

UInt Minty::Parse::to_uint(String const& string)
{
	return static_cast<UInt>(std::stoul(string));
}

Bool Minty::Parse::try_uint(String const& string, UInt& value)
{
	if (is_unsigned_integer(string))
	{
		value = static_cast<UInt>(std::stoul(string));
		return true;
	}

	return false;
}

UInt2 Minty::Parse::to_uint2(String const& string)
{
	std::istringstream ss(string);
	char discard;
	UInt2 result{};
	ss >> discard >> result.x >> discard >> result.y >> discard;
	return result;
}

Bool Minty::Parse::try_uint2(String const& string, UInt2& value)
{
	std::istringstream ss(string);
	char discard;
	UInt2 result{};
	if (ss >> discard >> result.x >> discard >> result.y >> discard)
	{
		value = result;
		return true;
	}
	return false;
}

UInt3 Minty::Parse::to_uint3(String const& string)
{
	std::istringstream ss(string);
	char discard;
	UInt3 result{};
	ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard;
	return result;
}

Bool Minty::Parse::try_uint3(String const& string, UInt3& value)
{
	std::istringstream ss(string);
	char discard;
	UInt3 result{};
	if (ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard)
	{
		value = result;
		return true;
	}
	return false;
}

UInt4 Minty::Parse::to_uint4(String const& string)
{
	std::istringstream ss(string);
	char discard;
	UInt4 result{};
	ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard >> result.w >> discard;
	return result;
}

Bool Minty::Parse::try_uint4(String const& string, UInt4& value)
{
	std::istringstream ss(string);
	char discard;
	UInt4 result{};
	if (ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard >> result.w >> discard)
	{
		value = result;
		return true;
	}
	return false;
}

Long Minty::Parse::to_long(String const& string)
{
	return std::stol(string);
}

Bool Minty::Parse::try_long(String const& string, Long& value)
{
	if (is_signed_integer(string))
	{
		value = std::stol(string);
		return true;
	}

	return false;
}

ULong Minty::Parse::to_ulong(String const& string)
{
	return std::stoul(string);
}

Bool Minty::Parse::try_ulong(String const& string, ULong& value)
{
	if (is_signed_integer(string))
	{
		value = std::stoul(string);
		return true;
	}

	return false;
}

Float Minty::Parse::to_float(String const& string)
{
	return std::stof(string);
}

Bool Minty::Parse::try_float(String const& string, Float& value)
{
	if (is_float(string))
	{
		value = std::stof(string);
		return true;
	}

	return false;
}

Float2 Minty::Parse::to_float2(String const& string)
{
	std::istringstream ss(string);
	char discard;
	Float2 result{};
	ss >> discard >> result.x >> discard >> result.y >> discard;
	return result;
}

Bool Minty::Parse::try_float2(String const& string, Float2& value)
{
	std::istringstream ss(string);
	char discard;
	Float2 result{};
	if (ss >> discard >> result.x >> discard >> result.y >> discard)
	{
		value = result;
		return true;
	}
	return false;
}

Float3 Minty::Parse::to_float3(String const& string)
{
	std::istringstream ss(string);
	char discard;
	Float3 result{};
	ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard;
	return result;
}

Bool Minty::Parse::try_float3(String const& string, Float3& value)
{
	std::istringstream ss(string);
	char discard;
	Float3 result{};
	if (ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard)
	{
		value = result;
		return true;
	}
	return false;
}

Float4 Minty::Parse::to_float4(String const& string)
{
	std::istringstream ss(string);
	char discard;
	Float4 result{};
	ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard >> result.w >> discard;
	return result;
}

Bool Minty::Parse::try_float4(String const& string, Float4& value)
{
	std::istringstream ss(string);
	char discard;
	Float4 result{};
	if (ss >> discard >> result.x >> discard >> result.y >> discard >> result.z >> discard >> result.w >> discard)
	{
		value = result;
		return true;
	}
	return false;
}

Double Minty::Parse::to_double(String const& string)
{
	return std::stod(string);
}

Bool Minty::Parse::try_double(String const& string, Double& value)
{
	if (is_float(string))
	{
		value = std::stod(string);
		return true;
	}

	return false;
}

Size Minty::Parse::to_size(String const& string)
{
	return std::stoull(string);
}

Bool Minty::Parse::try_size(String const& string, Size& value)
{
	if (is_unsigned_integer(string))
	{
		value = std::stoull(string);
		return true;
	}

	return false;
}

UUID Minty::Parse::to_uuid(String const& string)
{
	if (string.empty()) return INVALID_UUID;

	Size id;
	std::stringstream ss(string);
	ss >> std::hex >> id;
	return UUID(id);
}

Bool Minty::Parse::try_uuid(String const& string, UUID& value)
{
	// UUIDs stored as base16
	if (Encoding::is_base16(string))
	{
		value = to_uuid(string);
		return true;
	}

	return false;
}

Type Minty::Parse::to_type(String const& string)
{
	if (string == "UUID") return Type::UUID;
	if (string == "Bool") return Type::Bool;
	if (string == "Bool2") return Type::Bool2;
	if (string == "Bool3") return Type::Bool3;
	if (string == "Bool4") return Type::Bool4;
	if (string == "Char") return Type::Char;
	if (string == "Byte") return Type::Byte;
	if (string == "Short") return Type::Short;
	if (string == "UShort") return Type::UShort;
	if (string == "Int") return Type::Int;
	if (string == "Int2") return Type::Int2;
	if (string == "Int3") return Type::Int3;
	if (string == "Int4") return Type::Int4;
	if (string == "UInt") return Type::UInt;
	if (string == "UInt2") return Type::UInt2;
	if (string == "UInt3") return Type::UInt3;
	if (string == "UInt4") return Type::UInt4;
	if (string == "Long") return Type::Long;
	if (string == "ULong") return Type::ULong;
	if (string == "Size") return Type::Size;
	if (string == "Float") return Type::Float;
	if (string == "Float2") return Type::Float2;
	if (string == "Float3") return Type::Float3;
	if (string == "Float4") return Type::Float4;
	if (string == "Double") return Type::Double;
	if (string == "String") return Type::String;
	if (string == "Matrix2") return Type::Matrix2;
	if (string == "Matrix3") return Type::Matrix3;
	if (string == "Matrix4") return Type::Matrix4;
	if (string == "Quaternion") return Type::Quaternion;
	if (string == "Asset") return Type::Asset;
	if (string == "Undefined") return Type::Undefined;

	return Type();
}

Bool Minty::Parse::try_type(String const& string, Type& value)
{
	value = to_type(string);
	return value != Type();
}

ShaderStage Minty::Parse::to_shader_stage(String const& string)
{
	if (string == "Undefined") return ShaderStage::Undefined;
	if (string == "Vertex") return ShaderStage::Vertex;
	if (string == "Fragment") return ShaderStage::Fragment;

	return ShaderStage();
}

Bool Minty::Parse::try_shader_stage(String const& string, ShaderStage& value)
{
	value = to_shader_stage(string);
	return value != ShaderStage();
}

ShaderInputType Minty::Parse::to_shader_descriptor_type(String const& string)
{
	if (string == "Undefined") return ShaderInputType::Undefined;
	if (string == "Sample") return ShaderInputType::Sample;
	if (string == "CombinedImageSampler") return ShaderInputType::CombinedImageSampler;
	if (string == "SampledImage") return ShaderInputType::SampledImage;
	if (string == "StorageImage") return ShaderInputType::StorageImage;
	if (string == "UniformTexelBuffer") return ShaderInputType::UniformTexelBuffer;
	if (string == "StorageTexelBuffer") return ShaderInputType::StorageTexelBuffer;
	if (string == "UniformBuffer") return ShaderInputType::UniformBuffer;
	if (string == "StorageBuffer") return ShaderInputType::StorageBuffer;
	if (string == "UniformBufferDynamic") return ShaderInputType::UniformBufferDynamic;
	if (string == "StorageBufferDynamic") return ShaderInputType::StorageBufferDynamic;
	if (string == "PushConstant") return ShaderInputType::PushConstant;

	return ShaderInputType();
}

Bool Minty::Parse::try_shader_descriptor_type(String const& string, ShaderInputType& value)
{
	value = to_shader_descriptor_type(string);
	return value != ShaderInputType();
}

Format Minty::Parse::to_format(String const& string)
{
	if (string == "UNDEFINED") return Format::UNDEFINED;
	if (string == "R4G4_UNORM_PACK8") return Format::R4G4_UNORM_PACK8;
	if (string == "R4G4B4A4_UNORM_PACK16") return Format::R4G4B4A4_UNORM_PACK16;
	if (string == "B4G4R4A4_UNORM_PACK16") return Format::B4G4R4A4_UNORM_PACK16;
	if (string == "R5G6B5_UNORM_PACK16") return Format::R5G6B5_UNORM_PACK16;
	if (string == "B5G6R5_UNORM_PACK16") return Format::B5G6R5_UNORM_PACK16;
	if (string == "R5G5B5A1_UNORM_PACK16") return Format::R5G5B5A1_UNORM_PACK16;
	if (string == "B5G5R5A1_UNORM_PACK16") return Format::B5G5R5A1_UNORM_PACK16;
	if (string == "A1R5G5B5_UNORM_PACK16") return Format::A1R5G5B5_UNORM_PACK16;
	if (string == "R8_UNORM") return Format::R8_UNORM;
	if (string == "R8_SNORM") return Format::R8_SNORM;
	if (string == "R8_USCALED") return Format::R8_USCALED;
	if (string == "R8_SSCALED") return Format::R8_SSCALED;
	if (string == "R8_UINT") return Format::R8_UINT;
	if (string == "R8_SINT") return Format::R8_SINT;
	if (string == "R8_SRGB") return Format::R8_SRGB;
	if (string == "R8G8_UNORM") return Format::R8G8_UNORM;
	if (string == "R8G8_SNORM") return Format::R8G8_SNORM;
	if (string == "R8G8_USCALED") return Format::R8G8_USCALED;
	if (string == "R8G8_SSCALED") return Format::R8G8_SSCALED;
	if (string == "R8G8_UINT") return Format::R8G8_UINT;
	if (string == "R8G8_SINT") return Format::R8G8_SINT;
	if (string == "R8G8_SRGB") return Format::R8G8_SRGB;
	if (string == "R8G8B8_UNORM") return Format::R8G8B8_UNORM;
	if (string == "R8G8B8_SNORM") return Format::R8G8B8_SNORM;
	if (string == "R8G8B8_USCALED") return Format::R8G8B8_USCALED;
	if (string == "R8G8B8_SSCALED") return Format::R8G8B8_SSCALED;
	if (string == "R8G8B8_UINT") return Format::R8G8B8_UINT;
	if (string == "R8G8B8_SINT") return Format::R8G8B8_SINT;
	if (string == "R8G8B8_SRGB") return Format::R8G8B8_SRGB;
	if (string == "B8G8R8_UNORM") return Format::B8G8R8_UNORM;
	if (string == "B8G8R8_SNORM") return Format::B8G8R8_SNORM;
	if (string == "B8G8R8_USCALED") return Format::B8G8R8_USCALED;
	if (string == "B8G8R8_SSCALED") return Format::B8G8R8_SSCALED;
	if (string == "B8G8R8_UINT") return Format::B8G8R8_UINT;
	if (string == "B8G8R8_SINT") return Format::B8G8R8_SINT;
	if (string == "B8G8R8_SRGB") return Format::B8G8R8_SRGB;
	if (string == "R8G8B8A8_UNORM") return Format::R8G8B8A8_UNORM;
	if (string == "R8G8B8A8_SNORM") return Format::R8G8B8A8_SNORM;
	if (string == "R8G8B8A8_USCALED") return Format::R8G8B8A8_USCALED;
	if (string == "R8G8B8A8_SSCALED") return Format::R8G8B8A8_SSCALED;
	if (string == "R8G8B8A8_UINT") return Format::R8G8B8A8_UINT;
	if (string == "R8G8B8A8_SINT") return Format::R8G8B8A8_SINT;
	if (string == "R8G8B8A8_SRGB") return Format::R8G8B8A8_SRGB;
	if (string == "B8G8R8A8_UNORM") return Format::B8G8R8A8_UNORM;
	if (string == "B8G8R8A8_SNORM") return Format::B8G8R8A8_SNORM;
	if (string == "B8G8R8A8_USCALED") return Format::B8G8R8A8_USCALED;
	if (string == "B8G8R8A8_SSCALED") return Format::B8G8R8A8_SSCALED;
	if (string == "B8G8R8A8_UINT") return Format::B8G8R8A8_UINT;
	if (string == "B8G8R8A8_SINT") return Format::B8G8R8A8_SINT;
	if (string == "B8G8R8A8_SRGB") return Format::B8G8R8A8_SRGB;
	if (string == "A8B8G8R8_UNORM_PACK32") return Format::A8B8G8R8_UNORM_PACK32;
	if (string == "A8B8G8R8_SNORM_PACK32") return Format::A8B8G8R8_SNORM_PACK32;
	if (string == "A8B8G8R8_USCALED_PACK32") return Format::A8B8G8R8_USCALED_PACK32;
	if (string == "A8B8G8R8_SSCALED_PACK32") return Format::A8B8G8R8_SSCALED_PACK32;
	if (string == "A8B8G8R8_UINT_PACK32") return Format::A8B8G8R8_UINT_PACK32;
	if (string == "A8B8G8R8_SINT_PACK32") return Format::A8B8G8R8_SINT_PACK32;
	if (string == "A8B8G8R8_SRGB_PACK32") return Format::A8B8G8R8_SRGB_PACK32;
	if (string == "A2R10G10B10_UNORM_PACK32") return Format::A2R10G10B10_UNORM_PACK32;
	if (string == "A2R10G10B10_SNORM_PACK32") return Format::A2R10G10B10_SNORM_PACK32;
	if (string == "A2R10G10B10_USCALED_PACK32") return Format::A2R10G10B10_USCALED_PACK32;
	if (string == "A2R10G10B10_SSCALED_PACK32") return Format::A2R10G10B10_SSCALED_PACK32;
	if (string == "A2R10G10B10_UINT_PACK32") return Format::A2R10G10B10_UINT_PACK32;
	if (string == "A2R10G10B10_SINT_PACK32") return Format::A2R10G10B10_SINT_PACK32;
	if (string == "A2B10G10R10_UNORM_PACK32") return Format::A2B10G10R10_UNORM_PACK32;
	if (string == "A2B10G10R10_SNORM_PACK32") return Format::A2B10G10R10_SNORM_PACK32;
	if (string == "A2B10G10R10_USCALED_PACK32") return Format::A2B10G10R10_USCALED_PACK32;
	if (string == "A2B10G10R10_SSCALED_PACK32") return Format::A2B10G10R10_SSCALED_PACK32;
	if (string == "A2B10G10R10_UINT_PACK32") return Format::A2B10G10R10_UINT_PACK32;
	if (string == "A2B10G10R10_SINT_PACK32") return Format::A2B10G10R10_SINT_PACK32;
	if (string == "R16_UNORM") return Format::R16_UNORM;
	if (string == "R16_SNORM") return Format::R16_SNORM;
	if (string == "R16_USCALED") return Format::R16_USCALED;
	if (string == "R16_SSCALED") return Format::R16_SSCALED;
	if (string == "R16_UINT") return Format::R16_UINT;
	if (string == "R16_SINT") return Format::R16_SINT;
	if (string == "R16_SFLOAT") return Format::R16_SFLOAT;
	if (string == "R16G16_UNORM") return Format::R16G16_UNORM;
	if (string == "R16G16_SNORM") return Format::R16G16_SNORM;
	if (string == "R16G16_USCALED") return Format::R16G16_USCALED;
	if (string == "R16G16_SSCALED") return Format::R16G16_SSCALED;
	if (string == "R16G16_UINT") return Format::R16G16_UINT;
	if (string == "R16G16_SINT") return Format::R16G16_SINT;
	if (string == "R16G16_SFLOAT") return Format::R16G16_SFLOAT;
	if (string == "R16G16B16_UNORM") return Format::R16G16B16_UNORM;
	if (string == "R16G16B16_SNORM") return Format::R16G16B16_SNORM;
	if (string == "R16G16B16_USCALED") return Format::R16G16B16_USCALED;
	if (string == "R16G16B16_SSCALED") return Format::R16G16B16_SSCALED;
	if (string == "R16G16B16_UINT") return Format::R16G16B16_UINT;
	if (string == "R16G16B16_SINT") return Format::R16G16B16_SINT;
	if (string == "R16G16B16_SFLOAT") return Format::R16G16B16_SFLOAT;
	if (string == "R16G16B16A16_UNORM") return Format::R16G16B16A16_UNORM;
	if (string == "R16G16B16A16_SNORM") return Format::R16G16B16A16_SNORM;
	if (string == "R16G16B16A16_USCALED") return Format::R16G16B16A16_USCALED;
	if (string == "R16G16B16A16_SSCALED") return Format::R16G16B16A16_SSCALED;
	if (string == "R16G16B16A16_UINT") return Format::R16G16B16A16_UINT;
	if (string == "R16G16B16A16_SINT") return Format::R16G16B16A16_SINT;
	if (string == "R16G16B16A16_SFLOAT") return Format::R16G16B16A16_SFLOAT;
	if (string == "R32_UINT") return Format::R32_UINT;
	if (string == "R32_SINT") return Format::R32_SINT;
	if (string == "R32_SFLOAT") return Format::R32_SFLOAT;
	if (string == "R32G32_UINT") return Format::R32G32_UINT;
	if (string == "R32G32_SINT") return Format::R32G32_SINT;
	if (string == "R32G32_SFLOAT") return Format::R32G32_SFLOAT;
	if (string == "R32G32B32_UINT") return Format::R32G32B32_UINT;
	if (string == "R32G32B32_SINT") return Format::R32G32B32_SINT;
	if (string == "R32G32B32_SFLOAT") return Format::R32G32B32_SFLOAT;
	if (string == "R32G32B32A32_UINT") return Format::R32G32B32A32_UINT;
	if (string == "R32G32B32A32_SINT") return Format::R32G32B32A32_SINT;
	if (string == "R32G32B32A32_SFLOAT") return Format::R32G32B32A32_SFLOAT;
	if (string == "R64_UINT") return Format::R64_UINT;
	if (string == "R64_SINT") return Format::R64_SINT;
	if (string == "R64_SFLOAT") return Format::R64_SFLOAT;
	if (string == "R64G64_UINT") return Format::R64G64_UINT;
	if (string == "R64G64_SINT") return Format::R64G64_SINT;
	if (string == "R64G64_SFLOAT") return Format::R64G64_SFLOAT;
	if (string == "R64G64B64_UINT") return Format::R64G64B64_UINT;
	if (string == "R64G64B64_SINT") return Format::R64G64B64_SINT;
	if (string == "R64G64B64_SFLOAT") return Format::R64G64B64_SFLOAT;
	if (string == "R64G64B64A64_UINT") return Format::R64G64B64A64_UINT;
	if (string == "R64G64B64A64_SINT") return Format::R64G64B64A64_SINT;
	if (string == "R64G64B64A64_SFLOAT") return Format::R64G64B64A64_SFLOAT;

	return Format();
}

Bool Minty::Parse::try_format(String const& string, Format& value)
{
	value = to_format(string);
	return value != Format();
}

ShaderPrimitiveTopology Minty::Parse::to_shader_primitive_topology(String const& string)
{
	if (string == "PointList") return ShaderPrimitiveTopology::PointList;
	if (string == "LineList") return ShaderPrimitiveTopology::LineList;
	if (string == "LineStrip") return ShaderPrimitiveTopology::LineStrip;
	if (string == "TriangleList") return ShaderPrimitiveTopology::TriangleList;
	if (string == "TriangleStrip") return ShaderPrimitiveTopology::TriangleStrip;
	if (string == "TriangleFan") return ShaderPrimitiveTopology::TriangleFan;

	return ShaderPrimitiveTopology();
}

Bool Minty::Parse::try_shader_primitive_topology(String const& string, ShaderPrimitiveTopology& value)
{
	value = to_shader_primitive_topology(string);
	return value != ShaderPrimitiveTopology();
}

ShaderCullMode Minty::Parse::to_shader_cull_mode(String const& string)
{
	if (string == "None") return ShaderCullMode::None;
	if (string == "Front") return ShaderCullMode::Front;
	if (string == "Back") return ShaderCullMode::Back;
	if (string == "Both") return ShaderCullMode::Both;

	return ShaderCullMode();
}

Bool Minty::Parse::try_shader_cull_mode(String const& string, ShaderCullMode& value)
{
	value = to_shader_cull_mode(string);
	return value != ShaderCullMode();
}

ShaderFrontFace Minty::Parse::to_shader_front_face(String const& string)
{
	if (string == "Undefined") return ShaderFrontFace::Undefined;
	if (string == "CounterClockwise") return ShaderFrontFace::CounterClockwise;
	if (string == "Clockwise") return ShaderFrontFace::Clockwise;

	return ShaderFrontFace();
}

Bool Minty::Parse::try_shader_front_face(String const& string, ShaderFrontFace& value)
{
	value = to_shader_front_face(string);
	return value != ShaderFrontFace();
}

ShaderPolygonMode Minty::Parse::to_shader_polygon_mode(String const& string)
{
	if (string == "Undefined") return ShaderPolygonMode::Undefined;
	if (string == "Fill") return ShaderPolygonMode::Fill;
	if (string == "Line") return ShaderPolygonMode::Line;
	if (string == "Point") return ShaderPolygonMode::Point;

	return ShaderPolygonMode();
}

Bool Minty::Parse::try_shader_polygon_mode(String const& string, ShaderPolygonMode& value)
{
	value = to_shader_polygon_mode(string);
	return value != ShaderPolygonMode();
}

AssetType Minty::Parse::to_asset_type(String const& string)
{
	if (string == "None") return AssetType::None;
	if (string == "Meta") return AssetType::Meta;
	if (string == "Wrap") return AssetType::Wrap;
	if (string == "Text") return AssetType::Text;
	if (string == "Script") return AssetType::Script;
	if (string == "AudioClip") return AssetType::AudioClip;
	if (string == "Animation") return AssetType::Animation;
	if (string == "Animator") return AssetType::Animator;
	if (string == "Texture") return AssetType::Texture;
	if (string == "ShaderCode") return AssetType::ShaderCode;
	if (string == "ShaderModule") return AssetType::ShaderModule;
	if (string == "Shader") return AssetType::Shader;
	//if (string == "ShaderPass") return AssetType::ShaderPass;
	if (string == "MaterialTemplate") return AssetType::MaterialTemplate;
	if (string == "Material") return AssetType::Material;
	if (string == "Sprite") return AssetType::Sprite;
	if (string == "FontVariant") return AssetType::FontVariant;
	if (string == "Font") return AssetType::Font;
	if (string == "Mesh") return AssetType::Mesh;
	if (string == "Scene") return AssetType::Scene;

	return AssetType();
}

Bool Minty::Parse::try_asset_type(String const& string, AssetType& value)
{
	value = to_asset_type(string);
	return value != AssetType();
}

MeshType Minty::Parse::to_mesh_type(String const& string)
{
	if (string == "Empty") return MeshType::Empty;
	if (string == "Custom") return MeshType::Custom;
	if (string == "Quad") return MeshType::Quad;
	if (string == "Cube") return MeshType::Cube;
	if (string == "Pyramid") return MeshType::Pyramid;
	if (string == "Sphere") return MeshType::Sphere;
	if (string == "Cylinder") return MeshType::Cylinder;

	return MeshType();
}

Bool Minty::Parse::try_mesh_type(String const& string, MeshType& value)
{
	value = to_mesh_type(string);
	return value != MeshType();
}

AnchorMode Minty::Parse::to_anchor_mode(String const& string)
{
	if (string == "All") return AnchorMode::All;

	AnchorMode anchorMode = AnchorMode::Empty;

	if (string.find("Top") != String::npos)
	{
		anchorMode = anchorMode | AnchorMode::Top;
	}
	else if (string.find("Middle") != String::npos)
	{
		anchorMode = anchorMode | AnchorMode::Middle;
	}
	else if (string.find("Bottom") != String::npos)
	{
		anchorMode = anchorMode | AnchorMode::Bottom;
	}

	if (string.find("Left") != String::npos)
	{
		anchorMode = anchorMode | AnchorMode::Left;
	}
	else if (string.find("Center") != String::npos)
	{
		anchorMode = anchorMode | AnchorMode::Center;
	}
	else if (string.find("Right") != String::npos)
	{
		anchorMode = anchorMode | AnchorMode::Right;
	}

	return anchorMode;
}

Bool Minty::Parse::try_anchor_mode(String const& string, AnchorMode& value)
{
	value = to_anchor_mode(string);
	return value != AnchorMode();
}

Perspective Minty::Parse::to_perspective(String const& string)
{
	if (string == "Perspective") return Perspective::Perspective;
	if (string == "Orthographic") return Perspective::Orthographic;

	return Perspective();
}

Bool Minty::Parse::try_perspective(String const& string, Perspective& value)
{
	value = to_perspective(string);
	return value != Perspective();
}

Color Minty::Parse::to_color(String const& string)
{
	Color color;
	if (try_color(string, color))
	{
		return color;
	}
	
	return Color::error();
}

Bool Minty::Parse::try_color(String const& string, Color& value)
{
	if (string == "Red") value = Color::red();
	else if (string == "Green") value = Color::green();
	else if (string == "Blue") value = Color::blue();
	else if (string == "White") value = Color::white();
	else if (string == "Black") value = Color::black();
	else if (string == "Yellow") value = Color::yellow();
	else if (string == "Cyan") value = Color::cyan();
	else if (string == "Magenta") value = Color::magenta();
	else
	{
		// assume hex
		String text = string;
		if (string.starts_with("#"))
		{
			text = text.substr(1, text.size() - 1);
		}

		if (!Encoding::is_base16(text))
		{
			return false;
		}

		ConstantContainer container = Encoding::decode_base16(text);
		Int rawValue;
		memcpy(&rawValue, container.data(), sizeof(Int));
		value = Color(rawValue);
	}

	return true;
}

Conditional Minty::Parse::to_conditional(String const& string)
{
	if (string == "==") return Conditional::Equal;
	if (string == "!=") return Conditional::NotEqual;
	if (string == ">") return Conditional::GreaterThan;
	if (string == ">=") return Conditional::GreaterThanOrEqualTo;
	if (string == "<") return Conditional::LessThan;
	if (string == "<=") return Conditional::LessThanOrEqualTo;

	return Conditional();
}

Bool Minty::Parse::try_conditional(String const& string, Conditional& value)
{
	value = to_conditional(string);
	return value != Conditional();
}

Attenuation Minty::Parse::to_attenuation(String const& string)
{
	if (string == "None") return Attenuation::None;
	if (string == "InverseDistance") return Attenuation::InverseDistance;
	if (string == "LinearDistance") return Attenuation::LinearDistance;
	if (string == "ExponentialDistance") return Attenuation::ExponentialDistance;

	return Attenuation();
}

Bool Minty::Parse::try_attenuation(String const& string, Attenuation& value)
{
	value = to_attenuation(string);
	return value != Attenuation();
}

ShaderInputRate Minty::Parse::to_shader_input_rate(String const& string)
{
	if (string == "Undefined") return ShaderInputRate::Undefined;
	if (string == "Vertex") return ShaderInputRate::Vertex;
	if (string == "Instance") return ShaderInputRate::Instance;

	return ShaderInputRate();
}

Bool Minty::Parse::try_shader_input_rate(String const& string, ShaderInputRate& value)
{
	value = to_shader_input_rate(string);
	return value != ShaderInputRate();
}

CoordinateMode Minty::Parse::to_coordinate_mode(String const& string)
{
	if (string == "Normalized") return CoordinateMode::Normalized;
	if (string == "Pixel") return CoordinateMode::Pixel;

	return CoordinateMode();
}

Bool Minty::Parse::try_coordinate_mode(String const& string, CoordinateMode& value)
{
	value = to_coordinate_mode(string);
	return value != CoordinateMode();
}