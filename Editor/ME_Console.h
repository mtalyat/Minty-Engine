#pragma once

#include "libraries/imgui/imgui.h"
#include "libraries/imgui/imgui_impl_glfw.h"
#include "libraries/imgui/imgui_impl_vulkan.h"

#include <Minty.h>
#include <vector>

namespace mintye
{
	class Console
	{
	private:
		struct Line
		{
			std::string text;
			minty::console::Color color;

			Line(std::string const& text, minty::console::Color color)
				: text(text)
				, color(color)
			{}
		};

		bool _scrollToBottom;
		bool _autoScroll;
		bool showNormal;
		bool showWarnings;
		bool showErrors;

		ImGuiTextFilter _filter;

		int _maxSize;

		std::vector<Line> _lines;

	public:
		Console();

		void draw(char const* title);

		size_t run_command(std::string const& command);

		void log(std::string const& text, minty::console::Color const color = minty::console::Color::White);

		void log_warning(std::string const& text);

		void log_error(std::string const& text);
	};
}

