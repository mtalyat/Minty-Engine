#pragma once
#include "Minty/Data/BaseContainer.h"

namespace Minty
{
	/// <summary>
	/// A ConstantContainer is a container with a fixed size.
	/// </summary>
	class ConstantContainer
		: public BaseContainer
	{
	public:
		ConstantContainer()
			: BaseContainer()
		{}

		ConstantContainer(Size const size)
			: BaseContainer()
		{
			if (size)
			{
				mp_data = new Byte[size];
				m_capacity = size;
				m_size = size;
			}
		}

		ConstantContainer(void const* const data, Size const size)
			: BaseContainer()
		{
			if (data && size)
			{
				mp_data = new Byte[size];
				m_capacity = size;
				m_size = size;
				memcpy(mp_data, data, size);
			}
		}

		ConstantContainer(ConstantContainer const& other)
			: BaseContainer(other)
		{}

		ConstantContainer& operator=(ConstantContainer const& other)
		{
			if (this != &other)
			{
				BaseContainer::operator=(other);
			}

			return *this;
		}

		ConstantContainer(ConstantContainer&& other) noexcept
			: BaseContainer(other)
		{}

		ConstantContainer& operator=(ConstantContainer&& other) noexcept
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