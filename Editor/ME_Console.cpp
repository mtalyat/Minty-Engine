#include "ME_Console.h"

using namespace minty;

mintye::Console::Console()
	: _scrollToBottom(true)
	, _autoScroll(true)
	, showNormal(true)
	, showWarnings(true)
	, showErrors(true)
	, _filter()
	, _maxSize(16384)
	, _lines()
{
	_lines.reserve(_maxSize);
}

void mintye::Console::draw(char const* title)
{
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Console"))
	{
		ImGui::End();
		return;
	}

	ImGui::Checkbox("Show Messages", &showNormal);
	ImGui::Checkbox("Show Warnings", &showWarnings);
	ImGui::Checkbox("Show Errors", &showErrors);

	_filter.Draw("Filter (\"incl,-excl\")", 180.0f);

	ImGui::Separator();

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		for (Line const& line : _lines)
		{
			// check if pass filter
			if (!_filter.PassFilter(line.text.c_str()))
			{
				continue;
			}

			// pick color
			bool hasColor = true;
			ImVec4 color;
			switch (line.color)
			{
			case console::Color::Red:
				color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
				break;
			case console::Color::Yellow:
				color = ImVec4(1.0f, 0.1f, 0.4f, 1.0f);
				break;
			case console::Color::Gray:
				color = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
				break;
			default:
				hasColor = false;
				break;
			}
			
			// add color
			if (hasColor)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, color);
			}

			// show text on screen
			ImGui::TextUnformatted(line.text.c_str());

			// remove color
			if (hasColor)
			{
				ImGui::PopStyleColor();
			}
		}

		if (_scrollToBottom || (_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
		{
			ImGui::SetScrollHereY(1.0f);
		}
		_scrollToBottom = false;

		ImGui::PopStyleVar();
	}

	ImGui::EndChild();

	ImGui::Separator();

	bool reclaimFocus = false;
	static char commandBuffer[128];
	ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll;// | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	if (ImGui::InputText("Command", commandBuffer, 128, flags))
	{
		if (commandBuffer[0])
		{
			log(commandBuffer);
			commandBuffer[0] = '\0';
		}
		reclaimFocus = true;
	}

	// focus on prev window
	ImGui::SetItemDefaultFocus();
	if (reclaimFocus)
	{
		ImGui::SetKeyboardFocusHere(-1);
	}

	ImGui::End();
}

size_t mintye::Console::run_command(std::string const& command)
{
	size_t errorCount = 0;
	//minty::console::log(command);
	std::array<char, 4096> buffer;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	std::string result;
	bool changeColor = true;
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		
		result = buffer.data();

		if (changeColor)
		{
			if (result.find("error") != std::string::npos)
			{
				//minty::console::error(result);
				log(result, minty::console::Color::Red);
				errorCount++;
			}
			else if (result.find("warning") != std::string::npos)
			{
				//minty::console::warn(result);
				log(result, minty::console::Color::Yellow);
			}
			else
			{
				//minty::console::info(result);
				log(result, minty::console::Color::Gray);
			}
		}

		// if newline, it was the end of the inputted line, so update the color for the next line
		changeColor = result.ends_with('\n');
	}

	return errorCount;
}

void mintye::Console::log(std::string const& text, minty::console::Color const color)
{
	if (static_cast<int>(_lines.size()) >= _maxSize)
	{
		return;
	}

	_lines.push_back(Line(text, color));
}

void mintye::Console::log_warning(std::string const& text)
{
	log(text, console::Color::Yellow);
}

void mintye::Console::log_error(std::string const& text)
{
	log(text, console::Color::Red);
}
