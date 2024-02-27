#include "ME_GameWindow.h"

mintye::GameWindow::GameWindow(Application& application)
	: EditorWindow(application)
{}

void mintye::GameWindow::draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoBackground))
	{
		ImGui::End();
		return;
	}



	ImGui::End();
}

void mintye::GameWindow::reset()
{
}
