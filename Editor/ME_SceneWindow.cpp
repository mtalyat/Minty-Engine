#include "ME_SceneWindow.h"

mintye::SceneWindow::SceneWindow(Application& application)
	: EditorWindow(application)
{}

void mintye::SceneWindow::draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Scene", nullptr))
	{
		ImGui::End();
		return;
	}

	ImGui::End();
}

void mintye::SceneWindow::reset()
{
}
