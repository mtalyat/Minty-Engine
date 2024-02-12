#include "pch.h"
#include "M_SystemRegistry.h"

#include "M_System.h"
#include "M_Console.h"
#include "M_Runtime.h"
#include "M_Parse.h"

#include <sstream>

namespace minty
{
	std::map<String const, SystemRegistry::SystemFunc const> SystemRegistry::_systemTypes = std::map<String const, SystemRegistry::SystemFunc const>();

	SystemRegistry::SystemRegistry(Runtime& engine, ID const sceneId)
		: SceneObject(engine, sceneId)
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
		: SceneObject(other)
		, _orderedSystems(std::move(other._orderedSystems))
		, _allSystems(std::move(other._allSystems))
	{}

	SystemRegistry& SystemRegistry::operator=(SystemRegistry&& other) noexcept
	{
		if (this != &other)
		{
			SceneObject::operator=(std::move(other));
			_orderedSystems = std::move(other._orderedSystems);
			_allSystems = std::move(other._allSystems);
		}

		return *this;
	}

	System* SystemRegistry::emplace(String const& name, System* const system, int const priority)
	{
		// if exists in all, do not add a duplicate
		if (_allSystems.contains(name))
		{
			Console::error(std::format("SystemRegistry already contains a System with the name \"{}\". Returning NULL.", name));
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

	System* SystemRegistry::emplace_by_name(String const& name, int const priority)
	{
		auto found = _systemTypes.find(name);
		if (found == _systemTypes.end())
		{
			// name not found
			Console::error(std::format("Cannot emplace System \"{}\". It has not been registered with the SystemRegistry.", name));
			return nullptr;
		}
		else
		{
			// name found
			System* system = found->second(get_engine(), get_scene_id());
			this->emplace(name, system, priority);
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
		std::map<System* const, String> lookup;

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
				String systemPriority = "";
				if (pair.first)
				{
					systemPriority = std::to_string(pair.first);
				}
				Node systemNode(lookup.at(system), systemPriority);
				Writer systemWriter(systemNode, writer.get_data());
				system->serialize(systemWriter);

				writer.write(systemNode);
			}
		}
	}

	void SystemRegistry::deserialize(Reader const& reader)
	{
		// read each one and set as we go, by name
		for (auto const& child : reader.get_node().get_children())
		{
			// create the system
			System* system = emplace_by_name(child.get_name(), child.to_int());

			// deserialize the system
			Reader systemReader(child, reader.get_data());
			system->deserialize(systemReader);
		}
	}

	String to_string(SystemRegistry const& value)
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
