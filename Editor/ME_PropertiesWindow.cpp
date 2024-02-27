#include "ME_PropertiesWindow.h"

#include "ME_Application.h"
#include "ME_ConsoleWindow.h"

using namespace minty;
using namespace mintye;

mintye::PropertiesWindow::PropertiesWindow(Application& application)
	: EditorWindow(application)
	, _targetMode(TargetMode::None)
	, _targetEntity(NULL_ENTITY)
{}

void mintye::PropertiesWindow::draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Properties"))
	{
		ImGui::End();
		return;
	}

	// draw based on target mode
	switch (_targetMode)
	{
	case TargetMode::None:
		draw_none();
		break;
	case TargetMode::Entity:
		draw_entity();
		break;
	default:
		ImGui::Text(std::format("Error: Unknown target mode: {}", static_cast<int>(_targetMode)).c_str());
		break;
	}

	ImGui::End();
}

void mintye::PropertiesWindow::reset()
{
}

void mintye::PropertiesWindow::draw_none() const
{
	ImGui::Text("--");
}

void mintye::PropertiesWindow::draw_entity() const
{
	Scene* scene = get_scene();

	// no scene?
	if (!scene) return;

	EntityRegistry& registry = get_scene()->get_entity_registry();

	// name
	ImGui::Text(registry.get_name(_targetEntity).c_str());
}

void mintye::PropertiesWindow::set_target(minty::Entity const entity)
{
	_targetMode = TargetMode::Entity;
	_targetEntity = entity;
}
