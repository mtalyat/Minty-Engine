#pragma once
#include "M_Object.h"

#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace minty
{
	template<class T>
	class Register :
		public Object
	{
	private:
		// the most amount of elements allowed
		ID _capacity;
		
		// the data within the register
		std::unordered_map<ID, T> _data;

		// name lookup
		std::unordered_map<std::string, ID> _lookup;

		// next ID to be used
		ID _next;
	public:
		/// <summary>
		/// Create an empty Register.
		/// </summary>
		Register();

		/// <summary>
		/// Create a Register with a capacity.
		/// </summary>
		/// <param name="casize_tpacity">The maximum size limit.</param>
		Register(ID const capacity);

		/// <summary>
		/// Create a new object within the Register.
		/// </summary>
		/// <param name="obj">The object values.</param>
		/// <returns>The ID of the new object in this Register, or ERROR_ID if this Register is full.</returns>
		ID emplace(T const& obj);

		/// <summary>
		/// Creates a new named object within the Register with an alias.
		/// </summary>
		/// <param name="name">The name of the object.</param>
		/// <param name="obj">The object.</param>
		/// <returns>The ID of the object in the Register, or ERROR_ID if the Register is full.</returns>
		ID emplace(std::string const& name, T const& obj);

		/// <summary>
		/// Adds an alias for an ID in this Register.
		/// </summary>
		/// <param name="name">The alias name.</param>
		/// <param name="id">The ID of the object.</param>
		void emplace_alias(std::string const& name, ID const id);

		void erase(ID const id);

		void erase(std::string const& name);

		/// <summary>
		/// Removes an alias for an ID in this Register.
		/// </summary>
		/// <param name="name">The alias name.</param>
		void erase_alias(std::string const& name);

		bool contains(std::string const& name) const;

		bool contains(ID const id) const;

		/// <summary>
		/// Gets the ID for the given name in this Register.
		/// </summary>
		/// <param name="name">The name of the object.</param>
		/// <returns>The ID of the object with the name.</returns>
		ID get_id(std::string const name) const;

		/// <summary>
		/// Gets the object with the given ID.
		/// </summary>
		/// <param name="id">The ID of the object in this Register.</param>
		/// <returns>A pointer to the object, or nullptr if the ID was invalid.</returns>
		T const& at(ID const id) const;

		T& at(ID const id);

		/// <summary>
		/// Gets the object with the given name.
		/// </summary>
		/// <param name="name">The name of the object in this Register.</param>
		/// <returns>A pointer to the object.</returns>
		T const& at(std::string const& name) const;

		T& at(std::string const& name);

		/// <summary>
		/// Gets the number of objects within this Register.
		/// </summary>
		/// <returns>The object count.</returns>
		ID size() const;

		/// <summary>
		/// Checks if the Register is at full capacity.
		/// </summary>
		/// <returns>True if the Register has the maximum amount of objects.</returns>
		bool full() const;

		/// <summary>
		/// Clears all data from this Register.
		/// </summary>
		void clear();

		auto& data();

		auto const& data() const;

		void resize(ID const size);

		void reserve(ID const size);

		/// <summary>
		/// Sets the new capacity for this Register. All values are cleared.
		/// </summary>
		/// <param name="size">The new capacity.</param>
		void limit(ID const size);

		auto begin();

		auto end();

		auto cbegin() const;

		auto cend() const;

		auto begin() const;

		auto end() const;

		auto& front();

		auto& back();

		auto const& front() const;

		auto const& back() const;
	};

	template<class T>
	Register<T>::Register()
		: _data()
		, _capacity(MAX_ID)
		, _next(0)
	{}

	template<class T>
	Register<T>::Register(ID const capacity)
		: _data()
		, _capacity(capacity)
		, _next(0)
	{}

	template<class T>
	ID Register<T>::emplace(T const& obj)
	{
		// check if over capacity
		if (_data.size() >= _capacity)
		{
			return ERROR_ID;
		}

		// get id
		ID id = static_cast<ID>(_next++);

		// add
		_data.emplace(id, obj);

		// return the id
		return id;
	}

	template<class T>
	ID Register<T>::emplace(std::string const& name, T const& obj)
	{
		// set object
		ID id = emplace(obj);

		// set lookup name
		emplace_alias(name, id);

		// return id
		return id;
	}

	template<class T>
	void Register<T>::emplace_alias(std::string const& name, ID const id)
	{
		_lookup[name] = id;
	}

	template<class T>
	void Register<T>::erase(ID const id)
	{
		_data.erase(id);
	}

	template<class T>
	void Register<T>::erase(std::string const& name)
	{
		ID id = _lookup.at(name);

		_data.erase(id);
	}

	template<class T>
	void Register<T>::erase_alias(std::string const& name)
	{
		_lookup.erase(name);
	}

	template<class T>
	bool Register<T>::contains(std::string const& name) const
	{
		return _lookup.contains(name);
	}

	template<class T>
	bool Register<T>::contains(ID const id) const
	{
		return _data.contains(id);
	}

	template<class T>
	ID Register<T>::get_id(std::string const name) const
	{
		return _lookup.at(name);
	}

	template<class T>
	T const& Register<T>::at(ID const id) const
	{
		return _data.at(id);
	}

	template<class T>
	T& Register<T>::at(ID const id)
	{
		return _data.at(id);
	}

	template<class T>
	T const& Register<T>::at(std::string const& name) const
	{
		// use lookup to find id, then use that to get object
		return at(_lookup.at(name));
	}

	template<class T>
	T& Register<T>::at(std::string const& name)
	{
		// use lookup to find id, then use that to get object
		return at(_lookup.at(name));
	}

	template<class T>
	ID Register<T>::size() const
	{
		return static_cast<ID>(_data.size());
	}

	template<class T>
	bool Register<T>::full() const
	{
		return _data.size() >= _capacity;
	}

	template<class T>
	void Register<T>::clear()
	{
		_data.clear();
		_lookup.clear();
	}

	template<class T>
	auto& Register<T>::data()
	{
		return _data;
	}

	template<class T>
	auto const& Register<T>::data() const
	{
		return _data;
	}

	template<class T>
	void Register<T>::resize(ID const size)
	{
		_data.resize(size);
	}

	template<class T>
	void Register<T>::reserve(ID const size)
	{
		_data.reserve(size);
	}

	template<class T>
	void Register<T>::limit(ID const size)
	{
		clear();
		_capacity = size;
	}

	template<class T>
	auto Register<T>::begin()
	{
		return _data.begin();
	}

	template<class T>
	auto Register<T>::end()
	{
		return _data.end();
	}

	template<class T>
	auto Register<T>::cbegin() const
	{
		return _data.cbegin();
	}

	template<class T>
	auto Register<T>::cend() const
	{
		return _data.cend();
	}

	template<class T>
	auto Register<T>::begin() const
	{
		return _data.begin();
	}

	template<class T>
	auto Register<T>::end() const
	{
		return _data.end();
	}

	template<class T>
	auto& Register<T>::front()
	{
		return _data.front();
	}

	template<class T>
	auto& Register<T>::back()
	{
		return _data.back();
	}

	template<class T>
	auto const& Register<T>::front() const
	{
		return _data.front();
	}

	template<class T>
	auto const& Register<T>::back() const
	{
		return _data.back();
	}
}