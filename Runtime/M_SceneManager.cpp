#include "pch.h"
#include "M_SceneManager.h"

#include "M_Runtime.h"
#include "M_SerializedNode.h"
#include "M_Reader.h"

#include "M_NameComponent.h"

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
	SerializedNode node = SerializedNode::parse_file(path);

	// parse

	// if "Systems" exists, load systems based on name
	auto found = node.children.find("Systems");
	if (found != node.children.end())
	{
		SystemRegistry& systemRegistry = scene.get_system_registry();

		// iterate through list of systems
		for (auto const& pair : found->second.children)
		{
			// system could have priority listed, otherwise default to zero
			int priority = 0;
			pair.second.try_get_int(priority);
			systemRegistry.emplace_by_name(pair.first, priority);
		}
	}

	// if "Entities" exists, create entities based on their components
	found = node.children.find("Entities");
	if (found != node.children.end())
	{
		EntityRegistry& entityRegistry = scene.get_entity_registry();
		
		// iterate through list of entities
		for (auto const& pair : found->second.children)
		{
			// create entity
			Entity const entity = entityRegistry.create();

			// add name if there is one
			// empty name is either "" or "_"
			if (pair.first.size() > 1 || (pair.first.size() == 1 && pair.first.at(0) != '_'))
			{
				NameComponent& name = entityRegistry.emplace<NameComponent>(entity);
				name.name = pair.first;
			}
			
			// now add the other components (children)
			for (auto& componentPair : pair.second.children)
			{
				Component* component = entityRegistry.emplace_by_name(componentPair.first, entity);
				Reader reader(componentPair.second);
				component->deserialize(reader);
			}
		}
	}

	return id;
}

Scene& minty::SceneManager::get_scene(ID const id)
{
	return _scenes.at(id);
}
