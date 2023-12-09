#include "pch.h"
#include "M_Dynamic.h"

#include "M_Console.h"
#include "M_Parse.h"

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
	console::todo("Dynamic::serialize");
}

void minty::Dynamic::deserialize(Reader const& reader)
{
	// use node directly to have more control since this deserialization is tricky
	Node const* node = reader.get_node();

	// get size in bytes
	size_t size = node->get_size_t("size", node->to_size_t());

	// if no size, all done
	if (!size)
	{
		clear();
		return;
	}

	// go through and get all possible combinations of bytes/bits, pack into data
	byte* data = new byte[size];

	// assume 'x' is any number:
	// x refers to the byte at x
	// xi refers to an integer starting at byte x
	// xui refers to an unsigned integer starting at byte x
	// xf refers to a float starting at byte f

	size_t i;
	std::string name;
	for (auto const& childPair : node->children)
	{
		name = childPair.first;
		if (name.empty() || !std::isdigit(name.front()))
		{
			// does not start with byte location, so, ignore
			continue;
		}

		// get type
		if (std::isdigit(name.back()))
		{
			// byte
			i = parse::to_size_t(name);
			byte temp = childPair.second.front().to_byte();
			memcpy(&data[i], &temp, sizeof(byte));
		}
		else if (name.ends_with("ui"))
		{
			// unsigned int
			i = parse::to_size_t(name.substr(0, name.size() - 2));
			unsigned int temp = childPair.second.front().to_uint();
			memcpy(&data[i], &temp, sizeof(unsigned int));
		}
		else if (name.ends_with("i"))
		{
			// int
			i = parse::to_size_t(name.substr(0, name.size() - 1));
			int temp = childPair.second.front().to_int();
			memcpy(&data[i], &temp, sizeof(int));
		}
		else if (name.ends_with("f"))
		{
			// float
			i = parse::to_size_t(name.substr(0, name.size() - 1));
			float temp = childPair.second.front().to_float();
			memcpy(&data[i], &temp, sizeof(float));
		}
		else
		{

		}
	}

	/*Node const* n;
	std::string x;
	for (size_t i = 0; i < size; i++)
	{
		x = std::to_string(i);
		if (n = node->find(x))
		{
			
		}
		else if (n = node->find(x + "i"))
		{
			int temp = n->to_int();
			memcpy(&data[i], &temp, sizeof(int));
		}
		else if (n = node->find(x + "ui"))
		{
			unsigned int temp = n->to_uint();
			memcpy(&data[i], &temp, sizeof(unsigned int));
		}
		else if (n = node->find(x + "f"))
		{
			float temp = n->to_float();
			memcpy(&data[i], &temp, sizeof(float));
		}
	}*/


	// set new data
	set(data, size);

	// clean up
	delete[] data;
}
