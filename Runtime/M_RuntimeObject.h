#pragma once
#include "M_Object.h"

namespace minty
{
	class Runtime;

	/// <summary>
	/// An object that exists within a Runtime.
	/// </summary>
	class RuntimeObject
		: public Object
	{
	private:
		Runtime* _runtime;

	public:
		RuntimeObject();

		RuntimeObject(Runtime& engine);

		virtual ~RuntimeObject();

	public:
		Runtime& get_runtime() const;

		void set_runtime(Runtime& engine);
	};
}