#pragma once

#include "Minty/Core/Macros.h"
#include "Minty/Core/Math.h"
#include "Minty/Data/ConstantContainer.h"
#include "Minty/Data/DynamicContainer.h"

namespace Minty
{
	// a collection of Containers
	class Cargo
	{
	public:
		struct Slot
		{
			String name;
			Type type;
			ConstantContainer container;
		};

	private:
		std::unordered_map<String, Slot> m_slots;

	public:
		Cargo()
			: m_slots()
		{}

	public:
		void emplace_slot(String const& name, Type const type)
		{
			MINTY_ASSERT_FORMAT(!m_slots.contains(name), "Cannot add slot name \"{}\" to Cargo: name already exists.", name);

			m_slots.emplace(name, Slot{ name, type, ConstantContainer(sizeof_type(type)) });
		}

		void set(String const& name, Container const& container)
		{
			MINTY_ASSERT(m_slots.contains(name));

			Slot& slot = m_slots.at(name);

			// copy over data
			Size size = Math::min(sizeof_type(slot.type), container.size());
			slot.container.set(container.data(), size);
		}

		Bool contains(String const& name) const { return m_slots.contains(name); }

		Slot& at(String const& name) { return m_slots.at(name); }

		Slot const& at(String const& name) const { return m_slots.at(name); }

		// collects all of the cargo into the given container
		void collect(Container& container) const;
	};
}