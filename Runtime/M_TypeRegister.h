#pragma once

#include "M_Types.h"
#include <vector>
#include <unordered_map>

namespace minty
{
	/// <summary>
	/// Holds types and only allows the same or child types to be added.
	/// Only one of each child type can be held.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<class T>
	class TypeRegister
	{
	private:
		std::vector<T*> _values;
		std::unordered_map<TypeID, size_t> _lookup;

	public:
		TypeRegister();

		/// <summary>
		/// Checks if this TypeRegister contains the type U.
		/// </summary>
		/// <typeparam name="U"></typeparam>
		/// <returns></returns>
		template<class U>
		bool contains() const;

		/// <summary>
		/// Checks if this TypeRegister contains the type U and the given value.
		/// </summary>
		/// <typeparam name="U"></typeparam>
		/// <param name="obj"></param>
		/// <returns></returns>
		template<class U>
		bool contains(U* const obj) const;

		/// <summary>
		/// Sets the value for the type U.
		/// </summary>
		/// <typeparam name="U"></typeparam>
		/// <param name="obj"></param>
		template<class U>
		void emplace(U* const obj);

		/// <summary>
		/// Erases the value associated with the type U.
		/// </summary>
		/// <typeparam name="U"></typeparam>
		/// <returns></returns>
		template<class U>
		bool erase();

		/// <summary>
		/// Retrieves the value with the type U.
		/// </summary>
		/// <typeparam name="U"></typeparam>
		/// <returns></returns>
		template<class U>
		U* at() const;

		/// <summary>
		/// Retrieves the value at this index.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		T* at(size_t const index) const;

		/// <summary>
		/// Gets the value with the type U, or nullptr if none exists.
		/// </summary>
		/// <typeparam name="U"></typeparam>
		/// <returns></returns>
		template<class U>
		U* get() const;

		/// <summary>
		/// Gets the number of values in this TypeRegister.
		/// </summary>
		/// <returns></returns>
		size_t size() const;

		/// <summary>
		/// Clears the TypeRegister.
		/// </summary>
		void clear();

		std::vector<T*>::iterator begin();
		std::vector<T*>::iterator end();
		std::vector<T*>::iterator cbegin() const;
		std::vector<T*>::iterator cend() const;
		std::vector<T*>::iterator begin() const;
		std::vector<T*>::iterator end() const;
	};

	template<class T>
	TypeRegister<T>::TypeRegister()
		: _values()
		, _lookup()
	{}
	
	template<class T>
	template<class U>
	bool TypeRegister<T>::contains() const
	{
		MINTY_ASSERT_MESSAGE((is_type<U, T>()), std::format("Type U ({1}) must be derived from type T ({0}).", typeid(T).name(), typeid(U).name()));

		return _lookup.contains(typeid(U));
	}

	template<class T>
	template<class U>
	bool TypeRegister<T>::contains(U* const obj) const
	{
		MINTY_ASSERT_MESSAGE((is_type<U, T>()), std::format("Type U ({1}) must be derived from type T ({0}).", typeid(T).name(), typeid(U).name()));

		auto found = _lookup.find(typeid(U));

		if (found != _lookup.end())
		{
			return _values.at(found->second) == obj;
		}

		return false;
	}

	template<class T>
	template<class U>
	void TypeRegister<T>::emplace(U* const obj)
	{
		MINTY_ASSERT_MESSAGE((is_type<U, T>()), std::format("Type U ({1}) must be derived from type T ({0}).", typeid(T).name(), typeid(U).name()));

		// if the type exists, replace it, otherwise add it
		TypeID typeId = typeid(U);
		auto found = _lookup.find(typeId);

		if (found != _lookup.end())
		{
			_values[found->second] = obj;
		}
		else
		{
			_lookup.emplace(typeId, _values.size());
			_values.push_back(obj);
		}
	}

	template<class T>
	template<class U>
	bool TypeRegister<T>::erase()
	{
		MINTY_ASSERT_MESSAGE((is_type<U, T>()), std::format("Type U ({1}) must be derived from type T ({0}).", typeid(T).name(), typeid(U).name()));

		return false;
	}

	template<class T>
	template<class U>
	U* TypeRegister<T>::at() const
	{
		MINTY_ASSERT_MESSAGE(contains(), std::format("Type U ({1}) is not part of this TypeRegister.", typeid(T).name(), typeid(U).name()));

		return static_cast<U*>(_values.at(_lookup.at(typeid(U))));
	}

	template<class T>
	template<class U>
	U* TypeRegister<T>::get() const
	{
		auto found = _lookup.find(typeid(U));

		if (found != _lookup.end())
		{
			return static_cast<U*>(_values.at(found->second));
		}

		return nullptr;
	}

	template<class T>
	T* TypeRegister<T>::at(size_t const index) const
	{
		return _values.at(index);
	}

	template<class T>
	size_t TypeRegister<T>::size() const
	{
		return _values.size();
	}

	template<class T>
	void TypeRegister<T>::clear()
	{
		_values.clear();
		_lookup.clear();
	}

	template<class T>
	std::vector<T*>::iterator TypeRegister<T>::begin()
	{
		return _values.begin();
	}

	template<class T>
	std::vector<T*>::iterator TypeRegister<T>::end()
	{
		return _values.end();
	}

	template<class T>
	std::vector<T*>::iterator TypeRegister<T>::cbegin() const
	{
		return _values.cbegin();
	}

	template<class T>
	std::vector<T*>::iterator TypeRegister<T>::cend() const
	{
		return _values.end();
	}

	template<class T>
	std::vector<T*>::iterator TypeRegister<T>::begin() const
	{
		return _values.begin();
	}
	template<class T>
	std::vector<T*>::iterator TypeRegister<T>::end() const
	{
		return _values.end();
	}
}