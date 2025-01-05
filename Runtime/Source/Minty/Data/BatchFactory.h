#pragma once

#include "Minty/Data/Batch.h"
#include <unordered_set>

namespace Minty
{
	template<typename Size N, typename ...Args>
	class BatchFactory
	{
	private:
		std::unordered_set<Batch<N, Args...>> m_batches;

		Size m_newBatchCapacity;

	public:
		BatchFactory(Size const newBatchCapacity = 0)
			: m_batches()
			, m_newBatchCapacity(newBatchCapacity)
		{
			m_batches.reserve(16);
		}

		~BatchFactory() = default;

	public:
		/// <summary>
		/// Checks if there is a Batch with the given objects.
		/// </summary>
		/// <param name="objects"></param>
		/// <returns></returns>
		Bool has_batch(std::array<std::variant<Args...>, sizeof...(Args)> const& objects)
		{
			Batch<N, Args...> temp(objects);
			return m_batches.contains(temp);
		}

		/// <summary>
		/// Gets the previous Batch, or creates a new one if the objects do not match.
		/// </summary>
		/// <param name="objects"></param>
		/// <returns></returns>
		Batch<N, Args...>& get_or_create_batch(std::array<std::variant<Args...>, sizeof...(Args)> const& objects)
		{
			Batch<N, Args...> batch(objects);
			auto found = m_batches.find(batch);

			if (found == m_batches.end())
			{
				auto temp = m_batches.emplace(batch);
				return const_cast<Batch<N, Args...>&>(*m_batches.find(batch));
			}

			return const_cast<Batch<N, Args...>&>(*found);
		}

		Size get_batch_count() const { return m_batches.size(); }

		inline std::unordered_set<Batch<N, Args...>>::iterator begin() { return m_batches.begin(); }
		inline std::unordered_set<Batch<N, Args...>>::iterator end() { return m_batches.end(); }
	};
}