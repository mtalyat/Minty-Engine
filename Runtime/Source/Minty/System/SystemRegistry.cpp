#include "pch.h"
#include "SystemRegistry.h"

#include "Minty/Core/Console.h"
#include "Minty/Core/Parse.h"
#include "Minty/Serialize/Reader.h"
#include "Minty/Serialize/Writer.h"
#include "Minty/System/AllSystems.h"
#include <sstream>

namespace Minty
{
	std::map<String const, SystemRegistry::SystemFunc const> SystemRegistry::m_systemTypes = std::map<String const, SystemRegistry::SystemFunc const>();

	SystemRegistry::SystemRegistry(Scene& scene)
		: SceneObject(scene)
		, m_orderedSystems()
		, m_allSystems()
		, m_typeLookup()
	{
		//emplace(new AnimationSystem(scene));
		//emplace(new AudioSystem(scene));
		//emplace(new RenderSystem(scene));
		//emplace(new ScriptSystem(scene));
		//emplace(new UISystem(scene));
	}

	SystemRegistry::~SystemRegistry()
	{
		// delete each system
		for (auto const& pair : m_allSystems)
		{
			delete pair.second;
		}
	}

	SystemRegistry::SystemRegistry(SystemRegistry&& other) noexcept
		: SceneObject(std::move(other))
		, m_orderedSystems(std::move(other.m_orderedSystems))
		, m_allSystems(std::move(other.m_allSystems))
		, m_typeLookup(std::move(other.m_typeLookup))
	{}

	SystemRegistry& SystemRegistry::operator=(SystemRegistry&& other) noexcept
	{
		if (this != &other)
		{
			SceneObject::operator=(std::move(other));
			m_orderedSystems = std::move(other.m_orderedSystems);
			m_allSystems = std::move(other.m_allSystems);
			m_typeLookup = std::move(other.m_typeLookup);
		}

		return *this;
	}

	System* SystemRegistry::emplace(System* const system, int const priority)
	{
		MINTY_ASSERT(system != nullptr);

		// if exists in all, do not add a duplicate
		MINTY_ASSERT_FORMAT(!m_allSystems.contains(system->get_name()), "SystemRegistry already contains a System with the name \"{}\". Returning NULL.", system->get_name());

		// add to all systems
		m_allSystems.emplace(system->get_name(), system);
		m_typeLookup.emplace(typeid(*system), system);

		// add to ordered list for updating
		auto found = m_orderedSystems.find(priority);
		if (found == m_orderedSystems.end())
		{
			// new list
			m_orderedSystems.emplace(priority, std::set<System*>());
			m_orderedSystems.at(priority).emplace(system);
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
		MINTY_ASSERT_FORMAT(m_systemTypes.contains(name), "Cannot emplace System \"{}\". It has not been registered with the SystemRegistry.", name);

		// name found
		System* system = m_systemTypes.at(name)(get_scene());
		this->emplace(system, priority);
		return system;
	}

	void SystemRegistry::erase_by_name(String const& name)
	{
		auto found = m_allSystems.find(name);

		if (found != m_allSystems.end())
		{
			m_typeLookup.erase(typeid(*found->second));

			for (auto& pair : m_orderedSystems)
			{
				auto found2 = pair.second.find(found->second);

				if (found2 != pair.second.end())
				{
					pair.second.erase(found2);
				}
			}

			m_allSystems.erase(name);
		}
	}

	Size SystemRegistry::size() const
	{
		return m_allSystems.size();
	}

	void SystemRegistry::load()
	{
		for (auto& pair : m_orderedSystems)
		{
			for (System* const system : pair.second)
			{
				system->load();
			}
		}
	}

	void SystemRegistry::update(Time const time)
	{
		for (auto& pair : m_orderedSystems)
		{
			for (System* const system : pair.second)
			{
				system->update(time);
			}
		}
	}

	void SystemRegistry::finalize()
	{
		for (auto& pair : m_orderedSystems)
		{
			for (System* const system : pair.second)
			{
				system->finalize();
			}
		}
	}

	void SystemRegistry::draw()
	{
		for (auto& pair : m_orderedSystems)
		{
			for (System* const system : pair.second)
			{
				system->draw();
			}
		}
	}

	void SystemRegistry::fixed_update(Time const time)
	{
		for (auto& pair : m_orderedSystems)
		{
			for (System* const system : pair.second)
			{
				system->fixed_update(time);
			}
		}
	}

	void SystemRegistry::unload()
	{
		for (auto& pair : m_orderedSystems)
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
		for (auto const& pair : m_allSystems)
		{
			delete pair.second;
		}

		m_allSystems.clear();
		m_orderedSystems.clear();
		m_typeLookup.clear();
	}

	std::vector<String> SystemRegistry::get_registered_systems()
	{
		std::vector<String> result;
		result.reserve(m_systemTypes.size());

		for (auto const& pair : m_systemTypes)
		{
			result.push_back(pair.first);
		}

		return result;
	}

	void SystemRegistry::serialize(Writer& writer) const
	{
		// serialize each system
		for (auto const& [priority, list] : m_orderedSystems)
		{
			// write priority if not zero
			if (priority == 0)
			{
				for (auto const* system : list)
				{
					writer.write(system->get_name(), *system);
				}
			}
			else
			{
				for (auto const* system : list)
				{
					writer.write(system->get_name(), priority);
					writer.indent();
					system->serialize(writer);
					writer.outdent();
				}
			}
		}
	}

	void SystemRegistry::deserialize(Reader& reader)
	{
		// deserialize each system
		String name;
		Int priority;
		System* system;

		for (Size i = 0; i < reader.size(); i++)
		{
			// get the name
			name = "";
			reader.read_name(i, name);

			// get the priority value
			priority = 0;
			reader.read(i, priority);

			auto found = m_allSystems.find(name);

			if (found == m_allSystems.end())
			{
				// new
				system = emplace_by_name(name, priority);
			}
			else
			{
				// update existing
				system = found->second;

				// TODO: update priority
			}

			// set values
			reader.indent(i);
			system->deserialize(reader);
			reader.outdent();
		}
	}
}
