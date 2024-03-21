#include "ME_Theme.h"

using namespace minty;
using namespace mintye;

mintye::Theme::Theme()
	: _colors(new Color[ImGuiCol_COUNT])
{}

mintye::Theme::~Theme()
{
	if (_colors) delete[] _colors;
}

mintye::Theme::Theme(Theme const& other)
	: _colors(new Color[ImGuiCol_COUNT])
{
	memcpy(_colors, other._colors, sizeof(Color) * ImGuiCol_COUNT);
}

Theme& mintye::Theme::operator=(Theme const& other)
{
	if (this != &other)
	{
		// delete old
		if (_colors) delete _colors;

		// copy over new
		_colors = new Color[ImGuiCol_COUNT];
		memcpy(_colors, other._colors, sizeof(Color) * ImGuiCol_COUNT);
	}

	return *this;
}

mintye::Theme::Theme(Theme&& other) noexcept
	: _colors(std::move(other._colors))
{}

Theme& mintye::Theme::operator=(Theme&& other) noexcept
{
	if (this != &other)
	{
		if (_colors) delete[] _colors;

		_colors = std::move(other._colors);
	}

	return *this;
}

Color& mintye::Theme::operator[](ImGuiCol_ const col)
{
	return _colors[col];
}

void mintye::Theme::copy(ImGuiStyle const& style)
{
	ImVec4 color;
	for (int i = 0; i < ImGuiCol_COUNT; i++)
	{
		color = style.Colors[i];
		_colors[i] = Color(color.x, color.y, color.z, color.w);
	}
}

void mintye::Theme::apply(ImGuiStyle& style) const
{
	Color color;
	for (int i = 0; i < ImGuiCol_COUNT; i++)
	{
		color = _colors[i];
		style.Colors[i] = ImVec4(color.rf(), color.gf(), color.bf(), color.af());
	}
}
