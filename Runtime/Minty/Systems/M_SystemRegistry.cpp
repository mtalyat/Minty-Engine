#include "pch.h"
#include "Minty/Systems/M_SystemRegistry.h"

#include "Minty/Systems/M_System.h"
#include "Minty/Tools/M_Console.h"
#include "Minty/Tools/M_Parse.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

#include <sstream>

namespace Minty
{
	std::map<String const, SystemRegistry::SystemFunc const> SystemRegistry::_systemTypes = std::map<String const, SystemRegistry::SystemFunc const>();

	SystemRegistry::~SystemRegistry()
	{
		// delete each system
		for (auto const& pair : _allSystems)
		{
			delete pair.second;
		}
	}

	SystemRegistry::SystemRegistry(SystemRegistry&& other) noexcept
		: SceneObject(std::move(other))
		, _orderedSystems(std::move(other._orderedSystems))
		, _allSystems(std::move(other._allSystems))
		, _typeLookup(std::move(other._typeLookup))
	{}

	SystemRegistry& SystemRegistry::operator=(SystemRegistry&& other) noexcept
	{
		if (this != &other)
		{
			SceneObject::operator=(std::move(other));
			_orderedSystems = std::move(other._orderedSystems);
			_allSystems = std::move(other._allSystems);
			_typeLookup = std::move(other._typeLookup);
		}

		return *this;
	}

	System* SystemRegistry::emplace(System* const system, int const priority)
	{
		MINTY_ASSERT(system != nullptr);

		// if exists in all, do not add a duplicate
		MINTY_ASSERT_FORMAT(!_allSystems.contains(system->get_name()), "SystemRegistry already contains a System with the name \"{}\". Returning NULL.", system->get_name());

		// add to all systems
		_allSystems.emplace(system->get_name(), system);
		_typeLookup.emplace(typeid(*system), system);

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
		MINTY_ASSERT_FORMAT(_systemTypes.contains(name), "Cannot emplace System \"{}\". It has not been registered with the SystemRegistry.", name);

		// name found
		System* system = _systemTypes.at(name)(get_scene());
		this->emplace(system, priority);
		return system;
	}

	void SystemRegistry::erase_by_name(String const& name)
	{
		auto found = _allSystems.find(name);

		if (found != _allSystems.end())
		{
			_typeLookup.erase(typeid(*found->second));

			for (auto& pair : _orderedSystems)
			{
				auto found2 = pair.second.find(found->second);

				if (found2 != pair.second.end())
				{
					pair.second.erase(found2);
				}
			}

			_allSystems.erase(name);
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
			for (System* const system : pair.second)
			{
				system->load();
			}
		}
	}

	void SystemRegistry::update(Time const time)
	{
		for (auto& pair : _orderedSystems)
		{
			for (System* const system : pair.second)
			{
				system->update(time);
			}
		}
	}

	void SystemRegistry::fixed_update(Time const time)
	{
		for (auto& pair : _orderedSystems)
		{
			for (System* const system : pair.second)
			{
				system->fixed_update(time);
			}
		}
	}

	void SystemRegistry::unload()
	{
		for (auto& pair : _orderedSystems)
		{
			for (System* const system : pair.second)
			{
				system->unload();
			}
		}
	}

	void SystemRegistry::clear()
	{
		// delete all systems
		for (auto const& pair : _allSystems)
		{
			delete pair.second;
		}

		_allSystems.clear();
		_orderedSystems.clear();
		_typeLookup.clear();
	}

	std::vector<String> SystemRegistry::get_registered_systems()
	{
		std::vector<String> result;
		result.reserve(_systemTypes.size());

		for (auto const& pair : _systemTypes)
		{
			result.push_back(pair.first);
		}

		return result;
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
			for (System* const system : pair.second)
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
