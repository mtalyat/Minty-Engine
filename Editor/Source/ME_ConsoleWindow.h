#pragma once
#include "ME_EditorWindow.h"

#include <vector>
#include <queue>

namespace Mintye
{
	class ConsoleWindow
		: public EditorWindow
	{
	public:
		typedef Minty::Console::Color Color;

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

		// commands that are handled by the editor
		std::unordered_map <Minty::String, std::function<void()>> _editorCommands;

		Minty::String _filter;

		int _maxLines;
		std::deque<Line> _lines;
		std::mutex _linesLock;

		std::queue<std::vector<std::string>> _commandsQueue;
		std::mutex _commandsLock;
		bool _commandsThreadRunning;
	public:
		ConsoleWindow(EditorApplication& application);

		void draw() override;

		void reset() override;

		void refresh() override;

		/// <summary>
		/// Clears all text from the console.
		/// </summary>
		void clear();

		void run_command(std::string const& command, bool const wait = false);

		void run_commands(std::vector<std::string> const& commands, bool const wait = false);

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
		Minty::Size execute_command(std::string const& command);

		void execute_commands();
	};
}