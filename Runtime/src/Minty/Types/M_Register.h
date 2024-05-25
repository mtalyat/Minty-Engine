#pragma once

#include "Minty/Core/M_Base.h"
#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace Minty
{
	/// <summary>
	/// Holds a collection of data that is assigned an ID, and has an optional name (name).
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <class T>
	class Register
	{
	private:
		// the most amount of elements allowed
		ID _capacity;

		// the data within the register
		std::unordered_map<ID, T> _data;

		// names
		std::unordered_map<ID, String> _names;

		// name lookup
		std::unordered_map<String, ID> _lookup;

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
		/// Creates a new named object within the Register with an name.
		/// </summary>
		/// <param name="name">The name of the object.</param>
		/// <param name="obj">The object.</param>
		/// <returns>The ID of the object in the Register, or ERROR_ID if the Register is full.</returns>
		ID emplace(String const& name, T const& obj);

		/// <summary>
		/// Sets an name for an ID in this Register.
		/// </summary>
		/// <param name="name">The name name.</param>
		/// <param name="id">The ID of the object.</param>
		void emplace_name(String const& name, ID const id);

		/// <summary>
		/// Gets the next ID that will be used.
		/// </summary>
		/// <returns></returns>
		ID get_next() const;

		/// <summary>
		/// Removes an name for an ID in this Register.
		/// </summary>
		/// <param name="name">The name name.</param>
		void erase_name(String const& name);

		/// <summary>
		/// Erases the object with the given ID.
		/// </summary>
		/// <param name="id"></param>
		void erase(ID const id);

		/// <summary>
		/// Erases the object with the given name.
		/// </summary>
		/// <param name="name"></param>
		void erase(String const& name);

		/// <summary>
		/// Checks if this Register has an object with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		bool contains(ID const id) const;

		/// <summary>
		/// Checks if this Register has an object with the given name.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		bool contains(String const& name) const;

		/// <summary>
		/// Gets the ID for the given name in this Register.
		/// </summary>
		/// <param name="name">The name of the object.</param>
		/// <returns>The ID of the object with the name.</returns>
		ID get_id(String const& name) const;

		/// <summary>
		/// Gets the name for the given ID in this Register.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		String get_name(ID const id) const;

		/// <summary>
		/// Finds the ID that is associated with the given name in this Register.
		/// </summary>
		/// <param name="name">The name of the element.</param>
		/// <returns>The ID of the element, or ERROR_ID if the name does not exist.</returns>
		ID find(String const& name) const;

		/// <summary>
		/// Gets the object with the given ID.
		/// </summary>
		/// <param name="id">The ID of the object in this Register.</param>
		/// <returns>A pointer to the object, or nullptr if the ID was invalid.</returns>
		T& at(ID const id);

		/// <summary>
		/// Gets the object with the given ID.
		/// </summary>
		/// <param name="id">The ID of the object in this Register.</param>
		/// <returns>A pointer to the object, or nullptr if the ID was invalid.</returns>
		T const& at(ID const id) const;

		/// <summary>
		/// Gets the object with the given name.
		/// </summary>
		/// <param name="name">The name of the object in this Register.</param>
		/// <returns>A pointer to the object.</returns>
		T& at(String const& name);

		/// <summary>
		/// Gets the object with the given name.
		/// </summary>
		/// <param name="name">The name of the object in this Register.</param>
		/// <returns>A pointer to the object.</returns>
		T const& at(String const& name) const;

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

		/// <summary>
		/// Gets the vector containing all of this Register data.
		/// </summary>
		/// <returns></returns>
		auto& data();

		/// <summary>
		/// Gets the vector containing all of this Register data.
		/// </summary>
		/// <returns></returns>
		auto const& data() const;

		/// <summary>
		/// Resizes this Register.
		/// </summary>
		/// <param name="size"></param>
		void resize(ID const size);

		/// <summary>
		/// Reserves this Register.
		/// </summary>
		/// <param name="size"></param>
		void reserve(ID const size);

		/// <summary>
		/// Sets the new capacity for this Register. All values are cleared.
		/// </summary>
		/// <param name="size">The new capacity.</param>
		void limit(ID const size = MAX_ID);

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

	template <class T>
	Register<T>::Register()
		: _data(), _capacity(MAX_ID), _next(0)
	{
	}

	template <class T>
	Register<T>::Register(ID const capacity)
		: _data(), _capacity(capacity), _next(0)
	{
	}

	template <class T>
	ID Register<T>::emplace(T const& obj)
	{
		return emplace("", obj);
	}

	template <class T>
	ID Register<T>::emplace(String const& name, T const& obj)
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

		// add name
		if (!name.empty())
		{
			_names[id] = name;
			_lookup[name] = id;
		}

		// return the id
		return id;
	}

	template <class T>
	void Register<T>::emplace_name(String const& name, ID const id)
	{
		_lookup[name] = id;
		_names[id] = name;
	}

	template<class T>
	inline ID Register<T>::get_next() const
	{
		return _next;
	}

	template <class T>
	void Register<T>::erase_name(String const& name)
	{
		ID id = _lookup.at(name);
		_lookup.erase(name);
		_names.erase(id);
	}

	template <class T>
	void Register<T>::erase(ID const id)
	{
		if (_names.contains(id))
		{
			String name = _names.at(id);
			_lookup.erase(name);
			_names.erase(id);
		}

		_data.erase(id);
	}

	template <class T>
	void Register<T>::erase(String const& name)
	{
		ID id = _lookup.at(name);
		_lookup.erase(name);
		_names.erase(id);
		_data.erase(id);
	}

	template <class T>
	bool Register<T>::contains(String const& name) const
	{
		return _lookup.contains(name);
	}

	template <class T>
	bool Register<T>::contains(ID const id) const
	{
		return _data.contains(id);
	}

	template <class T>
	ID Register<T>::get_id(String const& name) const
	{
		auto found = _lookup.find(name);

		if (found == _lookup.end())
		{
			return ERROR_ID;
		}
		else
		{
			return found->second;
		}
	}

	template <class T>
	String Register<T>::get_name(ID const id) const
	{
		auto found = _names.find(id);

		if (found == _names.end())
		{
			return "";
		}
		else
		{
			return found->second;
		}
	}

	template<class T>
	ID Register<T>::find(String const& name) const
	{
		auto found = _lookup.find(name);
		if (found != _lookup.end())
		{
			return found->second;
		}

		return ERROR_ID;
	}

	template <class T>
	T const& Register<T>::at(ID const id) const
	{
		return _data.at(id);
	}

	template <class T>
	T& Register<T>::at(ID const id)
	{
		return _data.at(id);
	}

	template <class T>
	T const& Register<T>::at(String const& name) const
	{
		// use lookup to find id, then use that to get object
		return at(_lookup.at(name));
	}

	template <class T>
	T& Register<T>::at(String const& name)
	{
		// use lookup to find id, then use that to get object
		return at(_lookup.at(name));
	}

	template <class T>
	ID Register<T>::size() const
	{
		return static_cast<ID>(_data.size());
	}

	template <class T>
	bool Register<T>::full() const
	{
		return _data.size() >= _capacity;
	}

	template <class T>
	void Register<T>::clear()
	{
		_next = 0;
		_data.clear();
		_names.clear();
		_lookup.clear();
	}

	template <class T>
	auto& Register<T>::data()
	{
		return _data;
	}

	template <class T>
	auto const& Register<T>::data() const
	{
		return _data;
	}

	template <class T>
	void Register<T>::resize(ID const size)
	{
		_data.resize(size);
	}

	template <class T>
	void Register<T>::reserve(ID const size)
	{
		_data.reserve(size);
	}

	template <class T>
	void Register<T>::limit(ID const size)
	{
		_capacity = size;
	}

	template <class T>
	auto Register<T>::begin()
	{
		return _data.begin();
	}

	template <class T>
	auto Register<T>::end()
	{
		return _data.end();
	}

	template <class T>
	auto Register<T>::cbegin() const
	{
		return _data.cbegin();
	}

	template <class T>
	auto Register<T>::cend() const
	{
		return _data.cend();
	}

	template <class T>
	auto Register<T>::begin() const
	{
		return _data.begin();
	}

	template <class T>
	auto Register<T>::end() const
	{
		return _data.end();
	}

	template <class T>
	auto& Register<T>::front()
	{
		return _data.front();
	}

	template <class T>
	auto& Register<T>::back()
	{
		return _data.back();
	}

	template <class T>
	auto const& Register<T>::front() const
	{
		return _data.front();
	}

	template <class T>
	auto const& Register<T>::back() const
	{
		return _data.back();
	}
}