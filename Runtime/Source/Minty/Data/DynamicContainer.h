#pragma once
#include "Minty/Data/BaseContainer.h"

namespace Minty
{
	// a dynamic container of bytes
	class DynamicContainer
		: public BaseContainer
	{
	public:
		DynamicContainer()
			: BaseContainer()
		{}

		DynamicContainer(Size const capacity)
			: BaseContainer()
		{
			reserve(capacity);
		}

		DynamicContainer(DynamicContainer const& other)
			: BaseContainer(other)
		{}

		DynamicContainer& operator=(DynamicContainer const& other)
		{
			if (this != &other)
			{
				BaseContainer::operator=(other);
			}

			return *this;
		}

		DynamicContainer(DynamicContainer&& other) noexcept
			: BaseContainer(other)
		{}

		DynamicContainer& operator=(DynamicContainer&& other) noexcept
		{
			if (this != &other)
			{
				BaseContainer::operator=(std::move(other));
			}

			return *this;
		}

	public:
		Bool append(void const* const data, Size const size) override;
		Bool reserve(Size const newCapacity) override;
	};
}