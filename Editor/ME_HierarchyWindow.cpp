#include "ME_HierarchyWindow.h"

#include <imgui.h>

using namespace mintye;
using namespace minty;

mintye::HierarchyWindow::HierarchyWindow(Application& application)
	: EditorWindow(application)
{}

void mintye::HierarchyWindow::draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 800), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Hierarchy"))
	{
		return;
	}



	ImGui::End();
}

void mintye::HierarchyWindow::reset()
{

}
