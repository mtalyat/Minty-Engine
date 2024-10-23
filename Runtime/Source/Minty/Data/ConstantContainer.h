#pragma once
#include "Minty/Data/Container.h"

namespace Minty
{
	/// <summary>
	/// A ConstantContainer is a container with a fixed size.
	/// </summary>
	class ConstantContainer
		: public Container
	{
	public:
		ConstantContainer()
			: Container()
		{}

		ConstantContainer(Size const size)
			: Container()
		{
			if (size)
			{
				mp_data = new Byte[size];
				m_capacity = size;
				m_size = size;
			}
		}

		ConstantContainer(void const* const data, Size const size)
			: Container()
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
			: Container(other)
		{}

		ConstantContainer& operator=(ConstantContainer const& other)
		{
			if (this != &other)
			{
				Container::operator=(other);
			}

			return *this;
		}

		ConstantContainer(ConstantContainer&& other) noexcept
			: Container(other)
		{}

		ConstantContainer& operator=(ConstantContainer&& other) noexcept
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