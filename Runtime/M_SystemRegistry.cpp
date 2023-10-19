#include "pch.h"
#include "M_SystemRegistry.h"

#include "M_Console.h"
#include "M_Engine.h"
#include "M_Parse.h"

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

	System* SystemRegistry::emplace(std::string const& name, System* const system, int const priority)
	{
		// if exists in all, do not add a duplicate
		if (_allSystems.contains(name))
		{
			console::error(std::format("SystemRegistry already contains a System with the name \"{}\". Returning NULL.", name));
			return nullptr;
		}

		// add to all systems
		_allSystems.emplace(name, system);

		// add to ordered list for updating
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
			console::error(std::format("Cannot emplace System \"{}\". It has not been registered with the SystemRegistry.", name));
			return nullptr;
		}
		else
		{
			// name found
			System* system = found->second(_engine, _registry);
			this->emplace(name, system, priority);
			return system;
		}
	}

	System* SystemRegistry::find_by_name(std::string const& name) const
	{
		for (auto const& pair : _allSystems)
		{
			if (pair.first.compare(name) == 0)
			{
				// found name, return system
				return pair.second;
			}
		}

		// not found
		return nullptr;
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

	size_t SystemRegistry::size() const
	{
		return _allSystems.size();
	}

	void SystemRegistry::load()
	{
		for (auto& pair : _orderedSystems)
		{
			for (auto system : pair.second)
			{
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

	void SystemRegistry::clear()
	{
		// delete all systems
		for (auto& pair : _allSystems)
		{
			delete pair.second;
		}

		_allSystems.clear();
		_orderedSystems.clear();
	}
	
	void SystemRegistry::serialize(Writer& writer) const
	{
		// create reverse lookup for names
		std::map<System* const, std::string> lookup;

		for (auto const& pair : _allSystems)
		{
			lookup.emplace(pair.second, pair.first);
		}

		// write all systems:
		// name: priority

		for (auto const& pair : _orderedSystems)
		{
			for (auto const system : pair.second)
			{
				writer.write(lookup.at(system), pair.first);
			}
		}
	}

	void SystemRegistry::deserialize(Reader const& reader)
	{
		// read each one and set as we go, by name
		Node const* node = reader.get_node();

		for (auto const& pair : node->children)
		{
			emplace_by_name(pair.first, pair.second.to_int());
		}
	}

	std::string to_string(SystemRegistry const& value)
	{
		// if no systems in registry
		if (value._orderedSystems.size() == 0)
		{
			return "SystemRegistry()";
		}

		// if at least one system in registry
		std::stringstream stream;
		stream << "SystemRegistry(";

		size_t i = 0;
		for (auto const& pair : value._orderedSystems)
		{
			for (auto const system : pair.second)
			{
				for (auto const& pair : value._allSystems)
				{
					if (pair.second == system)
					{
						// match, use name
						if (i > 0) stream << ' ';

						stream << pair.first;

						i++;
						break;
					}
				}

				// ignore if no match found, I guess
			}
		}

		stream << ")";

		return stream.str();
	}
}
