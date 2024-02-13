#pragma once
#include "M_Object.h"

namespace minty
{
	class Runtime;

	class Engine
		: public Object
	{
	private:
		Runtime* _runtime;

	public:
		Engine();

		virtual ~Engine();

		void set_runtime(Runtime& runtime);

		Runtime& get_runtime() const;
	};
}