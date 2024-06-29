#pragma once
#include "ME_Minty.h"

namespace ImGui
{
	void TextNode(Minty::Node const& rootNode, bool const printRoot = true);

	bool InputNode(Minty::Node& rootNode, bool const printRoot = true, size_t const offset = 0);

	void BeginGroupBox();

	void EndGroupBox(Minty::Vector2 const size = Minty::Vector2(), Minty::Vector2 const margin = Minty::Vector2(), Minty::Color const borderColor = Minty::Color(63, 63, 63, 255));

	bool InputTextExpand(char const* label, char* buf, size_t buf_size, float const widthPercent = 1.0f, ImGuiInputTextFlags flags = 0);

	bool InputTextExpandOffset(char const* label, char* buf, size_t buf_size, float const offsetLeft, float const offsetRight, float const widthPercent = 1.0f, ImGuiInputTextFlags flags = 0);
	
	bool InputTextMultilineExpand(char const* label, char* buf, size_t buf_size, float const widthPercent = 1.0f, ImGuiInputTextFlags flags = 0);

	bool InputTextMultilineExpandOffset(char const* label, char* buf, size_t buf_size, float const offsetLeft, float const offsetRight, float const widthPercent = 1.0f, ImGuiInputTextFlags flags = 0);

	bool ButtonAlignedLeft(char const* label, ImVec2 const& size);
}