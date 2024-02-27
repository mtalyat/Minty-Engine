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

void mintye::PropertiesWindow::set_scene(minty::Scene* const scene)
{
	if (get_scene() != scene)
	{
		// new scene
		_targetMode = TargetMode::None;
		_targetEntity = NULL_ENTITY;
	}

	EditorWindow::set_scene(scene);
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
	static char popupBuffer[INPUT_SIZE];

	//		Entity

	ImGui::Text("Entity:");

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

	// these will be ignored in the general components list below,
	// since they are already drawn above
	static std::unordered_set<String> ignoreComponentNames = { "Name", "Tag", "Enabled" };

	//		Components

	ImGui::Text("Components:");

	Node entityNode = registry.serialize_entity(_targetEntity);

	static const size_t OFFSET = static_cast<size_t>(1 << 10); // 1024 component entries (should be more than enough)
	size_t i = 0;

	static float const xMargin = 2.0f;
	static float const yMargin = 2.0f;
	float const width = ImGui::GetContentRegionAvail().x - 2.0f * xMargin;

	// show each component
	for (Node& node : entityNode.get_children())
	{
		// skip already drawn components
		if (ignoreComponentNames.contains(node.get_name()))
		{
			continue;
		}

		ImGui::BeginGroupBox();

		// list components
		if (ImGui::InputNode(node, true, i))
		{
			// component changed, update it in the registy
			Component* component = registry.get_by_name(node.get_name(), _targetEntity);
			SerializationData data
			{
				.scene = get_scene(),
				.entity = _targetEntity,
			};
			Reader reader(node, &data);
			component->deserialize(reader);
		}

		// actions
		if (ImGui::Button(std::format("Remove##{}", i).c_str()))
		{
			registry.erase_by_name(node.get_name(), _targetEntity);
		}

		ImGui::EndGroupBox(Vector2(width, 0.0f), Vector2(xMargin, yMargin));

		i += OFFSET;
	}

	// entity actions
	if (ImGui::Button("Add Component", ImVec2(width, 0.0f)))
	{
		// clear buffer
		popupBuffer[0] = '\0';

		// show popup
		ImGui::OpenPopup("Add Component");
	}

	if (ImGui::BeginPopupModal("Add Component", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Enter the name of the component to add:");

		ImGui::InputText("##addComponentText", popupBuffer, INPUT_SIZE);

		if (ImGui::Button("Done"))
		{
			registry.emplace_by_name(popupBuffer, _targetEntity);

			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void mintye::PropertiesWindow::set_target(minty::Entity const entity)
{
	_targetMode = TargetMode::Entity;
	_targetEntity = entity;
}
