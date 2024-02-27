#include "ME_ImGuiHelper.h"
#include <vector>

using namespace minty;

void ImGui::TextNode(minty::Node const& node, bool const printRoot)
{
	std::vector<std::pair<Node const*, int>> nodes;
	nodes.push_back({ &node, printRoot ? 0 : -1 });

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
