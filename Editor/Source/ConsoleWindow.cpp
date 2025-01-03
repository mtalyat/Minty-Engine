#include "ConsoleWindow.h"

#include <regex>

using namespace Minty;

Mintye::ConsoleWindow::ConsoleWindow(EditorApplication& application)
	: EditorWindow(application)
	, _scrollToBottom(true)
	, _autoScroll(true)
	, showNormal(true)
	, showWarnings(true)
	, showErrors(true)
	, m_filter()
	, m_maxLines(16384)
	, m_lines()
{
	// create editor commands
	_editorCommands.emplace("clear", [this]() {
		clear();
		});
}

Color ConsoleColorToColor(Console::Color const color)
{
	switch (color)
	{
	case Console::Color::Black: return Color(0.0f, 0.0f, 0.0f, 1.0f);
	case Console::Color::Red: return Color(1.0f, 0.0f, 0.0f, 1.0f);
	case Console::Color::Green: return Color(0.0f, 1.0f, 0.0f, 1.0f);
	case Console::Color::Yellow: return Color(1.0f, 1.0f, 0.0f, 1.0f);
	case Console::Color::Blue: return Color(0.0f, 0.0f, 1.0f, 1.0f);
	case Console::Color::Magenta: return Color(1.0f, 0.0f, 1.0f, 1.0f);
	case Console::Color::Cyan: return Color(0.0f, 1.0f, 1.0f, 1.0f);
	case Console::Color::White: return Color(1.0f, 1.0f, 1.0f, 1.0f);
	case Console::Color::Gray: return Color(0.5f, 0.5f, 0.5f, 1.0f);
	default: return Color(0.0f, 0.0f, 0.0f, 1.0f);
	}
}

#include "Minty/Library/ImGUI.h"

void Mintye::ConsoleWindow::draw()
{
	GUI::set_next_window_size(Float2(520, 600), GuiCondition::FirstUseEver);
	if (!GUI::begin("Console"))
	{
		GUI::end();
		return;
	}

	if (GUI::button("Clear"))
	{
		run_command("clear");
	}

	// TODO: add filter back
	//_filter.Draw("Filter (\"incl,-excl\")", 180.0f);

	GUI::separator();

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	if (GUI::begin_child("ScrollingRegion", Float2(0, -footer_height_to_reserve), false, GuiWindowFlags::HorizontalScrollbar))
	{
		GUI::push_style_variable(GuiStyleID::ItemSpacing, Float2(4, 1));

		m_linesLock.lock();

		for (Line const& line : m_lines)
		{
			//// check if pass filter
			//if (!_filter.PassFilter(line.text.c_str()))
			//{
			//	continue;
			//}

			// pick color
			bool hasColor = line.color != Color::White;
			if (hasColor)
			{
				GUI::push_style_color(GuiColorID::Text, ConsoleColorToColor(line.color));
			}

			// show text on screen
			ImGui::TextUnformatted(line.text.c_str());

			// remove color
			if (hasColor)
			{
				GUI::pop_style_color();
			}
		}

		m_linesLock.unlock();

		if (_scrollToBottom || (_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
		{
			ImGui::SetScrollHereY(1.0f);
		}
		_scrollToBottom = false;

		ImGui::PopStyleVar();
	}

	ImGui::EndChild();

	GUI::separator();

	GUI::text("Command:");
	GUI::same_line();

	float windowWidth = GUI::get_content_region_size().x;
	float padding = ImGui::GetStyle().WindowPadding.x;
	float width = (windowWidth - padding * 2.0f);

	ImGui::PushItemWidth(width);

	bool reclaimFocus = false;
	static char commandBuffer[128];
	GuiInputTextFlags flags = GuiInputTextFlags::EnterReturnsTrue | GuiInputTextFlags::EscapeClearsAll;// | GuiInputTextFlags::CallbackCompletion | GuiInputTextFlags::CallbackHistory;
	if (GUI::input_text("##CommandLine", commandBuffer, 128, flags))
	{
		if (commandBuffer[0])
		{
			run_command(commandBuffer);
			commandBuffer[0] = '\0';
		}
		reclaimFocus = true;
	}

	ImGui::PopItemWidth();

	// focus on prev window
	ImGui::SetItemDefaultFocus();
	if (reclaimFocus)
	{
		GUI::set_keyboard_focus_here(-1);
	}

	GUI::end();
}

void Mintye::ConsoleWindow::reset()
{
	clear();
}

void Mintye::ConsoleWindow::refresh()
{
}

void Mintye::ConsoleWindow::clear()
{
	m_lines.clear();
}

void Mintye::ConsoleWindow::run_command(std::string const& command, bool const wait)
{
	// check if editor command
	auto found = _editorCommands.find(command);

	if (found != _editorCommands.end())
	{
		found->second();
		return;
	}
	
	// run command on system instead
	run_commands({ command }, wait);
}

void Mintye::ConsoleWindow::run_commands(std::vector<std::string> const& commands, bool const wait)
{
	// lock and add to queue
	m_commandsLock.lock();
	m_commandsQueue.push(commands);

	// if not executing, start executing
	if (!m_commandsThreadRunning)
	{
		m_commandsThreadRunning = true;
		std::thread thread(&ConsoleWindow::execute_commands, this);
		m_commandsLock.unlock();
		if(wait) thread.join();
		else thread.detach();
	}
	else
	{
		m_commandsLock.unlock();
	}
}

bool Mintye::ConsoleWindow::is_command_running() const
{
	return m_commandsThreadRunning;
}

void Mintye::ConsoleWindow::log(std::string const& text, Minty::Console::Color const color)
{
	m_linesLock.lock();

	if (static_cast<int>(m_lines.size()) >= m_maxLines)
	{
		// pop oldest off stack...
		m_lines.pop_front();
	}

	m_lines.push_back(Line(text, color));

	m_linesLock.unlock();
}

void Mintye::ConsoleWindow::log_important(std::string const& text)
{
	log(text, Color::Cyan);
}

void Mintye::ConsoleWindow::log_info(std::string const& text)
{
	log(text, Color::Gray);
}

void Mintye::ConsoleWindow::log_warning(std::string const& text)
{
	log(text, Color::Yellow);
}

void Mintye::ConsoleWindow::log_error(std::string const& text)
{
	log(text, Color::Red);
}

void Mintye::ConsoleWindow::set_max_lines(int const count)
{
	m_maxLines = count;
}

Size Mintye::ConsoleWindow::execute_command(std::string const& command)
{
	Size errorCount = 0;
	std::array<char, 4096> buffer;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	std::regex regex("(\033\\[)[0-?]*[ -\\/]*[@-~]"); // regex to remove escape codes
	std::string result;
	bool changeColor = true;
	Color color = Color::White;
	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {

		// get text
		result = buffer.data();

		// remove escape codes
		result = std::regex_replace(result , regex, "");

		if (changeColor)
		{
			// check for [type]
			if (/*result.starts_with("[errr]") || */result.find("err") != std::string::npos)
			{
				color = Color::Red;
				errorCount++;
			}
			else if (/*result.starts_with("[warn]") || */result.find("warn") != std::string::npos)
			{
				color = Color::Yellow;
			}
			else if (/*result.starts_with("[info]") || */result.find("info") != std::string::npos)
			{
				color = Color::Gray;
			}
			else if (result.starts_with("[test]"))
			{
				color = Color::Blue;
			}
			else if (result.starts_with("[todo]"))
			{
				color = Color::Magenta;
			}
			else
			{
				// normal
				color = Color::White;
			}
		}

		log(result, color);

		// if newline, it was the end of the inputted line, so update the color for the next line
		changeColor = result.ends_with('\n');
	}

	return errorCount;
}

void Mintye::ConsoleWindow::execute_commands()
{
	Size queueSize = 0;

	m_commandsLock.lock();
	queueSize = m_commandsQueue.size();

	while (queueSize)
	{
		// get command(s)
		std::vector<std::string> commands = m_commandsQueue.front();
		m_commandsQueue.pop();
		m_commandsLock.unlock();

		// execute the command(s)
		Size errorCount = 0;
		for (std::string const& command : commands)
		{
			// log command
			log(command);

			// run command
			errorCount = execute_command(command);

			// if any errors, do not call the other dependent commands
			if (errorCount)
			{
				break;
			}
		}

		// get new count
		m_commandsLock.lock();
		queueSize = m_commandsQueue.size();
	}

	// all done
	m_commandsThreadRunning = false;
	m_commandsLock.unlock();
}

Mintye::ConsoleWindow::Line::Line(std::string const& text, Color color)
	: text(text)
	, color(color)
{}
