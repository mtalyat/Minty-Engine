#include "pch.h"
#include "BaseContainer.h"

#include "Minty/Core/Macros.h"

using namespace Minty;

void Minty::BaseContainer::set_at(void const* const data, Size const size, Size const index)
{
	MINTY_ASSERT_MESSAGE(data != nullptr, "Cannot set nullptr data.");
	MINTY_ASSERT_MESSAGE(index + size <= m_size, "Cannot set data that exceeds the size of the container.");

	// copy into data
	memcpy(&mp_data[index], data, size);
}

void Minty::BaseContainer::clear()
{
	m_size = 0;
}

void Minty::BaseContainer::reset()
{
	if (mp_data != nullptr)
	{
		delete[] mp_data;
		mp_data = nullptr;
	}
	m_capacity = 0;
	m_size = 0;
}

Bool Minty::BaseContainer::resize(Size const newSize)
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
