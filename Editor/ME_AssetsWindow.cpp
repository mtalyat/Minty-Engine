#include "ME_AssetsWindow.h"

#include <imgui.h>

mintye::AssetsWindow::AssetsWindow(Application& application)
	: EditorWindow(application)
{}

void mintye::AssetsWindow::draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Assets"))
	{
		ImGui::End();
		return;
	}

	// TODO: create explorer in window

	// for now, button to open the folder
	if (ImGui::Button("Open Assets Folder"))
	{

	}

	ImGui::End();
}

void mintye::AssetsWindow::reset()
{
}
