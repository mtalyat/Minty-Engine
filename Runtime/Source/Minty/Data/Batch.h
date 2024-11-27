#pragma once

#include "Minty/Core/Type.h"
#include "Minty/Data/DynamicContainer.h"
#include <tuple>
#include <variant>

namespace Minty
{
	/// <summary>
	/// Holds a batch, or a compilation of a collection of data. Useful for rendering batches.
	/// </summary>
	template<typename Size N, typename ...Args>
	class Batch
	{
	private:
		std::array<std::variant<Args...>, N> m_objects;

		/// <summary>
		/// The compilation of data for this Batch.
		/// </summary>
		DynamicContainer m_data;

		/// <summary>
		/// The number of elements that have been included in this Batch.
		/// </summary>
		Size m_count;

		Size m_hash;

	public:
		Batch(std::array<std::variant<Args...>, sizeof...(Args)> const& objects, Size const dataCapacity = 0)
			: m_objects(objects)
			, m_data(dataCapacity)
			, m_count(0)
			, m_hash(generate_hash(objects))
		{}

		~Batch() = default;

	public:
		constexpr Size get_object_count() const { return sizeof...(Args); }

		std::array<std::variant<Args...>, N>& get_objects() { return m_objects; }
		std::array<std::variant<Args...>, N> const& get_objects() const { return m_objects; }

		template<typename T>
		T& get_object(Size const index)
		{
			auto& element = m_objects[index];

			if (auto* val = std::get_if<T>(&element))
			{
				return *val;
			}

			// not a valid type
			throw std::bad_variant_access();
		}

		template<typename T>
		T const& get_object(Size const index) const
		{
			auto const& element = m_objects[index];

			if (auto* val = std::get_if<T>(&element))
			{
				return *val;
			}

			// not a valid type
			throw std::bad_variant_access();
		}

		inline DynamicContainer& get_data_container() { return m_data; }

		inline DynamicContainer const& get_data_container() const { return m_data; }

		inline void* get_data() const { return m_data.data(); }

		inline Size get_data_size() const { return m_data.size(); }

		inline Size get_count() const { return m_count; }

		inline Bool empty() const { return m_count == 0; }

		inline void set_count(Size const count) const { m_count = count; }

		inline void increment() { m_count += 1; }

		inline Size get_hash() const { return m_hash; }

		Bool operator==(Batch<N, Args...> const& other) const
		{
			auto const& otherObjects = other.get_objects();
			for (Size i = 0; i < N; i++)
			{
				if (m_objects[i] != otherObjects[i])
				{
					return false;
				}
			}

			return true;
		}

		static Size generate_hash(std::array<std::variant<Args...>, sizeof...(Args)> const& objects)
		{
			Size hash = N;

			// generate the hash once here, since it will never change throughout this object's lifetime
			for (auto const& element : objects) {
				std::visit([&](auto const& val) {
					hash ^= std::hash<std::decay_t<decltype(val)>>{}(val)+0x9e3779b9 + (hash << 6) + (hash >> 2);
					}, element);
			}

			return hash;
		}
	};
}

namespace std
{
	template<Minty::Size N, typename... Args>
	struct hash<Minty::Batch<N, Args...>>
	{
		size_t operator()(Minty::Batch<N, Args...> const& obj) const
		{
			return obj.get_hash();
		}
	};
}