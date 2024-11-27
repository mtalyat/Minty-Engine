//#include "ME_Theme.h"
//
//using namespace Minty;
//using namespace Mintye;
//
//Mintye::Theme::Theme()
//	: _colors(new Color[GuiColorID::COUNT])
//{}
//
//Mintye::Theme::~Theme()
//{
//	if (_colors) delete[] _colors;
//}
//
//Mintye::Theme::Theme(Theme const& other)
//	: _colors(new Color[GuiColorID::COUNT])
//{
//	memcpy(_colors, other._colors, sizeof(Color) * GuiColorID::COUNT);
//}
//
//Theme& Mintye::Theme::operator=(Theme const& other)
//{
//	if (this != &other)
//	{
//		// delete old
//		if (_colors) delete _colors;
//
//		// copy over new
//		_colors = new Color[GuiColorID::COUNT];
//		memcpy(_colors, other._colors, sizeof(Color) * GuiColorID::COUNT);
//	}
//
//	return *this;
//}
//
//Mintye::Theme::Theme(Theme&& other) noexcept
//	: _colors(std::move(other._colors))
//{}
//
//Theme& Mintye::Theme::operator=(Theme&& other) noexcept
//{
//	if (this != &other)
//	{
//		if (_colors) delete[] _colors;
//
//		_colors = std::move(other._colors);
//	}
//
//	return *this;
//}
//
//Color& Mintye::Theme::operator[](GuiColorID:: const col)
//{
//	return _colors[col];
//}
//
//void Mintye::Theme::copy(ImGuiStyle const& style)
//{
//	Float4 color;
//	for (int i = 0; i < GuiColorID::COUNT; i++)
//	{
//		color = style.Colors[i];
//		_colors[i] = Color(color.x, color.y, color.z, color.w);
//	}
//}
//
//void Mintye::Theme::apply(ImGuiStyle& style) const
//{
//	Color color;
//	for (int i = 0; i < GuiColorID::COUNT; i++)
//	{
//		color = _colors[i];
//		style.Colors[i] = Float4(color.rf(), color.gf(), color.bf(), color.af());
//	}
//}
