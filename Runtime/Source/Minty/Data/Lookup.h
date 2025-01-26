#pragma once

#include "Minty/Core/String.h"
#include "Minty/Core/ToString.h"
#include "Minty/Core/Type.h"
#include "Minty/Core/UUID.h"
#include <unordered_map>
#include <vector>

namespace Minty
{
	template<typename T>
	class Lookup
	{
	public:
		struct Data
		{
			UUID id;
			String name;
			T value;
		};

	private:
		std::unordered_map<UUID, Size> m_ids;
		std::unordered_map<String, Size> m_names;
		std::vector<Data> m_values;

	public:
		Lookup()
			: m_ids()
			, m_names()
			, m_values()
		{}

		~Lookup() = default;

	public:
		void emplace(UUID const id, String const& name, T const& value)
		{
			MINTY_ASSERT_FORMAT(!m_ids.contains(id), "Cannot emplace ID of \"{}\" into the Lookup. The ID already exists.", to_string(id));
 			MINTY_ASSERT_FORMAT(!m_names.contains(name), "Cannot emplace name of \"{}\" into the Lookup. The name already exists.", name);

			Size index = m_values.size();
			m_ids.emplace(id, index);
			m_names.emplace(name, index);
			m_values.push_back(Data
				{
					id, name, value
				});
		}

		Bool contains(UUID const id) const { return m_ids.contains(id); }

		Bool contains(String const& name) const { return m_names.contains(name); }

		T& at(UUID const id) { return m_values.at(m_ids.at(id)).value; }
		T const& at(UUID const id) const { return m_values.at(m_ids.at(id)).value; }

		T& at(String const& name) { return m_values.at(m_names.at(name)).value; }
		T const& at(String const& name) const { return m_values.at(m_names.at(name)).value; }

		UUID get_id(String const& name) const { return m_values.at(m_names.at(name)).id; }

		String const& get_name(UUID const id) const { return m_values.at(m_ids.at(id)).name; }

		void erase(UUID const id)
		{
			Size index = m_ids.at(id);
			
			erase(index);
		}

		void erase(String const& name)
		{
			Size index = m_names.at(name);

			erase(index);
		}

		Size size() const
		{
			return m_values.size();
		}

		void clear()
		{
			m_values.clear();
			m_ids.clear();
			m_names.clear();
		}

		std::vector<Data>::iterator begin()
		{
			return m_values.begin();
		}

		std::vector<Data>::iterator end()
		{
			return m_values.end();
		}

		std::vector<Data>::const_iterator begin() const
		{
			return m_values.begin();
		}

		std::vector<Data>::const_iterator end() const
		{
			return m_values.end();
		}

	private:
		void erase(Size const index)
		{
			Data const& data = m_values.at(index);
			m_ids.erase(data.id);
			m_names.erase(data.name);
			m_values.erase(m_values.begin() + index);
		}
	};
}