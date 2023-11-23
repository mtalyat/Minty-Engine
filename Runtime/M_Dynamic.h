#pragma once

#include "M_Base.h"

namespace minty
{
	/// <summary>
	/// Holds a dynamic set of data, where its type can be unknown at compile and runtime.
	/// </summary>
	class Dynamic
	{
	private:
		void* _data;
		size_t _size;

	public:
		Dynamic();

		Dynamic(void* const data, size_t const size);

		// copy
		Dynamic(Dynamic const& other);

		// copy
		Dynamic& operator=(Dynamic const& other);

		// move
		Dynamic(Dynamic&& other);

		// move
		Dynamic& operator=(Dynamic&& other);

		~Dynamic();

		template<typename T>
		void set(T* const data, size_t const count = 1);
		
		void set(void* const data, size_t const size);

		template<typename T>
		T* get() const;

		void* data() const;

		size_t size() const;

		void clear();
	};
	template<typename T>
	void Dynamic::set(T* const data, size_t const count)
	{
		set(data, sizeof(T) * count);
	}
	template<typename T>
	T* Dynamic::get() const
	{
		return static_cast<T>(_data);
	}
}