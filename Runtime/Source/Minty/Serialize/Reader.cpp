#include "pch.h"
#include "Reader.h"

#include "Minty/Core/Parse.h"
#include "Minty/Serialize/Serializable.h"

using namespace Minty;

void Minty::FileReaderBehavior::read_data(void* const data, Size const size)
{
	mp_file->read(data, size);
}

std::vector<Byte> Minty::FileReaderBehavior::read_all()
{
	std::vector<Byte> fileData(mp_file->size());
	read_data(fileData.data(), fileData.size());
	return fileData;
}

void Minty::MemoryReaderBehavior::read_data(void* const data, Size const size)
{
	// read from current index
	Byte* memoryData = static_cast<Byte*>(mp_data->data());
	memcpy(data, &memoryData[m_position], size);

	// incremement position
	m_position += size;
}

std::vector<Byte> Minty::MemoryReaderBehavior::read_all()
{
	// save position
	Size position = m_position;
	m_position = 0;

	// read all data
	std::vector<Byte> memoryData(mp_data->size());
	read_data(memoryData.data(), memoryData.size());

	// set position back
	m_position = position;

	return memoryData;
}

Bool Minty::TextReaderBehavior::read_bool_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_bool(text);
}
Bool2 Minty::TextReaderBehavior::read_bool2_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_bool2(text);
}
Bool3 Minty::TextReaderBehavior::read_bool3_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_bool3(text);
}
Bool4 Minty::TextReaderBehavior::read_bool4_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_bool4(text);
}
Char Minty::TextReaderBehavior::read_char_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_char(text);
}
Byte Minty::TextReaderBehavior::read_byte_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_byte(text);
}
Short Minty::TextReaderBehavior::read_short_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_short(text);
}
UShort Minty::TextReaderBehavior::read_ushort_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_ushort(text);
}
Int Minty::TextReaderBehavior::read_int_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_int(text);
}
Int2 Minty::TextReaderBehavior::read_int2_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_int2(text);
}
Int3 Minty::TextReaderBehavior::read_int3_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_int3(text);
}
Int4 Minty::TextReaderBehavior::read_int4_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_int4(text);
}
UInt Minty::TextReaderBehavior::read_uint_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_uint(text);
}
UInt2 Minty::TextReaderBehavior::read_uint2_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_uint2(text);
}
UInt3 Minty::TextReaderBehavior::read_uint3_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_uint3(text);
}
UInt4 Minty::TextReaderBehavior::read_uint4_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_uint4(text);
}
Long Minty::TextReaderBehavior::read_long_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_long(text);
}
ULong Minty::TextReaderBehavior::read_ulong_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_ulong(text);
}
Size Minty::TextReaderBehavior::read_size_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_size(text);
}
Float Minty::TextReaderBehavior::read_float_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_float(text);
}
Float2 Minty::TextReaderBehavior::read_float2_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_float2(text);
}
Float3 Minty::TextReaderBehavior::read_float3_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_float3(text);
}
Float4 Minty::TextReaderBehavior::read_float4_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_float4(text);
}
Double Minty::TextReaderBehavior::read_double_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_double(text);
}
String Minty::TextReaderBehavior::read_string_from_buffer(const void* const data, Size const size) const
{
	if (!size) return "";

	std::vector<Char> text(size + 1);
	memcpy(text.data(), data, size);
	return String(text.data());
}
UUID Minty::TextReaderBehavior::read_uuid_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_uuid(text);
}
Type Minty::TextReaderBehavior::read_type_from_buffer(const void* const data, Size const size) const
{
	if (!size) return {};

	String text = read_string_from_buffer(data, size);
	return Parse::to_type(text);
}

void* Minty::TextReaderBehavior::read_typed_from_buffer(const void* const data, Size const size, Type const type) const
{
	if (!size) return nullptr;

	void* output;

	switch (type)
	{
	case Type::Bool:
	{
		auto temp = read_bool_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Bool2:
	{
		auto temp = read_bool2_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Bool3:
	{
		auto temp = read_bool3_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Bool4:
	{
		auto temp = read_bool4_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Char:
	{
		auto temp = read_char_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Byte:
	{
		auto temp = read_byte_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Short:
	{
		auto temp = read_short_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::UShort:
	{
		auto temp = read_ushort_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Int:
	{
		auto temp = read_int_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Int2:
	{
		auto temp = read_int2_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Int3:
	{
		auto temp = read_int3_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Int4:
	{
		auto temp = read_int4_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::UInt:
	{
		auto temp = read_uint_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::UInt2:
	{
		auto temp = read_uint2_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::UInt3:
	{
		auto temp = read_uint3_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::UInt4:
	{
		auto temp = read_uint4_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Long:
	{
		auto temp = read_long_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::ULong:
	{
		auto temp = read_ulong_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Size:
	{
		auto temp = read_size_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Float:
	{
		auto temp = read_float_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Float2:
	{
		auto temp = read_float2_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Float3:
	{
		auto temp = read_float3_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Float4:
	{
		auto temp = read_float4_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::Double:
	{
		auto temp = read_double_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::String:
	{
		auto temp = read_string_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	case Type::UUID:
	{
		auto temp = read_uuid_from_buffer(data, size);
		output = new Byte[size];
		memcpy(output, &temp, size);
	}
	break;
	default:
		MINTY_ABORT_FORMAT("Cannot read type \"{}\".", to_string(type));
	}

	return output;
}

Variable Minty::TextReaderBehavior::read_variable_from_buffer(const void* const data, Size const size) const
{
	if (!size) return{};

	// read as string
	String text = read_string_from_buffer(data, size);

	// find space
	Size index = text.find(' ');
	if (index == String::npos)
	{
		MINTY_ERROR_FORMAT("Error reading Variable. Invalid format: \"{}\".", text);
		return{};
	}

	// get type
	String typeString = text.substr(0, index);
	Type type = Parse::to_type(typeString);

	MINTY_ASSERT_FORMAT(type != Type::Undefined, "Error reading Variable. Unknown type: \"{}\".", typeString);

	// knowing where the space is: parse the type from the data after the space
	Size offset = index + 1;
	Byte const* dataPtr = static_cast<Byte const*>(data);
	dataPtr += offset;
	Size dataSize = size - offset;
	void* value = read_typed_from_buffer(dataPtr, dataSize, type);

	// create variable
	Variable var(type, value);

	// done with temp value
	delete[] value;

	return var;
}

Node Minty::TextReaderBehavior::read_node(const void* const data, Size const size) const
{
	// get contents of file as text for parsing
	std::vector<Char> contents(size + 1);
	contents[size] = 0; // set 0 at the end for good measure
	memcpy(contents.data(), data, size);

	// get string
	String text(contents.data());

	// split lines
	std::vector<String> lines = split(text, "\n");

	// parse node
	int indent = 0;
	String key;
	String value;

	Node root;

	// if no lines, node is empty
	if (lines.empty())
	{
		return root;
	}

	// if first line starts with a ": ", then that is the data for the root node
	if (lines.front().starts_with(": "))
	{
		String temp = lines.front().substr(2, lines.front().size() - 2);
		root.set_data(temp.data(), temp.size());
	}

	std::vector<Node*> nodeStack;
	nodeStack.push_back(&root);
	Node* node = nodeStack.back();

	int const SPACES_PER_TAB = 4;

	for (String line : lines)
	{
		// skip empty/whitespace/comment lines
		Size solidIndex = line.find_first_not_of(" \t\n\v\f\r");
		if (line.size() == 0 || solidIndex == String::npos || line.front() == '#' || line.front() == ':')
		{
			continue;
		}

		// count number of tabs (indents)

		int spaces = 0;
		for (char const c : line)
		{
			if (c == ' ')
			{
				// add once space
				spaces += 1;
			}
			else if (c == '\t')
			{
				// multiple sapces
				spaces += SPACES_PER_TAB;
			}
			else
			{
				// done
				break;
			}
		}
		int i = spaces / SPACES_PER_TAB;

		int indentChange = i - indent;

		// if new indent is too deep, ignore
		if (indentChange > 1)
		{
			MINTY_WARN_FORMAT("Discarding line, invalid indent change of {}: {}", indentChange, line);
			continue;
		}

		// check change in index
		if (indentChange > 0)
		{
			nodeStack.push_back(&node->get_children().back());

			// start using that as active node
			node = nodeStack.back();

			// update indent
			indent = i;
		}
		else if (indentChange < 0)
		{
			// going down, so pop down X nodes, where X is the difference between indents
			for (; indentChange < 0; indentChange++)
			{
				nodeStack.pop_back();
			}

			// update node reference
			node = nodeStack.back();

			// update indent
			indent = i;
		}

		// remove indents for parsing
		if (spaces > 0)
		{
			line = line.substr(solidIndex, line.size() - solidIndex);
		}

		Node newNode;

		if (line.starts_with("- "))
		{
			// bullet point, use "" as key and the whole line as the value
			key = "";
			value = line.substr(2, line.size() - 2);
			value = replace(value, "\\n", "\n");
			newNode.set_data(value);
		}
		else
		{
			// split by colon, if there is one
			Size split = line.find_first_of(':');

			if (split == String::npos)
			{
				// no split, just key
				key = line;
			}
			else
			{
				// split: implies key: value
				key = line.substr(0, split);
				// ignore ": "
				Size size = line.size() - split - 2;
				if (split < line.size() - 2)
				{
					// something on other side of ": "
					value = line.substr(split + 2, size);
				}
				else
				{
					// nothing on other side of the ": "
					value = "";
				}
				value = replace(value, "\\n", "\n");
				newNode.set_data(value);
			}
		}

		// set newNode name to key
		newNode.set_name(key);

		// get index of newNode
		Size index = node->get_children().size();

		// add node to children
		node->add_child(key, newNode);
	}

	return root;
}

Bool Minty::Reader::read_object(String const& name, Serializable& data)
{
	if (indent(name))
	{
		data.deserialize(*this);

		outdent();

		return true;
	}

	return false;
}

Bool Minty::Reader::read_object(Size const index, Serializable& data)
{
	if (indent(index))
	{
		data.deserialize(*this);

		outdent();

		return true;
	}

	return false;
}

Bool Minty::Reader::read(Size const index, void* const data, Type const type) const
{
	switch (type)
	{
	case Type::Bool:
		return read_bool(index, *static_cast<Bool* const>(data));
	case Type::Bool2:
		return read_bool2(index, *static_cast<Bool2* const>(data));
	case Type::Bool3:
		return read_bool3(index, *static_cast<Bool3* const>(data));
	case Type::Bool4:
		return read_bool4(index, *static_cast<Bool4* const>(data));
	case Type::Char:
		return read_char(index, *static_cast<Char* const>(data));
	case Type::Byte:
		return read_byte(index, *static_cast<Byte* const>(data));
	case Type::Short:
		return read_short(index, *static_cast<Short* const>(data));
	case Type::UShort:
		return read_ushort(index, *static_cast<UShort* const>(data));
	case Type::Int:
		return read_int(index, *static_cast<Int* const>(data));
	case Type::Int2:
		return read_int2(index, *static_cast<Int2* const>(data));
	case Type::Int3:
		return read_int3(index, *static_cast<Int3* const>(data));
	case Type::Int4:
		return read_int4(index, *static_cast<Int4* const>(data));
	case Type::UInt:
		return read_uint(index, *static_cast<UInt* const>(data));
	case Type::UInt2:
		return read_uint2(index, *static_cast<UInt2* const>(data));
	case Type::UInt3:
		return read_uint3(index, *static_cast<UInt3* const>(data));
	case Type::UInt4:
		return read_uint4(index, *static_cast<UInt4* const>(data));
	case Type::Long:
		return read_long(index, *static_cast<Long* const>(data));
	case Type::ULong:
		return read_ulong(index, *static_cast<ULong* const>(data));
	case Type::Size:
		return read_size(index, *static_cast<Size* const>(data));
	case Type::Float:
		return read_float(index, *static_cast<Float* const>(data));
	case Type::Float2:
		return read_float2(index, *static_cast<Float2* const>(data));
	case Type::Float3:
		return read_float3(index, *static_cast<Float3* const>(data));
	case Type::Float4:
		return read_float4(index, *static_cast<Float4* const>(data));
	case Type::Double:
		return read_double(index, *static_cast<Double* const>(data));
	case Type::String:
		return read_string(index, *static_cast<String* const>(data));
	default:
		MINTY_ABORT("Cannot write type.");
	}
}

Bool Minty::Reader::read(String const& name, void* const data, Type const type) const
{
	switch (type)
	{
	case Type::Bool:
		return read_bool(name, *static_cast<Bool* const>(data));
	case Type::Bool2:
		return read_bool2(name, *static_cast<Bool2* const>(data));
	case Type::Bool3:
		return read_bool3(name, *static_cast<Bool3* const>(data));
	case Type::Bool4:
		return read_bool4(name, *static_cast<Bool4* const>(data));
	case Type::Char:
		return read_char(name, *static_cast<Char* const>(data));
	case Type::Byte:
		return read_byte(name, *static_cast<Byte* const>(data));
	case Type::Short:
		return read_short(name, *static_cast<Short* const>(data));
	case Type::UShort:
		return read_ushort(name, *static_cast<UShort* const>(data));
	case Type::Int:
		return read_int(name, *static_cast<Int* const>(data));
	case Type::Int2:
		return read_int2(name, *static_cast<Int2* const>(data));
	case Type::Int3:
		return read_int3(name, *static_cast<Int3* const>(data));
	case Type::Int4:
		return read_int4(name, *static_cast<Int4* const>(data));
	case Type::UInt:
		return read_uint(name, *static_cast<UInt* const>(data));
	case Type::UInt2:
		return read_uint2(name, *static_cast<UInt2* const>(data));
	case Type::UInt3:
		return read_uint3(name, *static_cast<UInt3* const>(data));
	case Type::UInt4:
		return read_uint4(name, *static_cast<UInt4* const>(data));
	case Type::Long:
		return read_long(name, *static_cast<Long* const>(data));
	case Type::ULong:
		return read_ulong(name, *static_cast<ULong* const>(data));
	case Type::Size:
		return read_size(name, *static_cast<Size* const>(data));
	case Type::Float:
		return read_float(name, *static_cast<Float* const>(data));
	case Type::Float2:
		return read_float2(name, *static_cast<Float2* const>(data));
	case Type::Float3:
		return read_float3(name, *static_cast<Float3* const>(data));
	case Type::Float4:
		return read_float4(name, *static_cast<Float4* const>(data));
	case Type::Double:
		return read_double(name, *static_cast<Double* const>(data));
	case Type::String:
		return read_string(name, *static_cast<String* const>(data));
	default:
		MINTY_ABORT_FORMAT("Cannot read type \"{}\".", to_string(type));
	}
}
