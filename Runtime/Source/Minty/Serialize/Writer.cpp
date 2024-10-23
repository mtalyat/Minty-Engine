#include "pch.h"
#include "Writer.h"

#include "Minty/Core/UUID.h"
#include "Minty/Serialize/Serializable.h"

using namespace Minty;

void Minty::FileWriterBehavior::write_data(const void* const data, const Size size)
{
	mp_file->write(data, size);
}

void Minty::MemoryWriterBehavior::write_data(const void* const data, const Size size)
{
    mp_data->append(data, size);
}

void Minty::TextWriterBehavior::write_indent_to_buffer(const Size indent, std::vector<Byte>& buffer)
{
	write_string_to_buffer(String(indent, INDENT), buffer);
}

Bool Minty::TextWriterBehavior::write_name_to_buffer(const String& data, std::vector<Byte>& buffer)
{
	MINTY_ASSERT_MESSAGE(data.size() < 256, "A name cannot be longer than 255 bytes.");

	if (data.empty())
	{
		// no name: do bullet point
		write_string_to_buffer("- ", buffer);
		return false;
	}
	else
	{
		// yes name: write name
		write_string_to_buffer(data, buffer);
		return true;
	}
}

void Minty::TextWriterBehavior::write_separator_to_buffer(std::vector<Byte>& buffer)
{
	write_string_to_buffer(": ", buffer);
}

void Minty::TextWriterBehavior::write_space_to_buffer(std::vector<Byte>& buffer)
{
    write_string_to_buffer(" ", buffer);
}

void Minty::TextWriterBehavior::write_end_to_buffer(std::vector<Byte>& buffer)
{
	write_string_to_buffer("\n", buffer);
}

void Minty::TextWriterBehavior::write_string_to_buffer(const String& data, std::vector<Byte>& buffer)
{
	// add string to buffer
	Size offset = buffer.size();
	buffer.resize(offset + data.size());
	memcpy(buffer.data() + offset, data.data(), data.size());
}

void Minty::TextWriterBehavior::write_bool_to_buffer(const Bool data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_bool2_to_buffer(const Bool2 data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_bool3_to_buffer(const Bool3 data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_bool4_to_buffer(const Bool4 data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_char_to_buffer(const Char data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_byte_to_buffer(const Byte data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_short_to_buffer(const Short data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_ushort_to_buffer(const UShort data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_int_to_buffer(const Int data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_int2_to_buffer(const Int2 data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_int3_to_buffer(const Int3 data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_int4_to_buffer(const Int4 data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_uint_to_buffer(const UInt data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_uint2_to_buffer(const UInt2 data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_uint3_to_buffer(const UInt3 data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_uint4_to_buffer(const UInt4 data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_long_to_buffer(const Long data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_ulong_to_buffer(const ULong data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_size_to_buffer(const Size data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_float_to_buffer(const Float data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_float2_to_buffer(const Float2 data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_float3_to_buffer(const Float3 data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_float4_to_buffer(const Float4 data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_double_to_buffer(const Double data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_uuid_to_buffer(const UUID data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}
void Minty::TextWriterBehavior::write_type_to_buffer(const Type data, std::vector<Byte>& buffer)
{
    write_string_to_buffer(Minty::to_string(data), buffer);
}

void Minty::TextWriterBehavior::write_typed_to_buffer(const Type type, void const* const data, std::vector<Byte>& buffer)
{
    switch (type)
    {
    case Type::Bool:
        write_bool_to_buffer(*static_cast<Bool const* const>(data), buffer);
        break;
    case Type::Bool2:
        write_bool2_to_buffer(*static_cast<Bool2 const* const>(data), buffer);
        break;
    case Type::Bool3:
        write_bool3_to_buffer(*static_cast<Bool3 const* const>(data), buffer);
        break;
    case Type::Bool4:
        write_bool4_to_buffer(*static_cast<Bool4 const* const>(data), buffer);
        break;
    case Type::Char:
        write_char_to_buffer(*static_cast<Char const* const>(data), buffer);
        break;
    case Type::Byte:
        write_byte_to_buffer(*static_cast<Byte const* const>(data), buffer);
        break;
    case Type::Short:
        write_short_to_buffer(*static_cast<Short const* const>(data), buffer);
        break;
    case Type::UShort:
        write_ushort_to_buffer(*static_cast<UShort const* const>(data), buffer);
        break;
    case Type::Int:
        write_int_to_buffer(*static_cast<Int const* const>(data), buffer);
        break;
    case Type::Int2:
        write_int2_to_buffer(*static_cast<Int2 const* const>(data), buffer);
        break;
    case Type::Int3:
        write_int3_to_buffer(*static_cast<Int3 const* const>(data), buffer);
        break;
    case Type::Int4:
        write_int4_to_buffer(*static_cast<Int4 const* const>(data), buffer);
        break;
    case Type::UInt:
        write_uint_to_buffer(*static_cast<UInt const* const>(data), buffer);
        break;
    case Type::UInt2:
        write_uint2_to_buffer(*static_cast<UInt2 const* const>(data), buffer);
        break;
    case Type::UInt3:
        write_uint3_to_buffer(*static_cast<UInt3 const* const>(data), buffer);
        break;
    case Type::UInt4:
        write_uint4_to_buffer(*static_cast<UInt4 const* const>(data), buffer);
        break;
    case Type::Long:
        write_long_to_buffer(*static_cast<Long const* const>(data), buffer);
        break;
    case Type::ULong:
        write_ulong_to_buffer(*static_cast<ULong const* const>(data), buffer);
        break;
    case Type::Size:
        write_size_to_buffer(*static_cast<Size const* const>(data), buffer);
        break;
    case Type::Float:
        write_float_to_buffer(*static_cast<Float const* const>(data), buffer);
        break;
    case Type::Float2:
        write_float2_to_buffer(*static_cast<Float2 const* const>(data), buffer);
        break;
    case Type::Float3:
        write_float3_to_buffer(*static_cast<Float3 const* const>(data), buffer);
        break;
    case Type::Float4:
        write_float4_to_buffer(*static_cast<Float4 const* const>(data), buffer);
        break;
    case Type::Double:
        write_double_to_buffer(*static_cast<Double const* const>(data), buffer);
        break;
    case Type::String:
        write_string_to_buffer(*static_cast<String const* const>(data), buffer);
        break;
    default:
        MINTY_ABORT_FORMAT("Cannot write type \"{}\".", to_string(type));
    }
}

void Minty::Writer::write_object(String const& name, Serializable const& data)
{
    write(name);
    indent();
    data.serialize(*this);
    outdent();
}

void Minty::Writer::write(String const& name, const void* const data, Type const type)
{
    switch (type)
    {
    case Type::Bool:
        return write_bool(name, *static_cast<Bool const* const>(data));
    case Type::Bool2:
        return write_bool2(name, *static_cast<Bool2 const* const>(data));
    case Type::Bool3:
        return write_bool3(name, *static_cast<Bool3 const* const>(data));
    case Type::Bool4:
        return write_bool4(name, *static_cast<Bool4 const* const>(data));
    case Type::Char:
        return write_char(name, *static_cast<Char const* const>(data));
    case Type::Byte:
        return write_byte(name, *static_cast<Byte const* const>(data));
    case Type::Short:
        return write_short(name, *static_cast<Short const* const>(data));
    case Type::UShort:
        return write_ushort(name, *static_cast<UShort const* const>(data));
    case Type::Int:
        return write_int(name, *static_cast<Int const* const>(data));
    case Type::Int2:
        return write_int2(name, *static_cast<Int2 const* const>(data));
    case Type::Int3:
        return write_int3(name, *static_cast<Int3 const* const>(data));
    case Type::Int4:
        return write_int4(name, *static_cast<Int4 const* const>(data));
    case Type::UInt:
        return write_uint(name, *static_cast<UInt const* const>(data));
    case Type::UInt2:
        return write_uint2(name, *static_cast<UInt2 const* const>(data));
    case Type::UInt3:
        return write_uint3(name, *static_cast<UInt3 const* const>(data));
    case Type::UInt4:
        return write_uint4(name, *static_cast<UInt4 const* const>(data));
    case Type::Long:
        return write_long(name, *static_cast<Long const* const>(data));
    case Type::ULong:
        return write_ulong(name, *static_cast<ULong const* const>(data));
    case Type::Size:
        return write_size(name, *static_cast<Size const* const>(data));
    case Type::Float:
        return write_float(name, *static_cast<Float const* const>(data));
    case Type::Float2:
        return write_float2(name, *static_cast<Float2 const* const>(data));
    case Type::Float3:
        return write_float3(name, *static_cast<Float3 const* const>(data));
    case Type::Float4:
        return write_float4(name, *static_cast<Float4 const* const>(data));
    case Type::Double:
        return write_double(name, *static_cast<Double const* const>(data));
    case Type::String:
        return write_string(name, *static_cast<String const* const>(data));
    default:
        MINTY_ABORT_FORMAT("Cannot write type \"{}\".", to_string(type));
    }
}
