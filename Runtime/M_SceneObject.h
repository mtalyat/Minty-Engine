#pragma once
#include "M_RuntimeObject.h"

namespace minty
{
	class Scene;
	class EntityRegistry;
	class SystemRegistry;

	/// <summary>
	/// An object that exists within a Scene.
	/// </summary>
	class SceneObject
		: public RuntimeObject
	{
	private:
		Scene* _scene;

	public:
		SceneObject(Runtime& runtime, Scene& scene);

		Scene& get_scene() const;

		EntityRegistry& get_entity_registry() const;

		SystemRegistry& get_system_registry() const;

		void set_scene(Scene& scene);
	};
}