#include "pch.h"
#include "Type.h"

#include "Minty/Core/Macros.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/UUID.h"

using namespace Minty;

Type Minty::type_typeid(const TypeID typeId)
{
	if (typeId == typeid(Bool))
	{
		return Type::Bool;
	}
	else if (typeId == typeid(Bool2))
	{
		return Type::Bool2;
	}
	else if (typeId == typeid(Bool3))
	{
		return Type::Bool3;
	}
	else if (typeId == typeid(Bool4))
	{
		return Type::Bool4;
	}
	else if (typeId == typeid(Char))
	{
		return Type::Char;
	}
	else if (typeId == typeid(Byte))
	{
		return Type::Byte;
	}
	else if (typeId == typeid(Short))
	{
		return Type::Short;
	}
	else if (typeId == typeid(UShort))
	{
		return Type::UShort;
	}
	else if (typeId == typeid(Int))
	{
		return Type::Int;
	}
	else if (typeId == typeid(Int2))
	{
		return Type::Int2;
	}
	else if (typeId == typeid(Int3))
	{
		return Type::Int3;
	}
	else if (typeId == typeid(Int4))
	{
		return Type::Int4;
	}
	else if (typeId == typeid(UInt))
	{
		return Type::UInt;
	}
	else if (typeId == typeid(UInt2))
	{
		return Type::UInt2;
	}
	else if (typeId == typeid(UInt3))
	{
		return Type::UInt3;
	}
	else if (typeId == typeid(UInt4))
	{
		return Type::UInt4;
	}
	else if (typeId == typeid(Long))
	{
		return Type::Long;
	}
	else if (typeId == typeid(ULong))
	{
		return Type::ULong;
	}
	else if (typeId == typeid(Size))
	{
		return Type::Size;
	}
	else if (typeId == typeid(Float))
	{
		return Type::Float;
	}
	else if (typeId == typeid(Float2))
	{
		return Type::Float2;
	}
	else if (typeId == typeid(Float3))
	{
		return Type::Float3;
	}
	else if (typeId == typeid(Float4))
	{
		return Type::Float4;
	}
	else if (typeId == typeid(Double))
	{
		return Type::Double;
	}
	else if (typeId == typeid(String))
	{
		return Type::String;
	}
	else if (typeId == typeid(Matrix2))
	{
		return Type::Matrix2;
	}
	else if (typeId == typeid(Matrix3))
	{
		return Type::Matrix3;
	}
	else if (typeId == typeid(Matrix4))
	{
		return Type::Matrix4;
	}
	else if (typeId == typeid(Quaternion))
	{
		return Type::Quaternion;
	}
	else if (typeId == typeid(UUID))
	{
		return Type::UUID;
	}
	else
	{
		return Type::Undefined;
	}
}

TypeID Minty::typeid_type(const Type type)
{
	switch (type)
	{
	case Type::Bool:
		return typeid(Bool);
	case Type::Bool2:
		return typeid(Bool2);
	case Type::Bool3:
		return typeid(Bool3);
	case Type::Bool4:
		return typeid(Bool4);
	case Type::Char:
		return typeid(Char);
	case Type::Byte:
		return typeid(Byte);
	case Type::Short:
		return typeid(Short);
	case Type::UShort:
		return typeid(UShort);
	case Type::Int:
		return typeid(Int);
	case Type::Int2:
		return typeid(Int2);
	case Type::Int3:
		return typeid(Int3);
	case Type::Int4:
		return typeid(Int4);
	case Type::UInt:
		return typeid(UInt);
	case Type::UInt2:
		return typeid(UInt2);
	case Type::UInt3:
		return typeid(UInt3);
	case Type::UInt4:
		return typeid(UInt4);
	case Type::Long:
		return typeid(Long);
	case Type::ULong:
		return typeid(ULong);
	case Type::Size:
		return typeid(Size);
	case Type::Float:
		return typeid(Float);
	case Type::Float2:
		return typeid(Float2);
	case Type::Float3:
		return typeid(Float3);
	case Type::Float4:
		return typeid(Float4);
	case Type::Double:
		return typeid(Double);
	case Type::Matrix2:
		return typeid(Matrix2);
	case Type::Matrix3:
		return typeid(Matrix3);
	case Type::Matrix4:
		return typeid(Matrix4);
	case Type::Quaternion:
		return typeid(Quaternion);
	case Type::UUID:
		return typeid(UUID);
	default:
		MINTY_ABORT("typeid_type not implemented for given type.");
	}
}

Size Minty::sizeof_type(const Type type)
{
	switch (type)
	{
	case Type::Undefined:
		return 0;
	case Type::Bool:
		return sizeof(Bool);
	case Type::Bool2:
		return sizeof(Bool2);
	case Type::Bool3:
		return sizeof(Bool3);
	case Type::Bool4:
		return sizeof(Bool4);
	case Type::Char:
		return sizeof(Char);
	case Type::Byte:
		return sizeof(Byte);
	case Type::Short:
		return sizeof(Short);
	case Type::UShort:
		return sizeof(UShort);
	case Type::Int:
		return sizeof(Int);
	case Type::Int2:
		return sizeof(Int2);
	case Type::Int3:
		return sizeof(Int3);
	case Type::Int4:
		return sizeof(Int4);
	case Type::UInt:
		return sizeof(UInt);
	case Type::UInt2:
		return sizeof(UInt2);
	case Type::UInt3:
		return sizeof(UInt3);
	case Type::UInt4:
		return sizeof(UInt4);
	case Type::Long:
		return sizeof(Long);
	case Type::ULong:
		return sizeof(ULong);
	case Type::Size:
		return sizeof(Size);
	case Type::Float:
		return sizeof(Float);
	case Type::Float2:
		return sizeof(Float2);
	case Type::Float3:
		return sizeof(Float3);
	case Type::Float4:
		return sizeof(Float4);
	case Type::Double:
		return sizeof(Double);
	case Type::Matrix2:
		return sizeof(Matrix2);
	case Type::Matrix3:
		return sizeof(Matrix3);
	case Type::Matrix4:
		return sizeof(Matrix4);
	case Type::Quaternion:
		return sizeof(Quaternion);
	case Type::Asset:
		return sizeof(void*);
	case Type::UUID:
		return sizeof(UUID);
	default:
		MINTY_ABORT("typeof_type not implemented for given type.");
	}
}