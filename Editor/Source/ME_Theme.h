//#pragma once
//
//#include "ME_Minty.h"
//
//namespace Mintye
//{
//	/// <summary>
//	/// Holds a color theme in the editor application.
//	/// </summary>
//	class Theme
//		: public Minty::Object
//	{
//	private:
//		Minty::Color* _colors;
//
//	public:
//		Theme();
//
//		~Theme();
//
//		Theme(Theme const& other);
//		Theme& operator=(Theme const& other);
//
//		Theme(Theme&& other) noexcept;
//		Theme& operator=(Theme&& other) noexcept;
//
//		/// <summary>
//		/// Gets the color at the given color index.
//		/// </summary>
//		/// <param name="col"></param>
//		/// <returns></returns>
//		Minty::Color& operator[](GuiColorID:: const col);
//
//		/// <summary>
//		/// Copies the colors from the given style into this Theme.
//		/// </summary>
//		/// <param name="style"></param>
//		void copy(ImGuiStyle const& style);
//
//		/// <summary>
//		/// Applies this theme to the given ImGuiStyle.
//		/// </summary>
//		/// <param name="style"></param>
//		void apply(ImGuiStyle& style) const;
//	};
//}