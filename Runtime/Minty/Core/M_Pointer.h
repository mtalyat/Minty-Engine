#pragma once

namespace Minty
{
	/// <summary>
	/// Holds smart pointer count data for weak and strong references.
	/// </summary>
	class Counter
	{
	public:
		int strongCount = 0;
		int weakCount = 0;
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
		friend class Ref<T>;

	private:
		T* _ptr;
		Counter* _counter;

	public:
		Owner() : _ptr(nullptr), _counter(nullptr) {}

		// create from data
		explicit Owner(T* const ptr, Counter* const counter)
			: _ptr(ptr), _counter(counter)
		{
			if (_counter) _counter->strongCount++;
		}

		// create new
		template<typename... Args>
		explicit Owner(Args&&... args)
			: _ptr(new T(std::forward<Args>(args)...))
			, _counter(new Counter())
		{
			_counter->strongCount++;
		}

		// implicit from null
		Owner(std::nullptr_t) : _ptr(nullptr), _counter(nullptr) {}

		// implicit from derived
		template <typename U, typename = std::enable_if_t<std::is_base_of<U, T>::value>>
		operator Owner<U>() const {
			return Owner<U>(static_cast<U*>(_ptr), _counter);
		}

		Owner(Owner const& other)
			: _ptr(other._ptr)
			, _counter(other._counter)
		{
			if (_counter) _counter->strongCount++;
		}
		Owner& operator=(Owner const& other)
		{
			if (this != &other)
			{
				if (_counter) _counter->strongCount--;
				MINTY_ASSERT_FORMAT(!_counter || _counter->strongCount >= 0, "Owner counter invalid in copy ({}).", _counter->strongCount);
				_ptr = other._ptr;
				_counter = other._counter;
				if (_counter) _counter->strongCount++;
			}
			return *this;
		}
		Owner(Owner&& other) noexcept
			: _ptr(other._ptr)
			, _counter(other._counter)
		{
			other._ptr = nullptr;
			other._counter = nullptr;
		}
		Owner& operator=(Owner&& other) noexcept {
			if (this != &other) {
				if (_counter) _counter->strongCount--;
				MINTY_ASSERT_FORMAT(!_counter || _counter->strongCount >= 0, "Owner counter invalid in move ({}).", _counter->strongCount);
				_ptr = other._ptr;
				_counter = other._counter;
				other._ptr = nullptr;
				other._counter = nullptr;
			}
			return *this;
		}

		~Owner()
		{
			release();
		}

		void release()
		{
			if (_ptr == nullptr) return;

			_counter->strongCount--;
			MINTY_ASSERT_FORMAT(_counter->strongCount >= 0, "Owner counter invalid in release ({}).", _counter->strongCount);

			// no more owners, so delete the data
			if (!_counter->strongCount)
			{
				delete _ptr;

				// if also no more weak references, also delete the counter
				if (!_counter->weakCount)
				{
					delete _counter;
				}
			}

			_ptr = nullptr;
			_counter = nullptr;
		}

		template<typename U>
		bool operator==(Owner<U> const& other) const { return get() == other.get(); }
		template<typename U>
		bool operator==(U* const other) const { return get() == other; }
		template<typename U>
		bool operator==(Ref<U> const& other) const { return get() == other.get(); }
		bool operator==(std::nullptr_t const other) const { return get() == nullptr; }
		template<typename U>
		bool operator!=(Owner<U> const& other) const { return !(*this == other); }
		template<typename U>
		bool operator!=(U* const other) const { return !(*this == other); }
		template<typename U>
		bool operator!=(Ref<U> const& other) const { return !(*this == other); }
		bool operator!=(std::nullptr_t const other) const { return get() != nullptr; }
		template<typename U>
		bool operator<(Owner<U> const& other) const { return get() < other.get(); }
		template<typename U>
		bool operator<(U* const other) const { return get() < other; }
		template<typename U>
		bool operator<(Ref<U> const& other) const { return get() < other.get(); }
		bool operator!() const { return !_counter || !static_cast<bool>(_counter->strongCount); }

		T* get() const { return _ptr; }
		T& operator*() const { return *get(); }
		T* operator->() const { return get(); }

		Ref<T> create_ref() const { return Ref<T>(_ptr, _counter); }
		template<typename U>
		Ref<U> create_ref() const { return Ref<U>(static_cast<U*>(_ptr), _counter); }
	};

	/// <summary>
	/// A weak pointer. 
	/// </summary>
	template<typename T>
	class Ref
	{
		friend class Owner<T>;

	private:
		T* _ptr;
		Counter* _counter;

	public:
		Ref() : _ptr(nullptr), _counter(nullptr) {}

		// create from data
		explicit Ref(T* const ptr, Counter* const counter)
			: _ptr(ptr), _counter(counter)
		{
			if (_counter) _counter->weakCount++;
		}

		// implicit from null
		Ref(std::nullptr_t) : _ptr(nullptr), _counter(nullptr) {}

		// implicit from derived type
		template <typename U, typename = std::enable_if_t<std::is_base_of<U, T>::value>>
		operator Ref<U>() const {
			return Ref<U>(static_cast<U*>(_ptr), _counter);
		}

		// implicit from Owner
		Ref(Owner<T> const owner)
			: _ptr(owner._ptr), _counter(owner._counter)
		{
			if (_counter) _counter->weakCount++;
		}

	public:
		Ref(Ref const& other)
			: _ptr(other._ptr)
			, _counter(other._counter)
		{
			if(_counter) _counter->weakCount++;
		}
		Ref& operator=(Ref const& other)
		{
			if (this != &other)
			{
				if (_counter) _counter->weakCount--;
				MINTY_ASSERT_FORMAT(!_counter || _counter->weakCount >= 0, "Ref counter invalid in copy ({}).", _counter->weakCount);
				_ptr = other._ptr;
				_counter = other._counter;
				if (_counter) _counter->weakCount++;
			}
			return *this;
		}
		Ref(Ref&& other) noexcept
			: _ptr(other._ptr)
			, _counter(other._counter)
		{
			other._ptr = nullptr;
			other._counter = nullptr;
		}
		Ref& operator=(Ref&& other) noexcept {
			if (this != &other) {
				if (_counter) _counter->weakCount--;
				MINTY_ASSERT_FORMAT(!_counter || _counter->weakCount >= 0, "Ref counter invalid in move ({}).", _counter->weakCount);
				_ptr = other._ptr;
				_counter = other._counter;
				other._ptr = nullptr;
				other._counter = nullptr;
			}
			return *this;
		}

		~Ref()
		{
			release();
		}

		void release()
		{
			if (_counter == nullptr) return;

			_counter->weakCount--;
			MINTY_ASSERT_FORMAT(_counter->weakCount >= 0, "Ref counter invalid in release ({}).", _counter->weakCount);

			// if no more strong and no more weak, delete counter
			if (!_counter->strongCount && !_counter->weakCount)
			{
				delete _counter;
			}

			_ptr = nullptr;
			_counter = nullptr;
		}

		template<typename U>
		bool operator==(Owner<U> const& other) const { return get() == other.get(); }
		template<typename U>
		bool operator==(U* const other) const { return get() == other; }
		template<typename U>
		bool operator==(Ref<U> const& other) const { return get() == other.get(); }
		bool operator==(std::nullptr_t const other) const { return get() == nullptr; }
		template<typename U>
		bool operator!=(Owner<U> const& other) const { return !(*this == other); }
		template<typename U>
		bool operator!=(U* const other) const { return !(*this == other); }
		template<typename U>
		bool operator!=(Ref<U> const& other) const { return !(*this == other); }
		bool operator!=(std::nullptr_t const other) const { return get() != nullptr; }
		template<typename U>
		bool operator<(Owner<U> const& other) const { return get() < other.get(); }
		template<typename U>
		bool operator<(U* const other) const { return get() < other; }
		template<typename U>
		bool operator<(Ref<U> const& other) const { return get() < other.get(); }
		bool operator!() const { return !_counter || !_counter->strongCount; }

		T* get() const
		{
			if(_counter)
				return _counter->strongCount ? _ptr : nullptr;

			return nullptr;
		}
		T& operator*() const { return *get(); }
		T* operator->() const { return get(); }

		template<typename U>
		Ref<U> static_cast_to() const { return Ref<U>(static_cast<U*>(_ptr), _counter); }
	};
}

namespace std {
	template <typename T>
	struct hash<Minty::Owner<T>> {
		size_t operator()(Minty::Owner<T> const& sp) const noexcept {
			return std::hash<T*>()(sp.get());
		}
	};

	template <typename T>
	struct hash<Minty::Ref<T>> {
		size_t operator()(Minty::Ref<T> const& sp) const noexcept {
			return std::hash<T*>()(sp.get());
		}
	};
}