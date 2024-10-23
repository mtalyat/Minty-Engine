#pragma once

#include "Minty/Core/Type.h"

namespace Minty
{
	// a static container of bytes
	class Container
	{
	protected:
		Byte* mp_data;
		Size m_capacity;
		Size m_size;

	public:
		Container()
			: mp_data(nullptr)
			, m_capacity(0)
			, m_size(0)
		{}

		virtual ~Container()
		{
			reset();
		}

		Container(Container const& other)
			: mp_data(new Byte[other.m_capacity])
			, m_capacity(other.m_capacity)
			, m_size(other.m_size)
		{
			memcpy(mp_data, other.mp_data, m_size);
		}

		Container& operator=(Container const& other)
		{
			if (this != &other)
			{
				if (mp_data) delete[] mp_data;

				mp_data = new Byte[other.m_capacity];
				m_capacity = other.m_capacity;
				m_size = other.m_size;
				memcpy(mp_data, other.mp_data, m_size);
			}

			return *this;
		}

		Container(Container&& other) noexcept
			: mp_data(other.mp_data)
			, m_capacity(other.m_capacity)
			, m_size(other.m_size)
		{
			other.mp_data = nullptr;
			other.m_capacity = 0;
			other.m_size = 0;
		}

		Container& operator=(Container&& other) noexcept
		{
			if (this != &other)
			{
				mp_data = other.mp_data;
				m_capacity = other.m_capacity;
				m_size = other.m_size;

				other.mp_data = nullptr;
				other.m_capacity = 0;
				other.m_size = 0;
			}

			return *this;
		}

	public:
		// sets data at the given offset, if it fits
		void set_at(void const* const data, Size const size, Size const index);

		// overrides all of the data in this Container
		void set(void const* const data, Size const size);

		// adds to the end of the data within this container, and reserves more space if needed
		virtual Bool append(void const* const data, Size const size) = 0;

		// clears all data from the container
		void clear();

		// resets the entire container
		void reset();

		// sets the new capacity
		virtual Bool reserve(Size const newCapacity) = 0;

		// sets the new size
		virtual Bool resize(Size const newSize);

		// gets the raw data array pointer
		void* data() const { return mp_data; }

		// gets the size of the data within the data array
		Size size() const { return m_size; }

		// gets the size of the data array in bytes
		Size capacity() const { return m_capacity; }

		// gets the data as a specific type
		template<typename T>
		T get() const
		{
			return *static_cast<T*>(static_cast<void*>(mp_data));
		}
	};
}