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

		void set(String const& name, void const* const data, Size const size)
		{
			MINTY_ASSERT(m_slots.contains(name));

			Slot& slot = m_slots.at(name);

			// copy over data
			Size minSize = Math::min(sizeof_type(slot.type), size);
			slot.container.set(data, minSize);
		}

		void emplace(String const& name, Type const type, void const* const data, Size const size = -1)
		{
			// add slot and set values
			emplace_slot(name, type);
			if (size == -1)
			{
				set(name, data, sizeof_type(type));
			}
			else
			{
				set(name, data, size);
			}
		}

		Bool contains(String const& name) const { return m_slots.contains(name); }

		Slot& at(String const& name) { return m_slots.at(name); }

		Slot const& at(String const& name) const { return m_slots.at(name); }

		// collects all of the cargo into the given container
		void collect(Container& container) const;
	};
}