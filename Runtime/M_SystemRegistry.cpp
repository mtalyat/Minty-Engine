#include "pch.h"
#include "M_SystemRegistry.h"

#include "M_System.h"
#include "M_Console.h"
#include "M_Runtime.h"
#include "M_Parse.h"
#include "M_Reader.h"
#include "M_Writer.h"

#include <sstream>

namespace minty
{
	std::map<String const, SystemRegistry::SystemFunc const> SystemRegistry::_systemTypes = std::map<String const, SystemRegistry::SystemFunc const>();

	SystemRegistry::SystemRegistry(Runtime& engine, ID const sceneId)
		: SceneObject(engine, sceneId)
		, _systems()
		, _orderedSystems()
		, _allSystems()
	{}

	SystemRegistry::~SystemRegistry()
	{
		// delete each system
		for (auto const system : _systems)
		{
			delete system;
		}
	}

	SystemRegistry::SystemRegistry(SystemRegistry&& other) noexcept
		: SceneObject(other)
		, _systems(std::move(other._systems))
		, _orderedSystems(std::move(other._orderedSystems))
		, _allSystems(std::move(other._allSystems))
	{}

	SystemRegistry& SystemRegistry::operator=(SystemRegistry&& other) noexcept
	{
		if (this != &other)
		{
			SceneObject::operator=(std::move(other));
			_systems = std::move(other._systems);
			_orderedSystems = std::move(other._orderedSystems);
			_allSystems = std::move(other._allSystems);
		}

		return *this;
	}

	System* SystemRegistry::emplace(System* const system, int const priority)
	{
		MINTY_ASSERT(system != nullptr);

		// if exists in all, do not add a duplicate
		MINTY_ASSERT_FORMAT(!_allSystems.contains(system->get_name()), "SystemRegistry already contains a System with the name \"{}\". Returning NULL.", system->get_name());

		// add to all systems
		index_t index = _systems.size();
		_systems.push_back(system);
		_allSystems.emplace(system->get_name(), index);

		// add to ordered list for updating
		auto found = _orderedSystems.find(priority);
		if (found == _orderedSystems.end())
		{
			// new list
			_orderedSystems.emplace(priority, std::set<index_t>());
			_orderedSystems.at(priority).emplace(index);
		}
		else
		{
			// existing list
			found->second.emplace(index);
		}

		return system;
	}

	System* SystemRegistry::emplace_by_name(String const& name, int const priority)
	{
		MINTY_ASSERT_FORMAT(_systemTypes.contains(name), "Cannot emplace System \"{}\". It has not been registered with the SystemRegistry.", name);

		// name found
		System* system = _systemTypes.at(name)(get_runtime(), get_scene_id());
		this->emplace(system, priority);
		return system;
	}

	void SystemRegistry::erase_by_name(String const& name)
	{
		auto found = _allSystems.find(name);

		if (found != _allSystems.end())
		{
			index_t index = found->second;

			_systems.erase(_systems.begin() + index);
			_allSystems.erase(name);
			
			for (auto& pair : _orderedSystems)
			{
				auto found2 = pair.second.find(index);

				if (found2 != pair.second.end())
				{
					pair.second.erase(found2);
				}
			}
		}
	}

	size_t SystemRegistry::size() const
	{
		return _systems.size();
	}

	void SystemRegistry::load()
	{
		for (auto& pair : _orderedSystems)
		{
			for (auto index : pair.second)
			{
				_systems.at(index)->load();
			}
		}
	}

	void SystemRegistry::update()
	{
		System* system;
		for (auto& pair : _orderedSystems)
		{
			for (auto index : pair.second)
			{
				system = _systems.at(index);
				if (system->is_enabled())
				{
					system->update();
				}
			}
		}
	}

	void SystemRegistry::fixed_update()
	{
		System* system;
		for (auto& pair : _orderedSystems)
		{
			for (auto index : pair.second)
			{
				system = _systems.at(index);
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
			for (auto index : pair.second)
			{
				_systems.at(index)->unload();
			}
		}
	}

	void SystemRegistry::clear()
	{
		// delete all systems
		for (auto const system : _systems)
		{
			delete system;
		}

		_systems.clear();
		_allSystems.clear();
		_orderedSystems.clear();
	}

	std::vector<System*>::iterator SystemRegistry::begin()
	{
		return _systems.begin();
	}

	std::vector<System*>::iterator SystemRegistry::end()
	{
		return _systems.end();
	}

	std::vector<System*>::const_iterator SystemRegistry::cbegin() const
	{
		return _systems.cbegin();
	}

	std::vector<System*>::const_iterator SystemRegistry::cend() const
	{
		return _systems.cend();
	}

	std::vector<System*>::const_iterator SystemRegistry::begin() const
	{
		return _systems.begin();
	}

	std::vector<System*>::const_iterator SystemRegistry::end() const
	{
		return _systems.end();
	}

	void SystemRegistry::serialize(Writer& writer) const
	{
		// create reverse lookup for names
		std::map<System* const, String> lookup;

		for (auto const& pair : _allSystems)
		{
			lookup.emplace(_systems.at(pair.second), pair.first);
		}

		// write all systems:
		// name: priority

		for (auto const& pair : _orderedSystems)
		{
			for (auto const index : pair.second)
			{
				System* system = _systems.at(index);
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
