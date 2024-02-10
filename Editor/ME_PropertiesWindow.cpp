#include "ME_PropertiesWindow.h"

#include <imgui.h>

mintye::PropertiesWindow::PropertiesWindow(Application& application)
	: EditorWindow(application)
{}

void mintye::PropertiesWindow::draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Properties"))
	{
		ImGui::End();
		return;
	}



	ImGui::End();
}

void mintye::PropertiesWindow::reset()
{
}
