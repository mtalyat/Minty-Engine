#include "ME_AssetsWindow.h"

#include "ME_Application.h"
#include "ME_Project.h"
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
	Project* project = get_application().get_project();

	ImGui::BeginDisabled(!project);
	if (ImGui::Button("Open Assets Folder"))
	{
		minty::Operations::open_directory(get_application().get_project()->get_assets_path());
	}
	ImGui::EndDisabled();

	ImGui::End();
}

void mintye::AssetsWindow::reset()
{
}
