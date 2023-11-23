#include "pch.h"
#include "M_Dynamic.h"

#include "M_Console.h"

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

minty::Dynamic::Dynamic(Dynamic&& other)
	: _data(other._data)
	, _size(other._size)
{
	other._data = nullptr;
	other._size = 0;
}

Dynamic& minty::Dynamic::operator=(Dynamic&& other)
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
