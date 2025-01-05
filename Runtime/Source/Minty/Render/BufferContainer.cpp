#include "pch.h"
#include "BufferContainer.h"

using namespace Minty;

void Minty::BufferContainer::set_at(void const* const data, Size const size, Size const index)
{
	MINTY_ASSERT_MESSAGE(data != nullptr, "Cannot set nullptr data.");
	MINTY_ASSERT_MESSAGE(index + size <= m_size, "Cannot set data that exceeds the size of the container.");

	Byte* containerData = static_cast<Byte*>(m_buffer->data());

	memcpy(&containerData[index], data, size);
}

Bool Minty::BufferContainer::append(void const* const data, Size const size)
{
	MINTY_ASSERT_MESSAGE(data != nullptr, "Cannot append nullptr data.");
	MINTY_ASSERT_MESSAGE(size > 0, "Cannot append data without a size.");

	// if new size will surpass the capacity, double the capacity
	Size newSize = m_size + size;

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

	Size index = m_size;

	// resize first
	resize(newSize);

	// set data in the new space
	set_at(data, size, index);

	return true;
}

void Minty::BufferContainer::clear()
{
	m_size = 0;
}

void Minty::BufferContainer::reset()
{
	if (m_buffer != nullptr)
	{
		m_buffer = nullptr;
	}
	m_capacity = 0;
	m_size = 0;
}

Bool Minty::BufferContainer::reserve(Size const newCapacity)
{
	// ignore if below capacity
	if (newCapacity <= m_capacity) return true;

	// allocate a new buffer, using this buffer's current data, if there is one
	BufferBuilder builder{};
	if (m_buffer != nullptr)
	{
		builder.data = m_buffer->data();
	}
	builder.frequent = true;
	builder.size = newCapacity;
	builder.usage = m_usage;

	Owner<Buffer> newBuffer = Buffer::create(builder);

	// update reference and data
	m_buffer = newBuffer;
	m_capacity = newCapacity;

	return true;
}

Bool Minty::BufferContainer::resize(Size const newSize)
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