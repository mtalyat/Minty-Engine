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
		Runtime* _engine;

		ID _sceneId;
	public:
		SceneObject();

		SceneObject(Runtime& engine, ID const sceneId);

		virtual ~SceneObject();

	public:
		Runtime& get_engine() const;

		void set_engine(Runtime& engine);

		Scene& get_scene() const;

		ID get_scene_id() const;

		void set_scene(ID const sceneId);
	};
}