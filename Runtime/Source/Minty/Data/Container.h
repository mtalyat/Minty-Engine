#pragma once

#include "Minty/Core/Type.h"

namespace Minty
{
	// a container of bytes
	class Container
	{
	public:
		Container() = default;

		virtual ~Container() = default;

	public:
		// sets data at the given offset, if it fits
		virtual void set_at(void const* const data, Size const size, Size const index) = 0;

		// overrides all of the data in this Container
		void set(void const* const data, Size const size);

		// adds to the end of the data within this container, and reserves more space if needed
		virtual Bool append(void const* const data, Size const size) = 0;

		// clears all data from the container
		virtual void clear() = 0;

		// resets the entire container
		virtual void reset() = 0;

		// sets the new capacity
		virtual Bool reserve(Size const newCapacity) = 0;

		// sets the new size
		virtual Bool resize(Size const newSize) = 0;

		// gets the raw data array pointer
		virtual void* data() const = 0;

		// gets the size of the data within the data array
		virtual Size size() const = 0;

		// gets the size of the data array in bytes
		virtual Size capacity() const = 0;
	};
}