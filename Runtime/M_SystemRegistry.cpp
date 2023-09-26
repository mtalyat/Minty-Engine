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
		, _orderedSystems()
		, _allSystems()
	{}

	SystemRegistry::~SystemRegistry()
	{
		// delete each system
		for (auto& pair : _allSystems)
		{
			delete pair.second;
		}
	}

	SystemRegistry::SystemRegistry(SystemRegistry&& other) noexcept
		: _engine(other._engine)
		, _registry(other._registry)
		, _orderedSystems(std::move(other._orderedSystems))
		, _allSystems(std::move(other._allSystems))
	{
		other._engine = nullptr;
		other._registry = nullptr;
	}

	SystemRegistry& SystemRegistry::operator=(SystemRegistry&& other) noexcept
	{
		if (this != &other)
		{
			_engine = other._engine;
			_registry = other._registry;
			_orderedSystems = std::move(other._orderedSystems);
			_allSystems = std::move(other._allSystems);

			other._engine = nullptr;
			other._registry = nullptr;
		}

		return *this;
	}

	System* SystemRegistry::emplace(System* const system, int const priority)
	{
		auto found = _orderedSystems.find(priority);

		if (found == _orderedSystems.end())
		{
			// new list
			_orderedSystems.emplace(priority, std::set<System*>());
			_orderedSystems.at(priority).emplace(system);
		}
		else
		{
			// existing list
			found->second.emplace(system);
		}

		return system;
	}

	System* SystemRegistry::emplace_by_name(std::string const& name, int const priority)
	{
		auto const& found = _systemTypes.find(name);
		if (found == _systemTypes.end())
		{
			// name not found
			throw std::runtime_error(std::format("Cannot emplace System \"{}\". It has not been registered with the SystemRegistry.", name));
		}
		else
		{
			// name found
			System* system = found->second(_engine, _registry);
			this->emplace(system, priority);
			return system;
		}
	}

	void SystemRegistry::erase(System* const system)
	{
		// find system, remove it from list
		for (auto& pair : _orderedSystems)
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
		for (auto& pair : _orderedSystems)
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
		for (auto& pair : _orderedSystems)
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
		for (auto& pair : _orderedSystems)
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
		for (auto& pair : _orderedSystems)
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
		if (_orderedSystems.size() == 0)
		{
			return "SystemRegistry()";
		}

		// if at least one system in registry
		std::stringstream stream;
		stream << "SystemRegistry(";

		size_t i = 0;
		for (auto const& pair : _orderedSystems)
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
