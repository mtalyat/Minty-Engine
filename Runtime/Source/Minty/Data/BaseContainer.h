#pragma once
#include "Minty/Data/Container.h"

namespace Minty
{
	// a container of bytes
	class BaseContainer
		: public Container
	{
	protected:
		Byte* mp_data;
		Size m_capacity;
		Size m_size;

	public:
		BaseContainer()
			: mp_data(nullptr)
			, m_capacity(0)
			, m_size(0)
		{}

		virtual ~BaseContainer()
		{
			reset();
		}

		BaseContainer(BaseContainer const& other)
			: mp_data(new Byte[other.m_capacity])
			, m_capacity(other.m_capacity)
			, m_size(other.m_size)
		{
			memcpy(mp_data, other.mp_data, m_size);
		}

		BaseContainer& operator=(BaseContainer const& other)
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

		BaseContainer(BaseContainer&& other) noexcept
			: mp_data(other.mp_data)
			, m_capacity(other.m_capacity)
			, m_size(other.m_size)
		{
			other.mp_data = nullptr;
			other.m_capacity = 0;
			other.m_size = 0;
		}

		BaseContainer& operator=(BaseContainer&& other) noexcept
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
		void set_at(void const* const data, Size const size, Size const index) override;

		// clears all data from the container
		void clear() override;

		// resets the entire container
		void reset() override;

		// sets the new size
		virtual Bool resize(Size const newSize);

		// gets the raw data array pointer
		void* data() const override { return mp_data; }

		// gets the size of the data within the data array
		Size size() const override { return m_size; }

		// gets the size of the data array in bytes
		Size capacity() const override { return m_capacity; }

		// gets the data as a specific type
		template<typename T>
		T get() const
		{
			return *static_cast<T*>(static_cast<void*>(mp_data));
		}
	};
}