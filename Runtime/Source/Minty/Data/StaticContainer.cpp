#include "pch.h"
#include "StaticContainer.h"

#include "Minty/Core/Macros.h"
#include "Minty/Core/Math.h"

using namespace Minty;

Bool Minty::StaticContainer::resize(Size const newSize)
{
	// do nothing if already that size
	if (newSize == m_size) return false;

	// capacity and size always match
	Byte* newData = new Byte[newSize];

	// copy over data
	Size size = Math::min(m_size, newSize);
	memcpy(newData, mp_data, size);

	// delete old data
	delete[] mp_data;

	// assign new data and info
	mp_data = newData;
	m_size = newSize;
	m_capacity = newSize;

	return true;
}

Bool Minty::StaticContainer::append(void const* const data, Size const size)
{
	// cannot append to a static container
	return false;
}

Bool Minty::StaticContainer::reserve(Size const newCapacity)
{
	// cannot reserve a static container
	return false;
}
