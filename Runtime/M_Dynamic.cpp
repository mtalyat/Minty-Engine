#include "pch.h"
#include "M_Dynamic.h"

#include "M_Console.h"
#include "M_Parse.h"
#include "M_Encoding.h"
#include "M_Vector.h"
#include "M_Quaternion.h"
#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

minty::Dynamic::Dynamic()
	: _data(nullptr)
	, _size(0)
{}

minty::Dynamic::Dynamic(void* const data, size_t const size)
	: _data(nullptr)
	, _size(0)
{
	set(data, size);
}

minty::Dynamic::Dynamic(Dynamic const& other)
	: _data(nullptr)
	, _size(0)
{
	set(other._data, other._size);
}

Dynamic& minty::Dynamic::operator=(Dynamic const& other)
{
	if (&other != this)
	{
		set(other._data, other._size);
	}

	return *this;
}

minty::Dynamic::Dynamic(Dynamic&& other) noexcept
	: _data(other._data)
	, _size(other._size)
{
	other._data = nullptr;
	other._size = 0;
}

Dynamic& minty::Dynamic::operator=(Dynamic&& other) noexcept
{
	_data = other._data;
	_size = other._size;

	other._data = nullptr;
	other._size = 0;

	return *this;
}

minty::Dynamic::~Dynamic()
{
	if (_data)
	{
		free(_data);
	}
}

void minty::Dynamic::set(void* const data, size_t const size)
{
	clear();

	// if no size, do nothing
	if (!size)
	{
		return;
	}

	// allocate space
	_data = malloc(size);

	// ensure properly allocated
	if (!_data)
	{
		Console::error("Failed to malloc in Dynamic set().");
		return;
	}

	// if no data to copy, set to zeros, otherwise copy
	if (data)
	{
		// copy data over
		memcpy(_data, data, size);
	}
	else
	{
		// set to zeros
		memset(_data, 0, size);
	}
	
	_size = size;
}

String minty::Dynamic::get_string() const
{
	// make appropriate char array
	char* temp = new char[_size + 1];

	// copy data over
	memcpy(temp, _data, _size);

	// set last char to '\0'
	temp[_size] = '\0';

	// turn that into a string
	String out(temp);

	// clean up
	delete[] temp;

	return out;
}

void* minty::Dynamic::data() const
{
	return _data;
}

size_t minty::Dynamic::size() const
{
	return _size;
}

void minty::Dynamic::clear()
{
	// if data already set, free it
	if (_data)
	{
		free(_data);
	}

	_data = nullptr;
	_size = 0;
}

void minty::Dynamic::serialize(Writer& writer) const
{
	// write this data as a string
	std::stringstream stream;
	stream << *this;
	writer.get_node().set_data(stream.str());
}

void minty::Dynamic::deserialize(Reader const& reader)
{
	Node const& node = reader.get_node();

	// just decode the raw data
	if (node.has_data())
	{
		std::stringstream stream(node.get_data());
		stream >> *this;
		return;
	}

	Dynamic data;

	// no raw data given, so go through the values and set directly
	std::vector<Dynamic> values;
	values.reserve(node.get_children().size());

	// total size of all the read values
	size_t size = 0;

	for (Node const& child : node.get_children())
	{
		std::stringstream stream(child.get_name());
		stream >> data;
		size += data.size();
		values.push_back(data);
	}

	// now compile them all into one big array
	Byte* bytes = new Byte[size];

	// keep track of the offset
	size_t offset = 0;

	// copy each value over in order
	for (Dynamic const& value : values)
	{
		memcpy(bytes + offset, value.data(), value.size());

		offset += value.size();
	}

	// lastly, set all to this data
	set(bytes, size);

	// done with this byte array, delete it
	delete[] bytes;
}

bool minty::operator==(Dynamic const& left, Dynamic const& right)
{
	// equal if the same address OR size and data are equal
	return &left == &right || (left._size == right._size && !memcmp(left._data, right._data, left._size));
}

bool minty::operator!=(Dynamic const& left, Dynamic const& right)
{
	return !(left == right);
}

std::ostream& minty::operator<<(std::ostream& stream, Dynamic const& object)
{
	// output as base 64
	stream << Encoding::encode_base64(object);

	return stream;
}

std::istream& minty::operator>>(std::istream& stream, Dynamic& object)
{
	// input as a variety of types:
	/*
		b/ub: byte
		sb: signed byte
		c: char
		str: string
		s: short
		us: unsigned short
		i: int
		ui: unsigned int
		l: long
		ul: unsigned long
		e: empty bytes of size
		S: String
		ID: UUID
		V2: Vector2
		V3: Vector3
		V4: Vector4
		V2I: Vector2Int
		V3I: Vector3Int
		V4I: Vector4Int
		Q: Quaternion
		(no type given): base 64 encoded string
	*/

	// read in the type
	String type;
	stream >> type;

	// decide what to do based on that
	if (type.starts_with("S")) // string
	{
		// skip one char (should be a space)
		stream.get();

		// get value
		String temp;
		//stream >> temp;
		std::getline(stream, temp);
		size_t size = temp.size();

		// get size from type, if applicable
		if (type.size() > 1)
		{
			// get new size from type after the 'S'
			size = Parse::to_size(type.substr(1, type.size() - 1));

			// resize string
			temp.resize(size);
		}

		// set data to string
		object.set(temp.data(), size * sizeof(char));
	}
	else if (type == "i") // int
	{
		int temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "ui") // unsigned int
	{
		unsigned int temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "f") // float
	{
		float temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "d") // double
	{
		double temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "b" || type == "ub") // (unsigned) byte
	{
		unsigned char temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "sb") // signed byte
	{
		signed char temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "c") // char
	{
		char temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "s") // short
	{
		short temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "us") // unsigned short
	{
		unsigned short temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "l") // long
	{
		long temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "ul") // unsigned long
	{
		unsigned long temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "e")
	{
		size_t temp;
		stream >> temp;
		object.set(nullptr, temp);
	}
	else if (type == "ID")
	{
		UUID temp(INVALID_UUID);
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "V2")
	{
		Vector2 temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "V3")
	{
		Vector3 temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "V4")
	{
		Vector4 temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "V2I")
	{
		Vector2Int temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "V3I")
	{
		Vector3Int temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "V4I")
	{
		Vector4Int temp;
		stream >> temp;
		object.set(&temp);
	}
	else if (type == "Q")
	{
		Quaternion temp;
		stream >> temp;
		object.set(&temp);
	}
	else
	{
		// assume base64 encoding, but inside of type
		object = Encoding::decode_base64(type);
	}

	return stream;
}