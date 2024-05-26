#pragma once

namespace Minty
{
	/*template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr Scope<T> create_scope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> create_ref(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr T* ref_to_pointer(Ref<T> const ref)
	{
		return ref.get();
	}*/

	// https://stackoverflow.com/questions/5671241/how-does-weak-ptr-work

	/// <summary>
	/// Holds smart pointer count data for weak and strong references.
	/// </summary>
	class Counter
	{
	public:
		int strongCount;
		int weakCount;
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
			if (_counter) _counter->strongCount++;
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
				delete _ptr;
				delete _counter;
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

			// no more owners, so delete the data
			if (!_counter->strongCount)
			{
				delete _ptr;
			}

			// if no more weak references, also delete the counter
			if (!_counter->weakCount)
			{
				delete _counter;
			}

			_ptr = nullptr;
			_counter = nullptr;
		}

		bool operator==(Owner<T> const& other) const { return _ptr == other._ptr; }
		bool operator==(T* const other) const { return _ptr == other; }
		bool operator==(Ref<T> const& other) const { return _ptr == other._ptr; }
		bool operator!=(Owner<T> const& other) const { return _ptr != other._ptr; }
		bool operator!=(T* const other) const { return _ptr != other; }
		bool operator!=(Ref<T> const& other) const { return _ptr != other._ptr; }
		bool operator<(Owner<T> const& other) const { return _ptr < other._ptr; }
		bool operator<(T* const other) const { return _ptr < other; }
		bool operator<(Ref<T> const& other) const { return _ptr < other._ptr; }
		bool operator!() const { return !static_cast<bool>(_counter->strongCount); }
		operator bool() const { return static_cast<bool>(_counter->strongCount); }

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
				delete _ptr;
				delete _counter;
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

			// if no more strong and no more weak, delete counter
			if (!_counter->strongCount && !_counter->weakCount)
			{
				delete _counter;
			}

			_ptr = nullptr;
			_counter = nullptr;
		}

		bool operator==(Owner<T> const& other) const { return _ptr == other._ptr; }
		bool operator==(T* const other) const { return _ptr == other; }
		bool operator==(Ref<T> const& other) const { return _ptr == other._ptr; }
		bool operator!=(Owner<T> const& other) const { return _ptr != other._ptr; }
		bool operator!=(T* const other) const { return _ptr != other; }
		bool operator!=(Ref<T> const& other) const { return _ptr != other._ptr; }
		bool operator<(Owner<T> const& other) const { return _ptr < other._ptr; }
		bool operator<(T* const other) const { return _ptr < other; }
		bool operator<(Ref<T> const& other) const { return _ptr < other._ptr; }
		bool operator!() const { return !_counter || !_counter->strongCount; }
		operator bool() const { return _counter && static_cast<bool>(_counter->strongCount); }

		T* get() const
		{
			// if no strong count, ptr has been deleted, so return null
			return _counter->strongCount ? _ptr : nullptr;
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