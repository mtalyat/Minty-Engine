#pragma once

#include "Minty/Core/Type.h"
#include "Minty/Data/StaticContainer.h"

namespace Minty
{
	/// <summary>
	/// A container with a Type.
	/// </summary>
	class Variable
	{
	private:
		Type m_type;
		StaticContainer m_container;

	public:
		Variable()
			: m_type(Type::Undefined)
			, m_container(0)
		{}

		Variable(Type const type)
			: m_type(type)
			, m_container(sizeof_type(type))
		{}

		Variable(Type const type, void const* const data)
			: m_type(type)
			, m_container(sizeof_type(type))
		{
			// set data in container: assuming data is valid
			m_container.set(data, m_container.size());
		}

		~Variable() = default;

		Type type() const { return m_type; }

		template<typename T>
		T get() const
		{
			if (m_type != Type::Undefined)
			{
				return m_container.get<T>();
			}
			else
			{
				return T{};
			}
		}

		template<typename T>
		T set(T const& value)
		{
			if (m_type != Type::Undefined)
			{
				m_container.set(&value, sizeof(T));
			}
		}

		void* data() const { return m_container.data(); }

		Size size() const { return m_container.size(); }
	};
}