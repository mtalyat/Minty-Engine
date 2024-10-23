#pragma once

#include "Minty/Scene/Scene.h"

namespace Minty
{
	//class EntityRegistry;
	//class SystemRegistry;

	/// <summary>
	/// An object that exists within a Scene.
	/// </summary>
	class SceneObject
	{
	private:
		Scene* _scene;

	public:
		SceneObject(Scene& scene)
			: _scene(&scene) {}

		Scene& get_scene() const { return *_scene; }

		EntityRegistry& get_entity_registry() const { return _scene->get_entity_registry(); }

		SystemRegistry& get_system_registry() const { return _scene->get_system_registry(); }

		void set_scene(Scene& scene) { _scene = &scene; }
	};
}