#pragma once

#include "Minty/Core/UUID.h"
#include "Minty/Render/BufferContainer.h"
#include <unordered_map>

namespace Minty
{
	class BufferCargo
	{
	private:
		Size m_initialCapacity;
		BufferUsage m_usage;
		std::unordered_map<UUID, std::vector<BufferContainer>> m_groups;

	public:
		BufferCargo(BufferUsage const usage)
			: m_initialCapacity(0)
			, m_usage(usage)
			, m_groups()
		{}

		BufferCargo(Size const initialCapacity, BufferUsage const usage)
			: m_initialCapacity(initialCapacity)
			, m_usage(usage)
			, m_groups()
		{}

		~BufferCargo() = default;

	public:
		UUID create_group();

		BufferContainer& get_container(UUID const group, Size const index);
	};
}