#pragma once
#include "ME_EditorWindow.h"

#include <vector>
#include <queue>

namespace mintye
{
	class ConsoleWindow
		: public EditorWindow
	{
	public:
		typedef minty::Console::Color Color;

	private:
		struct Line
		{
			std::string text;
			Color color;

			Line(std::string const& text, Color color);
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
		ConsoleWindow(Application& application);

		virtual void draw() override;

		virtual void reset() override;

		void run_command(std::string const& command);

		void run_commands(std::vector<std::string> const& commands);

		/// <summary>
		/// Checks if a command is running in the Console.
		/// </summary>
		/// <returns>True if there is a command being executed, otherwise false.</returns>
		bool is_command_running() const;

		void log(std::string const& text, Color const color = Color::White);

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