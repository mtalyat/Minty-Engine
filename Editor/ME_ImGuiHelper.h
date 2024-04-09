#pragma once
#include "ME_Minty.h"

namespace ImGui
{
	void TextNode(minty::Node const& rootNode, bool const printRoot = true);

	bool InputNode(minty::Node& rootNode, bool const printRoot = true, size_t const offset = 0);

	void BeginGroupBox();

	void EndGroupBox(minty::Vector2 const size = minty::Vector2(), minty::Vector2 const margin = minty::Vector2(), minty::Color const borderColor = minty::Color(63, 63, 63, 255));

	bool InputTextExpand(char const* label, char* buf, size_t buf_size, float const widthPercent = 1.0f, ImGuiInputTextFlags flags = 0);

	bool InputTextExpandOffset(char const* label, char* buf, size_t buf_size, float const offsetLeft, float const offsetRight, float const widthPercent = 1.0f, ImGuiInputTextFlags flags = 0);
}