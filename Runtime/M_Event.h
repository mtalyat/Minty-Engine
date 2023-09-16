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
		std::vector<func>* mp_functions;

	public:

		/// <summary>
		/// Creates a new Event.
		/// </summary>
		Event()
			: mp_functions(new std::vector<func>())
		{}

		~Event()
		{
			delete mp_functions;
		}

		// () operator
		void operator()(T const arg) const
		{
			invoke(arg);
		}

		/// <summary>
		/// Add a new function to be ran when this event is invoked.
		/// </summary>
		/// <param name="f"></param>
		void emplace(func const& f)
		{
			mp_functions->push_back(f);
		}

		/// <summary>
		/// Runs the functions within this Event using the given argument.
		/// </summary>
		/// <param name="arg"></param>
		void invoke(T const arg) const
		{
			for (func f : *mp_functions)
			{
				f(arg);
			}
		}
	};

	template<>
	class Event<void>
		: public Object
	{
	public:
		typedef std::function<void(void)> func;

	private:
		std::vector<func>* mp_functions;

	public:

		/// <summary>
		/// Creates a new Event.
		/// </summary>
		Event()
			: mp_functions(new std::vector<func>())
		{}

		~Event()
		{
			delete mp_functions;
		}

		// () operator
		void operator()() const
		{
			invoke();
		}

		/// <summary>
		/// Add a new function to be ran when this event is invoked.
		/// </summary>
		/// <param name="f"></param>
		inline void emplace(func const& f)
		{
			mp_functions->push_back(f);
		}

		/// <summary>
		/// Run the functions within this Event.
		/// </summary>
		void invoke() const
		{
			for (func f : *mp_functions)
			{
				f();
			}
		}
	};
}