#include "pch.h"
#include "Container.h"

#include "Minty/Core/Macros.h"

using namespace Minty;

void Minty::Container::set_at(void const* const data, Size const size, Size const index)
{
	if (size == 0) return;

	MINTY_ASSERT_MESSAGE(data != nullptr, "Cannot set nullptr data.");
	MINTY_ASSERT_MESSAGE(index + size <= m_size, "Cannot set data that exceeds the size of the container.");

	// copy into data
	memcpy(&mp_data[index], data, size);
}

void Minty::Container::set(void const* const data, Size const size)
{
	// resize to new size
	resize(size);

	// set at 0
	set_at(data, size, 0);
}

void Minty::Container::clear()
{
	m_size = 0;
}

void Minty::Container::reset()
{
	if (mp_data != nullptr)
	{
		delete[] mp_data;
		mp_data = nullptr;
	}
	m_capacity = 0;
	m_size = 0;
}

Bool Minty::Container::resize(Size const newSize)
{
	if (newSize > m_capacity)
	{
		// need to resize: try double capacity or new size, whichever is larger
		Size newCapacity = m_capacity << 1;
		if (newSize > newCapacity) newCapacity = newSize;

		// resize if able
		if (!reserve(newCapacity)) return false;
	}

	// set size
	m_size = newSize;

	return true;
}
