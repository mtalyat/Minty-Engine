#include "pch.h"
#include "DynamicContainer.h"

#include "Minty/Core/Macros.h"

using namespace Minty;

Bool Minty::DynamicContainer::append(void const* const data, Size const size)
{
	MINTY_ASSERT_MESSAGE(data != nullptr, "Cannot append nullptr data.");
	MINTY_ASSERT_MESSAGE(size > 0, "Cannot append data without a size.");

	// if new size will surpass the capacity, double the capacity
	Size newSize = m_size + size;

	if (newSize > m_capacity)
	{
		if (m_capacity)
		{
			Size newCapacity;

			// reserve double current capacity, or more if needed
			if (newSize > (m_capacity << 1))
			{
				newCapacity = newSize;
			}
			else
			{
				newCapacity = m_capacity << 1;
			}

			reserve(newCapacity);
		}
		else
		{
			// no capacity yet, just allocate what is needed
			reserve(newSize);
		}
	}

	Size index = m_size;

	// resize first
	resize(newSize);

	// set data in the new space
	set_at(data, size, index);

	return true;
}

Bool Minty::DynamicContainer::reserve(Size const newCapacity)
{
	// ignore if below capacity
	if (newCapacity <= m_capacity) return true;

	// allocate a new array
	Byte* newData = new Byte[newCapacity];

	// copy over existing data, if any
	if (mp_data)
	{
		memcpy(newData, mp_data, m_size);

		// delete old array
		delete[] mp_data;
	}

	// update reference and data
	mp_data = newData;
	m_capacity = newCapacity;

	return true;
}
