#include "ME_ConsoleWindow.h"

#include <regex>

using namespace Minty;

Mintye::ConsoleWindow::ConsoleWindow(EditorApplication& application)
	: EditorWindow(application)
	, _scrollToBottom(true)
	, _autoScroll(true)
	, showNormal(true)
	, showWarnings(true)
	, showErrors(true)
	, _filter()
	, _maxLines(16384)
	, _lines()
{
	// create editor commands
	_editorCommands.emplace("clear", [this]() {
		clear();
		});
}

ImVec4 ColorToImVec4(Console::Color const color)
{
	switch (color)
	{
	case Console::Color::Black: return ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	case Console::Color::Red: return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	case Console::Color::Green: return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	case Console::Color::Yellow: return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	case Console::Color::Blue: return ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
	case Console::Color::Magenta: return ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
	case Console::Color::Cyan: return ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	case Console::Color::White: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	case Console::Color::Gray: return ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	default: return ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}

void Mintye::ConsoleWindow::draw()
{
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Console"))
	{
		ImGui::End();
		return;
	}

	if (ImGui::Button("Clear"))
	{
		run_command("clear");
	}

	//ImGui::Checkbox("Show Messages", &showNormal);
	//ImGui::Checkbox("Show Warnings", &showWarnings);
	//ImGui::Checkbox("Show Errors", &showErrors);

	_filter.Draw("Filter (\"incl,-excl\")", 180.0f);

	ImGui::Separator();

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		_linesLock.lock();

		for (Line const& line : _lines)
		{
			// check if pass filter
			if (!_filter.PassFilter(line.text.c_str()))
			{
				continue;
			}

			// pick color
			bool hasColor = line.color != Color::White;
			if (hasColor)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ColorToImVec4(line.color));
			}

			// show text on screen
			ImGui::TextUnformatted(line.text.c_str());

			// remove color
			if (hasColor)
			{
				ImGui::PopStyleColor();
			}
		}

		_linesLock.unlock();

		if (_scrollToBottom || (_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
		{
			ImGui::SetScrollHereY(1.0f);
		}
		_scrollToBottom = false;

		ImGui::PopStyleVar();
	}

	ImGui::EndChild();

	ImGui::Separator();

	ImGui::Text("Command:");
	ImGui::SameLine();

	float windowWidth = ImGui::GetContentRegionAvail().x;
	float padding = ImGui::GetStyle().WindowPadding.x;
	float width = (windowWidth - padding * 2.0f);

	ImGui::PushItemWidth(width);

	bool reclaimFocus = false;
	static char commandBuffer[128];
	ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll;// | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	if (ImGui::InputText("##CommandLine", commandBuffer, 128, flags))
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
		ImGui::SetKeyboardFocusHere(-1);
	}

	ImGui::End();
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
	_lines.clear();
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
	_commandsLock.lock();
	_commandsQueue.push(commands);

	// if not executing, start executing
	if (!_commandsThreadRunning)
	{
		_commandsThreadRunning = true;
		std::thread thread(&ConsoleWindow::execute_commands, this);
		_commandsLock.unlock();
		if(wait) thread.join();
		else thread.detach();
	}
	else
	{
		_commandsLock.unlock();
	}
}

bool Mintye::ConsoleWindow::is_command_running() const
{
	return _commandsThreadRunning;
}

void Mintye::ConsoleWindow::log(std::string const& text, Minty::Console::Color const color)
{
	_linesLock.lock();

	if (static_cast<int>(_lines.size()) >= _maxLines)
	{
		// pop oldest off stack...
		_lines.pop_front();
	}

	_lines.push_back(Line(text, color));

	_linesLock.unlock();
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
	_maxLines = count;
}

size_t Mintye::ConsoleWindow::execute_command(std::string const& command)
{
	size_t errorCount = 0;
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
	size_t queueSize = 0;

	_commandsLock.lock();
	queueSize = _commandsQueue.size();

	while (queueSize)
	{
		// get command(s)
		std::vector<std::string> commands = _commandsQueue.front();
		_commandsQueue.pop();
		_commandsLock.unlock();

		// execute the command(s)
		size_t errorCount = 0;
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
		_commandsLock.lock();
		queueSize = _commandsQueue.size();
	}

	// all done
	_commandsThreadRunning = false;
	_commandsLock.unlock();
}

Mintye::ConsoleWindow::Line::Line(std::string const& text, Color color)
	: text(text)
	, color(color)
{}
