#pragma once
#include "Minty/Core/Macros.h"
#include "Minty/Core/Types.h"

namespace Minty
{
	/// <summary>
	/// Holds smart pointer count data for weak and strong references.
	/// </summary>
	class Counter
	{
	public:
		Int strongCount = 0;
		Int weakCount = 0;
	};

	template<typename T>
	class Ref;

	/// <summary>
	/// A shared pointer.
	/// </summary>
	/// <param name="other"></param>
	template<typename T>
	class Owner
	{
		// so Ref types can access Owner
		friend class Ref<T>;

		// so other Ref types can access Ref
		template<typename U>
		friend class Owner;

	private:
		/// <summary>
		/// A pointer to the data.
		/// </summary>
		T* mp_ptr;

		/// <summary>
		/// A pointer to the counter.
		/// </summary>
		Counter* mp_counter;

	public:
		/// <summary>
		/// Creates an empty Owner.
		/// </summary>
		Owner() : mp_ptr(nullptr), mp_counter(nullptr) {}

		/// <summary>
		/// Creates a new Owner from an existing data instance.
		/// </summary>
		/// <param name="ptr"></param>
		explicit Owner(T* const ptr)
			: mp_ptr(ptr), mp_counter(new Counter())
		{
			mp_counter->strongCount++;
		}

		/// <summary>
		/// Creates a new Owner from an existing data instance and counter.
		/// </summary>
		/// <param name="ptr">The data pointer.</param>
		/// <param name="counter">The counter pointer.</param>
		explicit Owner(T* const ptr, Counter* const counter)
			: mp_ptr(ptr), mp_counter(counter)
		{
			if (mp_counter) mp_counter->strongCount++;
		}

		/// <summary>
		/// Creates a new Owner with a new instance of data.
		/// </summary>
		/// <typeparam name="...Args">The types for the constructor of the data class.</typeparam>
		/// <param name="...args">The arguments for the constructor of the data class.</param>
		template<typename... Args>
		explicit Owner(Args&&... args)
			: mp_ptr(new T(std::forward<Args>(args)...))
			, mp_counter(new Counter())
		{
			mp_counter->strongCount++;
		}

		// implicit from null
		Owner(std::nullptr_t) : mp_ptr(nullptr), mp_counter(nullptr) {}

		// casting
		template<typename U>
		Owner(const Owner<U>& other)
			: mp_ptr(static_cast<T*>(other.mp_ptr))
			, mp_counter(other.mp_counter)
		{
			if (mp_counter) mp_counter->strongCount++;
		}

		Owner(Owner const& other)
			: mp_ptr(other.mp_ptr)
			, mp_counter(other.mp_counter)
		{
			if (mp_counter) mp_counter->strongCount++;
		}
		Owner& operator=(Owner const& other)
		{
			if (this != &other)
			{
				release();
				mp_ptr = other.mp_ptr;
				mp_counter = other.mp_counter;
				if (mp_counter) mp_counter->strongCount++;
			}
			return *this;
		}
		Owner(Owner&& other) noexcept
			: mp_ptr(other.mp_ptr)
			, mp_counter(other.mp_counter)
		{
			other.mp_ptr = nullptr;
			other.mp_counter = nullptr;
		}
		Owner& operator=(Owner&& other) noexcept {
			if (this != &other) {
				release();
				mp_ptr = other.mp_ptr;
				mp_counter = other.mp_counter;
				other.mp_ptr = nullptr;
				other.mp_counter = nullptr;
			}
			return *this;
		}

		~Owner()
		{
			release();
		}

		void release()
		{
			if (mp_ptr == nullptr) return;

			mp_counter->strongCount--;
			MINTY_ASSERT_FORMAT(mp_counter->strongCount >= 0, "Owner counter invalid in release ({}).", mp_counter->strongCount);

			// no more owners, so delete the data
			if (!mp_counter->strongCount)
			{
				delete mp_ptr;

				// if also no more weak references, also delete the counter
				if (!mp_counter->weakCount)
				{
					delete mp_counter;
				}
			}

			mp_ptr = nullptr;
			mp_counter = nullptr;
		}

		template<typename U>
		Bool operator==(Owner<U> const& other) const { return get() == other.get(); }
		template<typename U>
		Bool operator==(U* const other) const { return get() == other; }
		template<typename U>
		Bool operator==(Ref<U> const& other) const { return get() == other.get(); }
		Bool operator==(std::nullptr_t const other) const { return get() == nullptr; }
		template<typename U>
		Bool operator!=(Owner<U> const& other) const { return !(*this == other); }
		template<typename U>
		Bool operator!=(U* const other) const { return !(*this == other); }
		template<typename U>
		Bool operator!=(Ref<U> const& other) const { return !(*this == other); }
		Bool operator!=(std::nullptr_t const other) const { return get() != nullptr; }
		template<typename U>
		Bool operator<(Owner<U> const& other) const { return get() < other.get(); }
		template<typename U>
		Bool operator<(U* const other) const { return get() < other; }
		template<typename U>
		Bool operator<(Ref<U> const& other) const { return get() < other.get(); }
		Bool operator!() const { return !mp_counter || !static_cast<Bool>(mp_counter->strongCount); }

		T* get() const { return mp_ptr; }
		T& operator*() const { return *get(); }
		T* operator->() const { return get(); }

		Ref<T> create_ref() const { return Ref<T>(mp_ptr, mp_counter); }
		template<typename U>
		Ref<U> create_ref() const { return Ref<U>(static_cast<U*>(mp_ptr), mp_counter); }
	};

	/// <summary>
	/// A weak pointer. 
	/// </summary>
	template<typename T>
	class Ref
	{
		// so Owners can access Ref
		friend class Owner<T>;

		// so other Ref types can access Ref
		template<typename U>
		friend class Ref;

	private:
		T* mp_ptr;
		Counter* mp_counter;

	public:
		Ref() : mp_ptr(nullptr), mp_counter(nullptr) {}

		// create from data
		explicit Ref(T* const ptr, Counter* const counter)
			: mp_ptr(ptr), mp_counter(counter)
		{
			if (mp_counter) mp_counter->weakCount++;
		}

		// implicit from null
		Ref(std::nullptr_t) : mp_ptr(nullptr), mp_counter(nullptr) {}

		// casting
		template<typename U>
		Ref(const Ref<U>& other) 
			: mp_ptr(static_cast<T*>(const_cast<U*>(other.mp_ptr)))
			, mp_counter(other.mp_counter)
		{
			if (mp_counter) mp_counter->weakCount++;
		}

		// implicit from Owner
		Ref(const Owner<T>& owner)
			: mp_ptr(owner.mp_ptr), mp_counter(owner.mp_counter)
		{
			if (mp_counter) mp_counter->weakCount++;
		}
		Ref& operator=(const Owner<T>& other)
		{
			if (mp_counter) mp_counter->weakCount--;
			MINTY_ASSERT_FORMAT(!mp_counter || mp_counter->weakCount >= 0, "Ref counter invalid in copy ({}).", mp_counter->weakCount);
			mp_ptr = other.mp_ptr;
			mp_counter = other.mp_counter;
			if (mp_counter) mp_counter->weakCount++;

			return *this;
		}

	public:
		Ref(Ref const& other)
			: mp_ptr(other.mp_ptr)
			, mp_counter(other.mp_counter)
		{
			if (mp_counter) mp_counter->weakCount++;
		}
		Ref& operator=(Ref const& other)
		{
			if (this != &other)
			{
				if (mp_counter) mp_counter->weakCount--;
				MINTY_ASSERT_FORMAT(!mp_counter || mp_counter->weakCount >= 0, "Ref counter invalid in copy ({}).", mp_counter->weakCount);
				mp_ptr = other.mp_ptr;
				mp_counter = other.mp_counter;
				if (mp_counter) mp_counter->weakCount++;
			}
			return *this;
		}
		Ref(Ref&& other) noexcept
			: mp_ptr(other.mp_ptr)
			, mp_counter(other.mp_counter)
		{
			other.mp_ptr = nullptr;
			other.mp_counter = nullptr;
		}
		Ref& operator=(Ref&& other) noexcept {
			if (this != &other) {
				if (mp_counter) mp_counter->weakCount--;
				MINTY_ASSERT_FORMAT(!mp_counter || mp_counter->weakCount >= 0, "Ref counter invalid in move ({}).", mp_counter->weakCount);
				mp_ptr = other.mp_ptr;
				mp_counter = other.mp_counter;
				other.mp_ptr = nullptr;
				other.mp_counter = nullptr;
			}
			return *this;
		}

		~Ref()
		{
			release();
		}

		void release()
		{
			if (mp_counter == nullptr) return;

			mp_counter->weakCount--;
			MINTY_ASSERT_FORMAT(mp_counter->weakCount >= 0, "Ref counter invalid in release ({}).", mp_counter->weakCount);

			// if no more strong and no more weak, delete counter
			if (!mp_counter->strongCount && !mp_counter->weakCount)
			{
				delete mp_counter;
			}

			mp_ptr = nullptr;
			mp_counter = nullptr;
		}

		template<typename U>
		Bool operator==(Owner<U> const& other) const { return get() == other.get(); }
		template<typename U>
		Bool operator==(U* const other) const { return get() == other; }
		template<typename U>
		Bool operator==(Ref<U> const& other) const { return get() == other.get(); }
		Bool operator==(std::nullptr_t const other) const { return get() == nullptr; }
		template<typename U>
		Bool operator!=(Owner<U> const& other) const { return !(*this == other); }
		template<typename U>
		Bool operator!=(U* const other) const { return !(*this == other); }
		template<typename U>
		Bool operator!=(Ref<U> const& other) const { return !(*this == other); }
		Bool operator!=(std::nullptr_t const other) const { return get() != nullptr; }
		template<typename U>
		Bool operator<(Owner<U> const& other) const { return get() < other.get(); }
		template<typename U>
		Bool operator<(U* const other) const { return get() < other; }
		template<typename U>
		Bool operator<(Ref<U> const& other) const { return get() < other.get(); }
		Bool operator!() const { return !mp_counter || !mp_counter->strongCount; }

		T* get() const
		{
			if (mp_counter)
				return mp_counter->strongCount ? mp_ptr : nullptr;

			return nullptr;
		}
		T& operator*() const { return *get(); }
		T* operator->() const { return get(); }

		template<typename U>
		Ref<U> static_cast_to() const { return Ref<U>(static_cast<U*>(mp_ptr), mp_counter); }
	};

	/// <summary>
	/// A Source is an Owner, but this object is where the pointer originates from.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class Source
	{
	private:
		Counter* mp_counter;

	public:
		Source()
			: mp_counter(new Counter({ 1, 0 }))
		{}

		virtual ~Source()
		{
			release();
		}

		void release()
		{
			if (mp_counter == nullptr) return;

			mp_counter->strongCount--;
			MINTY_ASSERT_FORMAT(mp_counter->strongCount >= 0, "Ref counter invalid in release ({}).", mp_counter->strongCount);

			// if no more strong and no more weak, delete counter
			if (!mp_counter->strongCount && !mp_counter->weakCount)
			{
				delete mp_counter;
			}

			mp_counter = nullptr;
		}

		Ref<T> create_ref()
		{
			return Ref<T>(static_cast<T*>(this), mp_counter);
		}
	};
}

namespace std {
	template <typename T>
	struct hash<Minty::Owner<T>> {
		Minty::Size operator()(Minty::Owner<T> const& sp) const noexcept {
			return std::hash<T*>()(sp.get());
		}
	};

	template <typename T>
	struct hash<Minty::Ref<T>> {
		Minty::Size operator()(Minty::Ref<T> const& sp) const noexcept {
			return std::hash<T*>()(sp.get());
		}
	};
}