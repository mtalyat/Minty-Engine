#pragma once
#include "M_Object.h"

namespace minty
{
	class Runtime;
	class Scene;

	class Engine
		: public Object
	{
		friend class Runtime;

	private:
		Runtime* _runtime;
		Scene* _loadedScene;
		Scene* _workingScene;

	public:
		Engine(Runtime& runtime);

		virtual ~Engine();

		Runtime& get_runtime() const;

		virtual void set_loaded_scene(Scene* const scene);

		Scene* get_loaded_scene() const;

		virtual void set_working_scene(Scene* const scene);

		Scene* get_working_scene() const;
	};
}