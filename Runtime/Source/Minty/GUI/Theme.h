#pragma once

#include "Minty/Core/Base.h"
#include "Minty/Core/Color.h"
#include "Minty/GUI/Enums.h"
#include <array>

namespace Minty
{
	class Theme
	{
	private:
		std::array<Color, 55> m_colors;

	public:
		Theme()
			: m_colors()
		{}

		~Theme() = default;

		Theme(Theme const& other)
			: m_colors()
		{
			memcpy(m_colors.data(), other.m_colors.data(), sizeof(Color) * m_colors.size());
		}

		Theme& operator=(Theme const& other)
		{
			if (this != &other)
			{
				memcpy(m_colors.data(), other.m_colors.data(), sizeof(Color) * m_colors.size());
			}
			return *this;
		}

		Theme(Theme&& other) noexcept
			: m_colors(std::move(other.m_colors))
		{}

		Theme& operator=(Theme&& other) noexcept
		{
			if (this != &other)
			{
				m_colors = std::move(other.m_colors);
			}
			return *this;
		}

		Color& operator[](GuiColorID const id)
		{
			return m_colors[static_cast<Size>(id)];
		}
	};
}