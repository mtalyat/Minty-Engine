#include "ME_PropertiesWindow.h"

#include "ME_Application.h"
#include "ME_ConsoleWindow.h"
#include "ME_ImGuiHelper.h"

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

	static const size_t INPUT_SIZE = 256;
	static char inputBuffer[INPUT_SIZE];

	//		Entity

	ImGui::Text("Entity");

	// name
	String text = registry.get_name(_targetEntity);
	size_t size = min(INPUT_SIZE, text.size() + 1);
	memcpy(inputBuffer, text.c_str(), size);
	inputBuffer[size - 1] = '\0';

	if (ImGui::InputText("Name", inputBuffer, INPUT_SIZE))
	{
		registry.set_name(_targetEntity, inputBuffer);
	}

	// id
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), to_string(registry.get_id(_targetEntity)).c_str());

	// enabled
	bool enabled = registry.get_enabled(_targetEntity);
	if (ImGui::Checkbox("Enabled", &enabled))
	{
		registry.set_enabled(_targetEntity, enabled);
	}

	// tag
	text = registry.get_tag(_targetEntity);
	size = min(INPUT_SIZE, text.size() + 1);
	memcpy(inputBuffer, text.c_str(), size);
	inputBuffer[size - 1] = '\0';

	if (ImGui::InputText("Tag", inputBuffer, INPUT_SIZE))
	{
		registry.set_tag(_targetEntity, inputBuffer);
	}

	ImGui::Separator();

	//		Components

	ImGui::Text("Components");

	Node entityNode = registry.serialize_entity(_targetEntity);

	ImGui::TextNode(entityNode);
}

void mintye::PropertiesWindow::set_target(minty::Entity const entity)
{
	_targetMode = TargetMode::Entity;
	_targetEntity = entity;
}
