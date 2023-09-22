#include "pch.h"
#include "M_SceneManager.h"

#include "M_Serializer.h"

using namespace minty;

minty::SceneManager::SceneManager()
	: _scenes()
{}

ID minty::SceneManager::create_scene()
{
	ID id = static_cast<ID>(_scenes.size());
	_scenes.push_back(Scene());
	return id;
}

ID minty::SceneManager::load_scene(std::string const& path)
{
	// create the scene
	ID id = create_scene();
	Scene& scene = _scenes.at(id);

	// load the data from the disk
	SerializedNode node = Serializer::parse_file(path);

	// parse

	// if "Systems" exists, load systems based on name
	if (node.children.find("Systems") != node.children.end())
	{

	}

	// if "Entities" exists, create entities based on their components
	if (node.children.find("Entities") != node.children.end())
	{

	}
}

Scene& minty::SceneManager::get_scene(ID const id)
{
	return _scenes.at(id);
}
