#include "ME_GameWindow.h"

mintye::GameWindow::GameWindow(EditorApplication& application)
	: EditorWindow(application)
{}

void mintye::GameWindow::draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Game", nullptr))
	{
		ImGui::End();
		return;
	}



	ImGui::End();
}

void mintye::GameWindow::reset()
{
}

void mintye::GameWindow::refresh()
{
}
