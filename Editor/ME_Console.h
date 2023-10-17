#pragma once

#include "libraries/imgui/imgui.h"
#include "libraries/imgui/imgui_impl_glfw.h"
#include "libraries/imgui/imgui_impl_vulkan.h"

#include <Minty.h>
#include <vector>
#include <queue>

namespace mintye
{
	class Console
	{
	private:
		struct Line
		{
			std::string text;
			minty::console::Color color;

			Line(std::string const& text, minty::console::Color color);
		};

		bool _scrollToBottom;
		bool _autoScroll;
		bool showNormal;
		bool showWarnings;
		bool showErrors;

		ImGuiTextFilter _filter;

		int _maxLines;
		std::deque<Line> _lines;
		std::mutex _linesLock;

		std::queue<std::vector<std::string>> _commandsQueue;
		std::mutex _commandsLock;
		bool _commandsThreadRunning;
	public:
		Console();

		void draw(char const* title);

		void run_command(std::string const& command);

		void run_commands(std::vector<std::string> const& commands);

		/// <summary>
		/// Checks if a command is running in the Console.
		/// </summary>
		/// <returns>True if there is a command being executed, otherwise false.</returns>
		bool is_command_running() const;

		void log(std::string const& text, minty::console::Color const color = minty::console::Color::White);

		void log_important(std::string const& text);

		void log_info(std::string const& text);

		void log_warning(std::string const& text);

		void log_error(std::string const& text);

		void set_max_lines(int const count);

	private:
		size_t execute_command(std::string const& command);

		void execute_commands();
	};
}

