#pragma once
#include "Minty/Data/BaseContainer.h"

namespace Minty
{
	// a static container of bytes
	class StaticContainer
		: public BaseContainer
	{
	public:
		StaticContainer()
			: BaseContainer()
		{}

		StaticContainer(Size const size)
			: BaseContainer()
		{
			if (size)
			{
				mp_data = new Byte[size];
				m_size = size;
				m_capacity = size;
			}
		}

		StaticContainer(StaticContainer const& other)
			: BaseContainer(other)
		{}

		StaticContainer& operator=(StaticContainer const& other)
		{
			if (this != &other)
			{
				BaseContainer::operator=(other);
			}

			return *this;
		}

		StaticContainer(StaticContainer&& other) noexcept
			: BaseContainer(other)
		{}

		StaticContainer& operator=(StaticContainer&& other) noexcept
		{
			if (this != &other)
			{
				BaseContainer::operator=(std::move(other));
			}

			return *this;
		}

	public:
		Bool resize(Size const newSize) override;
		Bool append(void const* const data, Size const size) override;
		Bool reserve(Size const newCapacity) override;
	};
}