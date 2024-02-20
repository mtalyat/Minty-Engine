#pragma once
#include "M_Object.h"

namespace minty
{
	class Runtime;
	class Scene;

	class Engine
		: public Object
	{
	private:
		Runtime* _runtime;
		Scene* _scene;

	public:
		Engine();

		virtual ~Engine();

		virtual void set_runtime(Runtime& runtime);

		Runtime& get_runtime() const;
	};
}