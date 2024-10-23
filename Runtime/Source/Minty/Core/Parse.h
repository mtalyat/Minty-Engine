#pragma once

#include "Minty/Core/Color.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/String.h"
#include "Minty/Core/Type.h"
#include "Minty/Core/UUID.h"
#include "Minty/FSM/Conditional.h"
#include "Minty/Render/Perspective.h"
#include "Minty/Render/Mesh.h"
#include "Minty/Render/Shader.h"
#include "Minty/UI/AnchorMode.h"
#include <string>

namespace Minty::Parse
{
	Bool to_bool(String const& string);

	Bool try_bool(String const& string, Bool& value);

	Bool2 to_bool2(String const& string);

	Bool try_bool2(String const& string, Bool2& value);

	Bool3 to_bool3(String const& string);

	Bool try_bool3(String const& string, Bool3& value);

	Bool4 to_bool4(String const& string);

	Bool try_bool4(String const& string, Bool4& value);

	Char to_char(String const& string);

	Bool try_char(String const& string, Char& value);

	Byte to_byte(String const& string);

	Bool try_byte(String const& string, Byte& value);

	Short to_short(String const& string);

	Bool try_short(String const& string, Short& value);

	UShort to_ushort(String const& string);

	Bool try_ushort(String const& string, UShort& value);

	Int to_int(String const& string);

	Bool try_int(String const& string, Int& value);

	Int2 to_int2(String const& string);

	Bool try_int2(String const& string, Int2& value);

	Int3 to_int3(String const& string);

	Bool try_int3(String const& string, Int3& value);

	Int4 to_int4(String const& string);

	Bool try_int4(String const& string, Int4& value);

	UInt to_uint(String const& string);

	Bool try_uint(String const& string, UInt& value);

	UInt2 to_uint2(String const& string);

	Bool try_uint2(String const& string, UInt2& value);

	UInt3 to_uint3(String const& string);

	Bool try_uint3(String const& string, UInt3& value);

	UInt4 to_uint4(String const& string);

	Bool try_uint4(String const& string, UInt4& value);

	Long to_long(String const& string);

	Bool try_long(String const& string, Long& value);

	ULong to_ulong(String const& string);

	Bool try_ulong(String const& string, ULong& value);

	Size to_size(String const& string);

	Bool try_size(String const& string, Size& value);

	Float to_float(String const& string);

	Bool try_float(String const& string, Float& value);

	Float2 to_float2(String const& string);

	Bool try_float2(String const& string, Float2& value);

	Float3 to_float3(String const& string);

	Bool try_float3(String const& string, Float3& value);

	Float4 to_float4(String const& string);

	Bool try_float4(String const& string, Float4& value);

	Double to_double(String const& string);

	Bool try_double(String const& string, Double& value);

	UUID to_uuid(String const& string);

	Bool try_uuid(String const& string, UUID& value);

	template<typename T>
	void parse_to(String const& string, T& value)
	{
		MINTY_ABORT("parse_to not implemented for the given type.");
	}

	template<typename T>
	Bool parse_try(String const& string, T& value)
	{
		MINTY_ABORT("parse_try not implemented for the given type.");
	}

	template<>
	inline void parse_to(String const& string, Bool& value)
	{
		value = to_bool(string);
	}

	template<>
	inline Bool parse_try(String const& string, Bool& value)
	{
		return try_bool(string, value);
	}

	template<>
	inline void parse_to(String const& string, Bool2& value)
	{
		value = to_bool2(string);
	}

	template<>
	inline Bool parse_try(String const& string, Bool2& value)
	{
		return try_bool2(string, value);
	}

	template<>
	inline void parse_to(String const& string, Bool3& value)
	{
		value = to_bool3(string);
	}

	template<>
	inline Bool parse_try(String const& string, Bool3& value)
	{
		return try_bool3(string, value);
	}

	template<>
	inline void parse_to(String const& string, Bool4& value)
	{
		value = to_bool4(string);
	}

	template<>
	inline Bool parse_try(String const& string, Bool4& value)
	{
		return try_bool4(string, value);
	}

	template<>
	inline void parse_to(String const& string, Char& value)
	{
		value = to_char(string);
	}

	template<>
	inline Bool parse_try(String const& string, Char& value)
	{
		return try_char(string, value);
	}

	template<>
	inline void parse_to(String const& string, Byte& value)
	{
		value = to_byte(string);
	}

	template<>
	inline Bool parse_try(String const& string, Byte& value)
	{
		return try_byte(string, value);
	}

	template<>
	inline void parse_to(String const& string, Short& value)
	{
		value = to_short(string);
	}

	template<>
	inline Bool parse_try(String const& string, Short& value)
	{
		return try_short(string, value);
	}

	template<>
	inline void parse_to(String const& string, UShort& value)
	{
		value = to_ushort(string);
	}

	template<>
	inline Bool parse_try(String const& string, UShort& value)
	{
		return try_ushort(string, value);
	}

	template<>
	inline void parse_to(String const& string, Int& value)
	{
		value = to_int(string);
	}

	template<>
	inline Bool parse_try(String const& string, Int& value)
	{
		return try_int(string, value);
	}

	template<>
	inline void parse_to(String const& string, Int2& value)
	{
		value = to_int2(string);
	}

	template<>
	inline Bool parse_try(String const& string, Int2& value)
	{
		return try_int2(string, value);
	}

	template<>
	inline void parse_to(String const& string, Int3& value)
	{
		value = to_int3(string);
	}

	template<>
	inline Bool parse_try(String const& string, Int3& value)
	{
		return try_int3(string, value);
	}

	template<>
	inline void parse_to(String const& string, Int4& value)
	{
		value = to_int4(string);
	}

	template<>
	inline Bool parse_try(String const& string, Int4& value)
	{
		return try_int4(string, value);
	}

	template<>
	inline void parse_to(String const& string, UInt& value)
	{
		value = to_uint(string);
	}

	template<>
	inline Bool parse_try(String const& string, UInt& value)
	{
		return try_uint(string, value);
	}

	template<>
	inline void parse_to(String const& string, UInt2& value)
	{
		value = to_uint2(string);
	}

	template<>
	inline Bool parse_try(String const& string, UInt2& value)
	{
		return try_uint2(string, value);
	}

	template<>
	inline void parse_to(String const& string, UInt3& value)
	{
		value = to_uint3(string);
	}

	template<>
	inline Bool parse_try(String const& string, UInt3& value)
	{
		return try_uint3(string, value);
	}

	template<>
	inline void parse_to(String const& string, UInt4& value)
	{
		value = to_uint4(string);
	}

	template<>
	inline Bool parse_try(String const& string, UInt4& value)
	{
		return try_uint4(string, value);
	}

	template<>
	inline void parse_to(String const& string, Long& value)
	{
		value = to_long(string);
	}

	template<>
	inline Bool parse_try(String const& string, Long& value)
	{
		return try_long(string, value);
	}

	template<>
	inline void parse_to(String const& string, ULong& value)
	{
		value = to_ulong(string);
	}

	template<>
	inline Bool parse_try(String const& string, ULong& value)
	{
		return try_ulong(string, value);
	}

	template<>
	inline void parse_to(String const& string, Size& value)
	{
		value = to_size(string);
	}

	template<>
	inline Bool parse_try(String const& string, Size& value)
	{
		return try_size(string, value);
	}

	template<>
	inline void parse_to(String const& string, Float& value)
	{
		value = to_float(string);
	}

	template<>
	inline Bool parse_try(String const& string, Float& value)
	{
		return try_float(string, value);
	}

	template<>
	inline void parse_to(String const& string, Float2& value)
	{
		value = to_float2(string);
	}

	template<>
	inline Bool parse_try(String const& string, Float2& value)
	{
		return try_float2(string, value);
	}

	template<>
	inline void parse_to(String const& string, Float3& value)
	{
		value = to_float3(string);
	}

	template<>
	inline Bool parse_try(String const& string, Float3& value)
	{
		return try_float3(string, value);
	}

	template<>
	inline void parse_to(String const& string, Float4& value)
	{
		value = to_float4(string);
	}

	template<>
	inline Bool parse_try(String const& string, Float4& value)
	{
		return try_float4(string, value);
	}

	template<>
	inline void parse_to(String const& string, Double& value)
	{
		value = to_double(string);
	}

	template<>
	inline Bool parse_try(String const& string, Double& value)
	{
		return try_double(string, value);
	}

	template<>
	inline void parse_to(String const& string, String& value)
	{
		value = string;
	}

	template<>
	inline Bool parse_try(String const& string, String& value)
	{
		value = string;
		return true;
	}

	template<>
	inline void parse_to(String const& string, UUID& value)
	{
		value = to_uuid(string);
	}

	template<>
	inline Bool parse_try(String const& string, UUID& value)
	{
		return try_uuid(string, value);
	}

	Type to_type(String const& string);
	Bool try_type(String const& string, Type& value);

	template<>
	inline void parse_to(String const& string, Type& value)
	{
		value = to_type(string);
	}

	template<>
	inline Bool parse_try(String const& string, Type& value)
	{
		return try_type(string, value);
	}

	ShaderStage to_shader_stage(String const& string);
	Bool try_shader_stage(String const& string, ShaderStage& value);

	template<>
	inline void parse_to(String const& string, ShaderStage& value)
	{
		value = to_shader_stage(string);
	}

	template<>
	inline Bool parse_try(String const& string, ShaderStage& value)
	{
		return try_shader_stage(string, value);
	}

	ShaderInputType to_shader_descriptor_type(String const& string);
	Bool try_shader_descriptor_type(String const& string, ShaderInputType& value);

	template<>
	inline void parse_to(String const& string, ShaderInputType& value)
	{
		value = to_shader_descriptor_type(string);
	}

	template<>
	inline Bool parse_try(String const& string, ShaderInputType& value)
	{
		return try_shader_descriptor_type(string, value);
	}

	Format to_format(String const& string);
	Bool try_format(String const& string, Format& value);

	template<>
	inline void parse_to(String const& string, Format& value)
	{
		value = to_format(string);
	}

	template<>
	inline Bool parse_try(String const& string, Format& value)
	{
		return try_format(string, value);
	}

	ShaderPrimitiveTopology to_shader_primitive_topology(String const& string);
	Bool try_shader_primitive_topology(String const& string, ShaderPrimitiveTopology& value);

	template<>
	inline void parse_to(String const& string, ShaderPrimitiveTopology& value)
	{
		value = to_shader_primitive_topology(string);
	}

	template<>
	inline Bool parse_try(String const& string, ShaderPrimitiveTopology& value)
	{
		return try_shader_primitive_topology(string, value);
	}

	ShaderCullMode to_shader_cull_mode(String const& string);
	Bool try_shader_cull_mode(String const& string, ShaderCullMode& value);

	template<>
	inline void parse_to(String const& string, ShaderCullMode& value)
	{
		value = to_shader_cull_mode(string);
	}

	template<>
	inline Bool parse_try(String const& string, ShaderCullMode& value)
	{
		return try_shader_cull_mode(string, value);
	}

	ShaderFrontFace to_shader_front_face(String const& string);
	Bool try_shader_front_face(String const& string, ShaderFrontFace& value);

	template<>
	inline void parse_to(String const& string, ShaderFrontFace& value)
	{
		value = to_shader_front_face(string);
	}

	template<>
	inline Bool parse_try(String const& string, ShaderFrontFace& value)
	{
		return try_shader_front_face(string, value);
	}

	ShaderPolygonMode to_shader_polygon_mode(String const& string);
	Bool try_shader_polygon_mode(String const& string, ShaderPolygonMode& value);

	template<>
	inline void parse_to(String const& string, ShaderPolygonMode& value)
	{
		value = to_shader_polygon_mode(string);
	}

	template<>
	inline Bool parse_try(String const& string, ShaderPolygonMode& value)
	{
		return try_shader_polygon_mode(string, value);
	}

	AssetType to_asset_type(String const& string);
	Bool try_asset_type(String const& string, AssetType& value);

	template<>
	inline void parse_to(String const& string, AssetType& value)
	{
		value = to_asset_type(string);
	}

	template<>
	inline Bool parse_try(String const& string, AssetType& value)
	{
		return try_asset_type(string, value);
	}

	template<>
	inline void parse_to(String const& string, Path& path)
	{
		if (string.starts_with("\"") && string.ends_with("\""))
		{
			path = Path(string.substr(1, string.length() - 2));
		}
		else
		{
			path = Path(string);
		}
	}

	template<>
	inline Bool parse_try(String const& string, Path& path)
	{
		parse_to(string, path);
		return true;
	}

	MeshType to_mesh_type(String const& string);
	Bool try_mesh_type(String const& string, MeshType& value);

	template<>
	inline void parse_to(String const& string, MeshType& value)
	{
		value = to_mesh_type(string);
	}

	template<>
	inline Bool parse_try(String const& string, MeshType& value)
	{
		return try_mesh_type(string, value);
	}

	AnchorMode to_anchor_mode(String const& string);
	Bool try_anchor_mode(String const& string, AnchorMode& value);

	template<>
	inline void parse_to(String const& string, AnchorMode& value)
	{
		value = to_anchor_mode(string);
	}

	template<>
	inline Bool parse_try(String const& string, AnchorMode& value)
	{
		return try_anchor_mode(string, value);
	}

	Perspective to_perspective(String const& string);
	Bool try_perspective(String const& string, Perspective& value);

	template<>
	inline void parse_to(String const& string, Perspective& value)
	{
		value = to_perspective(string);
	}

	template<>
	inline Bool parse_try(String const& string, Perspective& value)
	{
		return try_perspective(string, value);
	}

	Color to_color(String const& string);
	Bool try_color(String const& string, Color& value);

	template<>
	inline void parse_to(String const& string, Color& value)
	{
		value = to_color(string);
	}

	template<>
	inline Bool parse_try(String const& string, Color& value)
	{
		return try_color(string, value);
	}

	Conditional to_conditional(String const& string);
	Bool try_conditional(String const& string, Conditional& value);

	template<>
	inline void parse_to(String const& string, Conditional& value)
	{
		value = to_conditional(string);
	}

	template<>
	inline Bool parse_try(String const& string, Conditional& value)
	{
		return try_conditional(string, value);
	}
}