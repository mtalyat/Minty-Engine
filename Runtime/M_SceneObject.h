#pragma once
#include "M_Object.h"

namespace minty
{
	class Runtime;
	class Scene;

	class SceneObject
		: public Object
	{
	private:
		Runtime* _runtime;

		ID _sceneId;
	public:
		SceneObject();

		SceneObject(Runtime& engine, ID const sceneId);

		virtual ~SceneObject();

	public:
		Runtime& get_runtime() const;

		void set_runtime(Runtime& engine);

		Scene& get_scene() const;

		ID get_scene_id() const;

		void set_scene(ID const sceneId);
	};
}