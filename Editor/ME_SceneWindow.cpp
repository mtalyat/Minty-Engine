#include "ME_SceneWindow.h"

#include "ME_Application.h"
#include "ME_ApplicationRenderEngine.h"

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

	//ImVec2 size = ImGui::GetContentRegionAvail();

	//ApplicationRenderEngine& renderer = static_cast<ApplicationRenderEngine&>(get_application().get_runtime().get_render_engine());

	//ImGui::Image(renderer.get_scene_descriptor_set(), size);

	ImGui::End();
}

void mintye::SceneWindow::reset()
{
}
