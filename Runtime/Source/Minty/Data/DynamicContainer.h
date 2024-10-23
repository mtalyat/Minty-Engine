#pragma once
#include "Minty/Data/Container.h"

namespace Minty
{
	// a dynamic container of bytes
	class DynamicContainer
		: public Container
	{
	public:
		DynamicContainer()
			: Container()
		{}

		DynamicContainer(Size const capacity)
			: Container()
		{
			reserve(capacity);
		}

		DynamicContainer(DynamicContainer const& other)
			: Container(other)
		{}

		DynamicContainer& operator=(DynamicContainer const& other)
		{
			if (this != &other)
			{
				Container::operator=(other);
			}

			return *this;
		}

		DynamicContainer(DynamicContainer&& other) noexcept
			: Container(other)
		{}

		DynamicContainer& operator=(DynamicContainer&& other) noexcept
		{
			if (this != &other)
			{
				Container::operator=(std::move(other));
			}

			return *this;
		}

	public:
		Bool append(void const* const data, Size const size) override;
		Bool reserve(Size const newCapacity) override;
	};
}