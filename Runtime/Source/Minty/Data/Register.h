#pragma once

#include "Minty/Core/Pointer.h"
#include "Minty/Core/Type.h"
#include "Minty/Core/UUID.h"
#include <unordered_map>

namespace Minty
{
	template<typename T>
	class Register
	{
	private:
		std::unordered_map<UUID, T> m_data;

	public:
		Register()
			: m_data()
		{}

		~Register() = default;

	public:
		UUID emplace(T const& t)
		{
			UUID id = UUID::create();
			m_data.emplace(id, t);
			return id;
		}

		void emplace(UUID const id, T const& t)
		{
			m_data.emplace(id, t);
		}

		void erase(UUID const id)
		{
			m_data.erase(id);
		}

		Bool contains(UUID const id) const
		{
			return m_data.contains(id);
		}

		T& at(UUID const id)
		{
			return m_data.at(id);
		}

		T const& at(UUID const id) const
		{
			return m_data.at(id);
		}

		Size size() const
		{
			return m_data.size();
		}

		Bool empty() const
		{
			return m_data.empty();
		}

		T get(UUID const id) const
		{
			auto found = m_data.find(id);

			if (found == m_data.end())
			{
				return T{};
			}

			return found->second;
		}

		void set(UUID const id, T const& t)
		{
			m_data[id] = t;
		}

		void clear()
		{
			m_data.clear();
		}

		std::unordered_map<UUID, T>::iterator find(UUID const id)
		{
			return m_data.find(id);
		}

		std::unordered_map<UUID, T>::const_iterator find(UUID const id) const
		{
			return m_data.find(id);
		}

		std::unordered_map<UUID, T>::iterator begin()
		{
			return m_data.begin();
		}

		std::unordered_map<UUID, T>::iterator end()
		{
			return m_data.end();
		}

		std::unordered_map<UUID, T>::const_iterator begin() const
		{
			return m_data.begin();
		}

		std::unordered_map<UUID, T>::const_iterator end() const
		{
			return m_data.end();
		}
	};
}