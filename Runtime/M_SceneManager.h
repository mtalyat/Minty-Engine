#pragma once

#include "M_Object.h"
#include "M_Scene.h"
#include <vector>

namespace minty
{
	class SceneManager
		: public Object
	{
	private:
		std::vector<Scene> _scenes;

	public:
		SceneManager();

		ID create_scene();

		ID load_scene(std::string const& path);

		Scene& get_scene(ID const id);
	};
}