#include "ME_SceneWindow.h"

#include <imgui.h>

mintye::SceneWindow::SceneWindow(Application& application)
	: EditorWindow(application)
{}

void mintye::SceneWindow::draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Scene"))
	{
		ImGui::End();
		return;
	}



	ImGui::End();
}

void mintye::SceneWindow::reset()
{
}
