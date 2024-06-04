#include "ME_GameWindow.h"

Mintye::GameWindow::GameWindow(EditorApplication& application)
	: EditorWindow(application)
{}

void Mintye::GameWindow::draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Game", nullptr))
	{
		ImGui::End();
		return;
	}



	ImGui::End();
}

void Mintye::GameWindow::reset()
{
}

void Mintye::GameWindow::refresh()
{
}
