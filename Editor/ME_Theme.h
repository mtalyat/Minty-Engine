#pragma once

#include "ME_Minty.h"

namespace mintye
{
	/// <summary>
	/// Holds a color theme in the editor application.
	/// </summary>
	class Theme
		: public minty::Object
	{
	private:
		minty::Color* _colors;

	public:
		Theme();

		~Theme();

		Theme(Theme const& other);
		Theme& operator=(Theme const& other);

		Theme(Theme&& other) noexcept;
		Theme& operator=(Theme&& other) noexcept;

		/// <summary>
		/// Gets the color at the given color index.
		/// </summary>
		/// <param name="col"></param>
		/// <returns></returns>
		minty::Color& operator[](ImGuiCol_ const col);

		/// <summary>
		/// Copies the colors from the given style into this Theme.
		/// </summary>
		/// <param name="style"></param>
		void copy(ImGuiStyle const& style);

		/// <summary>
		/// Applies this theme to the given ImGuiStyle.
		/// </summary>
		/// <param name="style"></param>
		void apply(ImGuiStyle& style) const;
	};
}