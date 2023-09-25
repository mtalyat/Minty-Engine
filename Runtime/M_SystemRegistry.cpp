#include "pch.h"
#include "M_SystemRegistry.h"

#include "M_Console.h"
#include "M_Engine.h"

#include <sstream>

namespace minty
{
	std::map<std::string const, SystemRegistry::SystemFunc const> SystemRegistry::_systemTypes = std::map<std::string const, SystemRegistry::SystemFunc const>();

	SystemRegistry::SystemRegistry(Engine* const engine, EntityRegistry* const registry)
		: _engine(engine)
		, _registry(registry)
		, _systems(new std::map<int, std::set<System*>>())
	{}

	SystemRegistry::~SystemRegistry()
	{
		// delete each system
		for (auto& pair : *_systems)
		{
			for (auto system : pair.second)
			{
				delete system;
			}
		}
		delete _systems;
	}

	SystemRegistry::SystemRegistry(SystemRegistry&& other) noexcept
		: _engine(other._engine)
		, _registry(other._registry)
		, _systems(other._systems)
	{
		other._engine = nullptr;
		other._registry = nullptr;
		other._systems = nullptr;
	}

	SystemRegistry& SystemRegistry::operator=(SystemRegistry&& other) noexcept
	{
		if (this != &other)
		{
			_engine = other._engine;
			_registry = other._registry;
			_systems = other._systems;

			other._engine = nullptr;
			other._registry = nullptr;
			other._systems = nullptr;
		}

		return *this;
	}

	void SystemRegistry::emplace(System* const system, int const priority)
	{
		auto found = _systems->find(priority);

		if (found == _systems->end())
		{
			// new list
			_systems->emplace(priority, std::set<System*>());
			_systems->at(priority).emplace(system);
		}
		else
		{
			// existing list
			found->second.emplace(system);
		}
	}

	void SystemRegistry::emplace_by_name(std::string const& name, int const priority)
	{
		auto const& found = _systemTypes.find(name);
		if (found == _systemTypes.end())
		{
			// name not found
			console::error(std::format("Cannot emplace System \"{}\". It has not been registered with the SystemRegistry.", name));
		}
		else
		{
			// name found
			this->emplace(found->second(_engine, _registry), priority);
		}
	}

	void SystemRegistry::erase(System* const system)
	{
		// find system, remove it from list
		for (auto& pair : *_systems)
		{
			if (pair.second.erase(system))
			{
				// found
				return;
			}
		}
	}

	void SystemRegistry::load()
	{
		for (auto& pair : *_systems)
		{
			for (auto system : pair.second)
			{
				console::log(std::format("Loading system: {}", typeid(system).name()));
				system->load();
			}
		}
	}

	void SystemRegistry::update()
	{
		for (auto& pair : *_systems)
		{
			for (auto system : pair.second)
			{
				if (system->is_enabled())
				{
					system->update();
				}
			}
		}
	}

	void SystemRegistry::fixed_update()
	{
		for (auto& pair : *_systems)
		{
			for (auto system : pair.second)
			{
				if (system->is_enabled())
				{
					system->fixed_update();
				}
			}
		}
	}

	void SystemRegistry::unload()
	{
		for (auto& pair : *_systems)
		{
			for (auto system : pair.second)
			{
				system->unload();
			}
		}
	}

	std::string const SystemRegistry::to_string() const
	{
		// if no systems in registry
		if (_systems->size() == 0)
		{
			return "SystemRegistry()";
		}

		// if at least one system in registry
		std::stringstream stream;
		stream << "SystemRegistry(";

		size_t i = 0;
		for (auto const& pair : *_systems)
		{
			for (auto const& system : pair.second)
			{
				if (i > 0) stream << ' ';

				stream << system->to_string();

				i++;
			}
		}

		stream << ")";

		return stream.str();
	}
}
