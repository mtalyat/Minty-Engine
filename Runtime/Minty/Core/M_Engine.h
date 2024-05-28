#pragma once
#include "Minty/Types/M_Object.h"

namespace Minty
{
	class Scene;

	/// <summary>
	/// The base class for all Engines.
	/// </summary>
	class Engine
		: public Object
	{
	private:
		Ref<Scene> _loadedScene;
		Ref<Scene> _workingScene;

	public:
		Engine() = default;
		virtual ~Engine() = default;

		virtual void set_loaded_scene(Ref<Scene> const scene) { _loadedScene = scene; }

		Ref<Scene> get_loaded_scene() const { return _loadedScene; }

		virtual void set_working_scene(Ref<Scene> const scene) { _workingScene = scene; }

		Ref<Scene> get_working_scene() const { return _workingScene; }
	};
}