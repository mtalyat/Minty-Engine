#pragma once

#include "M_Object.h"
#include <vector>
#include <functional>

namespace minty
{
	/// <summary>
	/// An event is something that can have multiple functions be called upon the Event being invoked.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T = void>
	class Event
		: public Object
	{
	public:
		// the function type
		typedef std::function<void(T)> func;

	private:
		// the list of functions to be called
		std::vector<func> _functions;

	public:

		/// <summary>
		/// Creates a new Event.
		/// </summary>
		Event()
			: _functions(std::vector<func>())
		{}

		~Event()
		{}

		// () operator
		void operator()(T const arg) const
		{
			invoke(arg);
		}

		Event<T>& operator +=(func const& f)
		{
			emplace(f);
			return *this;
		}

		Event<T>& operator -=(func const& f)
		{
			erase(f);
			return *this;
		}

		/// <summary>
		/// Add a new function to be ran when this event is invoked.
		/// </summary>
		/// <param name="f"></param>
		void emplace(func const& f)
		{
			_functions.push_back(f);
		}

		/// <summary>
		/// Remove a function from the invokation list.
		/// </summary>
		/// <param name="f"></param>
		void erase(func const& f)
		{
			for (size_t i = 0; i < _functions.size(); i++)
			{
				// probably not the best way to check if equal
				if (&_functions.at(i) == &f)
				{
					_functions.erase(_functions.begin() + i);
				}
			}
		}

		/// <summary>
		/// Runs the functions within this Event using the given argument.
		/// </summary>
		/// <param name="arg"></param>
		void invoke(T const arg) const
		{
			for (func f : _functions)
			{
				f(arg);
			}
		}
	};
}