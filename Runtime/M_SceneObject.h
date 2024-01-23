#pragma once

#include "M_Object.h"

namespace minty
{
	class Engine;
	class Scene;

	class SceneObject
		: public Object
	{
	private:
		Engine* _engine;

		ID _sceneId;
	public:
		SceneObject();

		SceneObject(Engine& engine, ID const sceneId);

		virtual ~SceneObject();

	public:
		Engine& get_engine() const;

		void set_engine(Engine& engine);

		Scene& get_scene() const;

		ID get_scene_id() const;

		void set_scene(ID const sceneId);
	};
}