#include "pch.h"
#include "M_Dynamic.h"

#include "M_Console.h"
#include "M_Parse.h"
#include "M_Encoding.h"

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

	// if no data given, do nothing
	if (!data || !size)
	{
		return;
	}

	// allocate space
	_data = malloc(size);

	// ensure properly allocated
	if (!_data)
	{
		console::error("Failed to malloc in Dynamic set().");
		return;
	}

	// copy data over
	memcpy(_data, data, size);
	_size = size;
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
	// base64 encode the data, do that
	writer.write("size", _size);
	writer.write("data", encoding::encode_base64(*this));
}

void minty::Dynamic::deserialize(Reader const& reader)
{
	// get size in bytes
	size_t size = reader.read_size("size", reader.to_size());

	// if no size, all done
	if (!size)
	{
		clear();
		return;
	}

	// use node directly to have more control since this deserialization is tricky
	Node const& node = reader.get_node();

	// if there is the raw data base64 encoded, just read that and be done
	if (Node const* dataNode = node.find("data"))
	{
		Dynamic data = encoding::decode_base64(dataNode->to_string());
		set(data.data(), data.size());
		return;
	}

	// manual byte entry:

	// go through and get all possible combinations of bytes/bits, pack into data
	Byte* data = new Byte[size];
	memset(data, 0, size);

	// assume 'x' is any number, and 'y' is another number:
	// x refers to the byte at x
	// xi refers to an integer starting at byte x
	// xui refers to an unsigned integer starting at byte x
	// xf refers to a float starting at byte f
	// xsy refers to a string at x of y length

	size_t i;
	for (auto const& child : node.get_children())
	{
		String const& name = child.get_name();
		if (name.empty() || !std::isdigit(name.front()))
		{
			// does not start with byte location, so, ignore
			continue;
		}

		// get type
		if (std::isdigit(name.back()))
		{
			size_t sIndex = name.rfind('s');
			if (sIndex == std::string::npos)
			{
				// byte
				i = parse::to_size(name);
				Byte temp = child.to_byte();
				memcpy(&data[i], &temp, sizeof(Byte));
			}
			else
			{
				// string
				size_t stringSize = parse::to_size(name.substr(sIndex + 1, name.length() - 1 - sIndex));
				String temp = child.to_string();
				memcpy(&data[i], temp.data(), sizeof(char) * stringSize);
				data[i + stringSize - 1] = '\0';
			}
		}
		else if (name.ends_with("ui"))
		{
			// unsigned int
			i = parse::to_size(name.substr(0, name.size() - 2));
			unsigned int temp = child.to_uint();
			memcpy(&data[i], &temp, sizeof(unsigned int));
		}
		else if (name.ends_with("i"))
		{
			// int
			i = parse::to_size(name.substr(0, name.size() - 1));
			int temp = child.to_int();
			memcpy(&data[i], &temp, sizeof(int));
		}
		else if (name.ends_with("f"))
		{
			// float
			i = parse::to_size(name.substr(0, name.size() - 1));
			float temp = child.to_float();
			memcpy(&data[i], &temp, sizeof(float));
		}
		else
		{
			console::todo(std::format("Dynamic::deserialize(): unknown token: {}.", name));
		}
	}

	// set new data
	set(data, size);

	// clean up
	delete[] data;
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
