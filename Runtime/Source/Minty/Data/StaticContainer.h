#pragma once
#include "Minty/Data/Container.h"

namespace Minty
{
	// a static container of bytes
	class StaticContainer
		: public Container
	{
	public:
		StaticContainer()
			: Container()
		{}

		StaticContainer(Size const size)
			: Container()
		{
			if (size)
			{
				mp_data = new Byte[size];
				m_size = size;
				m_capacity = size;
			}
		}

		StaticContainer(StaticContainer const& other)
			: Container(other)
		{}

		StaticContainer& operator=(StaticContainer const& other)
		{
			if (this != &other)
			{
				Container::operator=(other);
			}

			return *this;
		}

		StaticContainer(StaticContainer&& other) noexcept
			: Container(other)
		{}

		StaticContainer& operator=(StaticContainer&& other) noexcept
		{
			if (this != &other)
			{
				Container::operator=(std::move(other));
			}

			return *this;
		}

	public:
		Bool resize(Size const newSize) override;
		Bool append(void const* const data, Size const size) override;
		Bool reserve(Size const newCapacity) override;
	};
}