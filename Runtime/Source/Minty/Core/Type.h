#pragma once

#include "Minty/Core/Types.h"

namespace Minty
{
	enum class Type
	{
		Undefined,
		Bool,
		Bool2,
		Bool3,
		Bool4,
		Char,
		Byte,
		Short,
		UShort,
		Int,
		Int2,
		Int3,
		Int4,
		UInt,
		UInt2,
		UInt3,
		UInt4,
		Long,
		ULong,
		Size,
		Float,
		Float2,
		Float3,
		Float4,
		Double,
		String,
		Matrix2,
		Matrix3,
		Matrix4,
		Quaternion,
		Asset,
		UUID,
	};

	Type type_typeid(const TypeID typeId);

	TypeID typeid_type(const Type type);

	Size sizeof_type(const Type type);
}