#pragma once
#include "types/M_Object.h"

namespace minty
{
	/// <summary>
	/// Holds a dynamic set of data, where its type can be unknown at compile and runtime.
	/// </summary>
	class Dynamic
		: public Object
	{
	private:
		void* _data;
		size_t _size;

	public:
		/// <summary>
		/// Creates an empty Dynamic.
		/// </summary>
		Dynamic();

		/// <summary>
		/// Creates a Dynamic with the given data.
		/// </summary>
		/// <param name="data">The data to set.</param>
		/// <param name="size">The size of the data in bytes.</param>
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

		friend bool operator==(Dynamic const& left, Dynamic const& right);

		friend bool operator!=(Dynamic const& left, Dynamic const& right);

		/// <summary>
		/// Sets the data within this Dynamic.
		/// </summary>
		/// <typeparam name="T">The type of data to set.</typeparam>
		/// <param name="data">The data to set.</param>
		template<typename T>
		void set(T* const data)
		{
			set(static_cast<void*>(data), sizeof(T));
		}

		/// <summary>
		/// Sets the data within this Dynamic.
		/// </summary>
		/// <typeparam name="T">The type of data to set.</typeparam>
		/// <param name="data">The data to set.</param>
		/// <param name="count">The number of elements of the data.</param>
		template<typename T>
		void set_array(T* const data, size_t const count)
		{
			set(static_cast<void*>(data), sizeof(T) * count);
		}
		
		/// <summary>
		/// Sets the data within this Dynamic.
		/// </summary>
		/// <param name="data">The data to set.</param>
		/// <param name="size">The size of the data in bytes.</param>
		void set(void* const data, size_t const size);

		/// <summary>
		/// Gets a pointer to the data.
		/// </summary>
		/// <typeparam name="T">The type of data.</typeparam>
		/// <returns>A pointer to the data within this Dynamic.</returns>
		template<typename T>
		T get() const
		{
			return *static_cast<T*>(_data);
		}

		String get_string() const;

		/// <summary>
		/// Gets the pointer to the data.
		/// </summary>
		/// <returns>A pointer to the data within this Dynamic.</returns>
		void* data() const;

		/// <summary>
		/// Gets the size of the data.
		/// </summary>
		/// <returns>The size of the data in bytes.</returns>
		size_t size() const;

		/// <summary>
		/// Clears all data.
		/// </summary>
		void clear();

		void serialize(Writer& writer) const;
		void deserialize(Reader const& reader);

		friend std::ostream& operator<<(std::ostream& stream, Dynamic const& object);
		friend std::istream& operator>>(std::istream& stream, Dynamic& object);
	};
}