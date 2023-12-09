#pragma once

#include "M_Base.h"
#include "M_ISerializable.h"

namespace minty
{
	/// <summary>
	/// Holds a dynamic set of data, where its type can be unknown at compile and runtime.
	/// </summary>
	class Dynamic
		: public ISerializable
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
		Dynamic(Dynamic&& other) noexcept;

		// move
		Dynamic& operator=(Dynamic&& other) noexcept;

		~Dynamic();

		template<typename T>
		void set(T* const data, size_t const count = 1);
		
		void set(void* const data, size_t const size);

		template<typename T>
		T* get() const;

		void* data() const;

		size_t size() const;

		void clear();

		void serialize(Writer& writer) const;

		void deserialize(Reader const& reader);
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