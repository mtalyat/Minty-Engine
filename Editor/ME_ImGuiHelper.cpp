#include "ME_ImGuiHelper.h"

#include "M_Math.h"
#include <vector>

using namespace minty;

void ImGui::TextNode(minty::Node const& rootNode, bool const printRoot)
{
	std::vector<std::pair<Node const*, int>> nodes;
	nodes.push_back({ &rootNode, printRoot ? 0 : -1 });

	while (!nodes.empty())
	{
		// get next node
		std::pair<Node const*, int> pair = nodes.back();
		nodes.pop_back();
		Node const* node = pair.first;

		// print, if indent is >= 0
		if (pair.second >= 0)
		{
			String string = node->get_node_string();
			ImGui::Text(String(static_cast<size_t>(pair.second << 1), ' ').append(string.empty() ? "_" : string).c_str());
		}

		// add children, in reverse so first children get dealth with first
		std::vector<Node> const& children = node->get_children();
		for (auto it = children.crbegin(); it != children.crend(); it++)
		{
			nodes.push_back({ &(*it), pair.second + 1 });
		}
	}
}

bool ImGui::InputNode(minty::Node& rootNode, bool const printRoot, size_t const offset)
{
	static int const BUFFER_SIZE = 256;
	static char buffer[BUFFER_SIZE] = "";

	std::vector<std::pair<Node*, int>> nodes;
	nodes.push_back({ &rootNode, printRoot ? 0 : -1 });

	bool modified = false;

	size_t i = offset;

	while (!nodes.empty())
	{
		// get next node
		std::pair<Node*, int> pair = nodes.back();
		nodes.pop_back();

		// ignore < 0 indents
		if (pair.second < 0) continue;

		String indentString = String(static_cast<size_t>(pair.second << 1), ' ');

		// print, if indent is >= 0
		if (pair.second == 0)
		{
			// display text on root
			String string = pair.first->get_node_string();
			ImGui::Text(indentString.append(string.empty() ? "_" : string).c_str());
		}
		else
		{
			// input text

			size_t size = min(BUFFER_SIZE, pair.first->get_data().size() + 1);
			memcpy(buffer, pair.first->get_data().c_str(), size);
			buffer[size - 1] = '\0';

			ImGui::Text(std::format("{}{}: ", indentString, pair.first->get_name()).c_str());
			ImGui::SameLine();
			if (ImGui::InputText(std::format("##{}", i).c_str(), buffer, BUFFER_SIZE))
			{
				pair.first->set_data(buffer);
				modified = true;
			}
		}

		// add children, in reverse so first children get dealt with first
		std::vector<Node>& children = pair.first->get_children();
		for (auto it = children.rbegin(); it != children.rend(); it++)
		{
			nodes.push_back({ &(*it), pair.second + 1 });
		}

		i++;
	}

	return modified;
}

void ImGui::BeginGroupBox()
{
	ImGui::BeginGroup();
}

void ImGui::EndGroupBox(minty::Vector2 const size, minty::Vector2 const margin, minty::Color const borderColor)
{
	ImGui::EndGroup();

	ImVec2 min = ImGui::GetItemRectMin();
	ImVec2 max = ImGui::GetItemRectMax();

	// override size if size != 0
	if (size.x > 0.0f)
	{
		max.x = min.x + size.x;
	}
	if (size.y > 0.0f)
	{
		max.y = min.y + size.y;
	}

	// add margin
	min.x -= margin.x;
	max.x += margin.x;
	min.y -= margin.y;
	max.y += margin.y;

	ImGui::GetWindowDrawList()->AddRect(min, max, IM_COL32(borderColor.r, borderColor.g, borderColor.b, borderColor.a));
}

bool ImGui::InputTextExpand(char const* label, char* buf, size_t buf_size, float widthPercent, ImGuiInputTextFlags flags)
{
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * widthPercent);
	return ImGui::InputText(label, buf, buf_size, flags);
}

bool ImGui::InputTextExpandOffset(char const* label, char* buf, size_t buf_size, float const offsetLeft, float const offsetRight, float const widthPercent, ImGuiInputTextFlags flags)
{
	float avail = ImGui::GetContentRegionAvail().x;
	ImGui::SetNextItemWidth((avail - offsetLeft - offsetRight) * widthPercent + offsetLeft);
	return ImGui::InputText(label, buf, buf_size, flags);
}
