#include "ME_HierarchyWindow.h"

using namespace mintye;
using namespace minty;

mintye::HierarchyWindow::HierarchyWindow(Application& application)
	: EditorWindow(application)
{}

void mintye::HierarchyWindow::draw()
{
	Scene* scene = get_scene();

	ImGui::SetNextWindowSize(ImVec2(400, 800), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Hierarchy") || !scene)
	{
		ImGui::End();
		return;
	}

	// split 50/50 into systems and entities, for now
	float windowHeight = ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y;
	float splitHeight = windowHeight * 0.5f;

	// draw systems
	SystemRegistry& systemRegistry = scene->get_system_registry();
	if (ImGui::BeginChild("HierarchySystems", ImVec2(0, splitHeight), true))
	{
		ImGui::Text(std::format("Systems ({})", systemRegistry.size()).c_str());
		ImGui::Separator();

		for (auto const system : systemRegistry)
		{
			ImGui::Text(system->get_name().c_str());
		}
	}
	ImGui::EndChild();
	
	// draw entities
	EntityRegistry* entityRegistry = &scene->get_entity_registry();
	if (ImGui::BeginChild("HierarchyEntities", ImVec2(0.0f, 0.0f), true))
	{
		ImGui::Text(std::format("Entities ({})", entityRegistry->size()).c_str());
		ImGui::Separator();

		for (auto [entity]: entityRegistry->storage<Entity>().each())
		{
			ImGui::Text(entityRegistry->get_name(entity).c_str());
		}
	}
	ImGui::EndChild();

	ImGui::End();
}

void mintye::HierarchyWindow::reset()
{

}
