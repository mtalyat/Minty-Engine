#include "pch.h"
#include "M_SystemManager.h"

namespace minty
{
	SystemManager::SystemManager()
		: _systems()
	{}

	SystemManager::~SystemManager()
	{
		// delete each system
		for (auto& pair : _systems)
		{
			for (auto system : pair.second)
			{
				delete system;
			}
		}

		//delete _systems;
	}

	void SystemManager::emplace(System* const system, int const priority)
	{
		auto found = _systems.find(priority);

		if (found == _systems.end())
		{
			// new list
			_systems.emplace(priority, std::set<System*>());
			_systems.at(priority).emplace(system);
		}
		else
		{
			// existing list
			found->second.emplace(system);
		}
	}

	void SystemManager::erase(System* const system)
	{
		// find system, remove it from list
		for (auto& pair : _systems)
		{
			if (pair.second.erase(system))
			{
				// found
				return;
			}
		}
	}

	void SystemManager::load()
	{
		for (auto& pair : _systems)
		{
			for (auto system : pair.second)
			{
				system->load();
			}
		}
	}

	void SystemManager::update()
	{
		for (auto& pair : _systems)
		{
			for (auto system : pair.second)
			{
				if (system->isEnabled())
				{
					system->update();
				}
			}
		}
	}

	void SystemManager::fixedUpdate()
	{
		for (auto& pair : _systems)
		{
			for (auto system : pair.second)
			{
				if (system->isEnabled())
				{
					system->fixedUpdate();
				}
			}
		}
	}

	void SystemManager::unload()
	{
		for (auto& pair : _systems)
		{
			for (auto system : pair.second)
			{
				system->unload();
			}
		}
	}
}
