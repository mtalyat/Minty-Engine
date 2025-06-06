#include "ME_PropertiesWindow.h"

#include "ME_EditorApplication.h"
#include "ME_ConsoleWindow.h"
#include "ME_HierarchyWindow.h"
#include "ME_ImGuiHelper.h"
#include "ME_Project.h"

using namespace Minty;
using namespace Mintye;

Mintye::PropertiesWindow::PropertiesWindow(EditorApplication& application)
	: EditorWindow(application)
	, _targetMode(TargetMode::None)
	, _targetIsBuiltIn(false)
	, _targetId(INVALID_UUID)
	, _targetEntity(NULL_ENTITY)
	, _targetPath()
	, _texts()
{}

void Mintye::PropertiesWindow::draw()
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
	case TargetMode::Asset:
		draw_asset();
		break;
	default:
		ImGui::Text(std::format("Error: Unknown target mode: {}", static_cast<int>(_targetMode)).c_str());
		break;
	}

	ImGui::End();
}

void Mintye::PropertiesWindow::reset()
{
}

void Mintye::PropertiesWindow::refresh()
{
	Path path = _targetPath;
	set_target(path);
}

void Mintye::PropertiesWindow::set_scene(Minty::Ref<Minty::Scene> const scene)
{
	if (get_scene() != scene)
	{
		// new scene
		_targetMode = TargetMode::None;
		_targetEntity = NULL_ENTITY;
	}

	EditorWindow::set_scene(scene);
}

bool Mintye::PropertiesWindow::input_node(Minty::Node& rootNode, bool const printRoot, uint32_t const offset)
{
	static int const BUFFER_SIZE = 256;
	static char buffer[BUFFER_SIZE] = "";

	std::vector<std::pair<Node*, int>> nodes;
	nodes.push_back({ &rootNode, printRoot ? 0 : -1 });

	bool modified = false;

	size_t i = offset;

	while (!nodes.empty())
	{
		// get next node
		std::pair<Node*, int> pair = nodes.back();
		nodes.pop_back();

		// ignore < 0 indents
		if (pair.second < 0) continue;

		String indentString = String(static_cast<size_t>(pair.second << 1), ' ');

		// print, if indent is >= 0
		if (pair.second == 0)
		{
			// display text on root
			String string = pair.first->get_node_string();
			ImGui::Text(indentString.append(string.empty() ? "_" : string).c_str());
		}
		else
		{
			// input text

			size_t size = std::min(static_cast<size_t>(BUFFER_SIZE), pair.first->get_data().size() + 1);
			memcpy(buffer, pair.first->get_data().c_str(), size);
			buffer[size - 1] = '\0';

			String text = std::format("{}{}: ", indentString, pair.first->get_name());
			float leftTextWidth = ImGui::CalcTextSize(text.c_str()).x;
			ImGui::Text(text.c_str());
			ImGui::SameLine();
			// if an ID, get the name of the object with the ID
			UUID id(INVALID_UUID);
			String idName = "";
			if (Parse::try_uuid(buffer, id))
			{
				idName = get_application().get_name(id);
			}
			float rightTextWidth = ImGui::CalcTextSize(idName.c_str()).x;
			// add 10 so it is within the group box
			if (ImGui::InputTextExpandOffset(std::format("{}##{}", idName, i).c_str(), buffer, BUFFER_SIZE, leftTextWidth, rightTextWidth + 10.0f))
			{
				pair.first->set_data(buffer);
				modified = true;
			}
		}

		// add children, in reverse so first children get dealt with first
		std::vector<Node>& children = pair.first->get_children();
		for (auto it = children.rbegin(); it != children.rend(); it++)
		{
			nodes.push_back({ &(*it), pair.second + 1 });
		}

		i++;
	}

	return modified;
}

void Mintye::PropertiesWindow::draw_none()
{
	ImGui::Text("--");
}

void Mintye::PropertiesWindow::draw_entity()
{
	Ref<Scene> scene = get_scene();

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
	size_t size = std::min(INPUT_SIZE, text.size() + 1);
	memcpy(inputBuffer, text.c_str(), size);
	inputBuffer[size - 1] = '\0';

	if (ImGui::InputText("Name", inputBuffer, INPUT_SIZE))
	{
		registry.set_name(_targetEntity, inputBuffer);
	}

	// id
	String idText = to_string(_targetId);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.65f, 1.0f));
	if (ImGui::Button(idText.c_str()))
	{
		// copy ID on click
		ImGui::SetClipboardText(idText.c_str());
	}
	ImGui::PopStyleColor();

	// enabled
	bool enabled = registry.get_enabled(_targetEntity);
	if (ImGui::Checkbox("Enabled", &enabled))
	{
		registry.set_enabled(_targetEntity, enabled);
	}

	ImGui::SameLine();

	// renderable (visible)
	bool visible = registry.get_renderable(_targetEntity);
	if (ImGui::Checkbox("Visible", &visible))
	{
		registry.set_renderable(_targetEntity, visible);
	}

	// tag
	text = registry.get_tag(_targetEntity);
	size = std::min(INPUT_SIZE, text.size() + 1);
	memcpy(inputBuffer, text.c_str(), size);
	inputBuffer[size - 1] = '\0';

	if (ImGui::InputText("Tag", inputBuffer, INPUT_SIZE))
	{
		registry.set_tag(_targetEntity, inputBuffer);
	}

	ImGui::Separator();

	// these will be ignored in the general components list below,
	// since they are already drawn above
	static std::unordered_set<String> ignoreComponentNames = { "Name", "Tag", "Enabled", "Renderable"};

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

		// if the node is a script component, list all of the scripts instead
		if (node.get_name() == "Script")
		{
			ScriptEngine& scriptEngine = ScriptEngine::instance();

			// treat each script as a component
			for (auto childNode : node.get_children())
			{
				// replace ID with script name
				childNode.set_name(scriptEngine.get_name_from_script_id(childNode.to_uuid()));
				childNode.set_data("");

				// treat as normal component
				draw_component(childNode, i, scene, registry);

				i += OFFSET;
			}
		}
		else
		{
			// just use the straight up component
			draw_component(node, i, scene, registry);
		}

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

		ImGui::SetKeyboardFocusHere();
		ImGui::InputText("##addComponentText", popupBuffer, INPUT_SIZE);

		if (ImGui::Button("Done") || ImGui::IsKeyPressed(ImGuiKey_Enter))
		{
			// check for Script asset
			// ensure it is loaded
			Project* project = get_project();
			AssetEngine& assets = AssetEngine::instance();

			Path assetPath = project->find_asset(popupBuffer, AssetType::Script);
			
			// register it with scene if needed
			if (!assetPath.empty())
			{
				if (!scene->is_registered(assetPath))
				{
					scene->register_asset(assetPath);
				}
			}

			// place onto the object
			if (!registry.emplace_by_name(popupBuffer, _targetEntity))
			{
				get_application().log_error(std::format("Failed to add component \"{}\" to Entity \"{}\".", popupBuffer, registry.get_name(_targetEntity)));
			}

			memset(popupBuffer, 0, IM_ARRAYSIZE(popupBuffer));
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel") || ImGui::IsKeyPressed(ImGuiKey_Escape))
		{
			memset(popupBuffer, 0, IM_ARRAYSIZE(popupBuffer));
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void Mintye::PropertiesWindow::draw_component(Minty::Node& node, size_t const i, Minty::Ref<Minty::Scene> const scene, Minty::EntityRegistry& registry)
{
	static float const xMargin = 2.0f;
	static float const yMargin = 2.0f;
	float const width = ImGui::GetContentRegionAvail().x - 2.0f * xMargin;

	static std::unordered_set<String> dirtyableComponentNames = { "Transform", "UITransform", "Canvas" };

	ImGui::BeginGroupBox();

	// list components
	if (input_node(node, true, static_cast<uint32_t>(i)))
	{
		// component changed, update it in the registy
		Component* component = registry.get_by_name(node.get_name(), _targetEntity);
		SerializationData data
		{
			.scene = get_scene().get(),
			.entity = _targetEntity,
		};
		Reader reader(node, &data);
		component->deserialize(reader);

		// if certain component was updated, dirty the entity, update, continue
		if (dirtyableComponentNames.contains(node.get_name()))
		{
			registry.dirty(_targetEntity);
			scene->finalize();
		}
	}

	// additonal actions for specific components
	if (node.get_name() == "Relationship")
	{
		// only if relationship has a parent
		RelationshipComponent* relationship = static_cast<RelationshipComponent*>(registry.get_by_name(node.get_name(), _targetEntity));

		if (relationship && relationship->parent != NULL_ENTITY)
		{
			bool canMoveUp = relationship->prev != NULL_ENTITY;
			bool canMoveDown = relationship->next != NULL_ENTITY;

			if (!canMoveUp) ImGui::BeginDisabled();

			if (ImGui::Button(std::format("Move Up##{}", i).c_str()))
			{
				registry.move_to_previous(_targetEntity);
				HierarchyWindow* hierarchy = get_application().find_editor_window<HierarchyWindow>("Hierarchy");
				if (hierarchy) hierarchy->refresh();
			}

			if (!canMoveUp) ImGui::EndDisabled();

			ImGui::SameLine();

			if (!canMoveDown) ImGui::BeginDisabled();

			if (ImGui::Button(std::format("Move Down##{}", i).c_str()))
			{
				registry.move_to_next(_targetEntity);
				HierarchyWindow* hierarchy = get_application().find_editor_window<HierarchyWindow>("Hierarchy");
				if (hierarchy) hierarchy->refresh();
			}

			if (!canMoveDown) ImGui::EndDisabled();

			ImGui::SameLine();

			if (!canMoveUp) ImGui::BeginDisabled();

			if (ImGui::Button(std::format("Move to First##{}", i).c_str()))
			{
				registry.move_to_first(_targetEntity);
				HierarchyWindow* hierarchy = get_application().find_editor_window<HierarchyWindow>("Hierarchy");
				if (hierarchy) hierarchy->refresh();
			}

			if (!canMoveUp) ImGui::EndDisabled();

			ImGui::SameLine();

			if (!canMoveDown) ImGui::BeginDisabled();

			if (ImGui::Button(std::format("Move to Last##{}", i).c_str()))
			{
				registry.move_to_last(_targetEntity);
				HierarchyWindow* hierarchy = get_application().find_editor_window<HierarchyWindow>("Hierarchy");
				if (hierarchy) hierarchy->refresh();
			}

			if (!canMoveDown) ImGui::EndDisabled();
		}
	}

	// actions
	if (ImGui::Button(std::format("Remove##{}", i).c_str()))
	{
		registry.erase_by_name(node.get_name(), _targetEntity);
	}

	ImGui::EndGroupBox(Vector2(width, 0.0f), Vector2(xMargin, yMargin));
}

void Mintye::PropertiesWindow::draw_asset()
{
	// TODO: do differently for specific asset types

	// print name
	ImGui::Text(_targetPath.stem().string().c_str());

	ImGui::Separator();

	// button ID to copy it
	if (ImGui::Button(to_string(_targetId).c_str()))
	{
		ImGui::SetClipboardText(to_string(_targetId).c_str());
	}

	if (!_targetIsBuiltIn)
	{
		ImGui::Separator();

		// button to refresh the contents
		if (ImGui::Button("Refresh"))
		{
			get_application().refresh();
			return;
		}

		// gap
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.0f);

		// button to open the file
		if (ImGui::Button("Open File"))
		{
			Operations::open(get_project()->get_base_path() / _targetPath);
		}

		ImGui::SameLine();

		// button to open the meta file
		if (ImGui::Button("Open Meta"))
		{
			Operations::open(get_project()->get_base_path() / Asset::get_meta_path(_targetPath));
		}

		ImGui::SameLine();

		// button to open the directory
		if (ImGui::Button("Open Folder"))
		{
			Operations::open_directory(get_project()->get_base_path() / _targetPath);
		}

		// gap
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.0f);

		if (ImGui::Button("Delete File"))
		{
			// delete file and its corresponding .meta file
			std::filesystem::remove(get_project()->get_base_path() / _targetPath);
			std::filesystem::remove(get_project()->get_base_path() / Asset::get_meta_path(_targetPath));

			clear_target();

			get_application().refresh();

			return;
		}
	}

	// show all texts
	for (auto const& text : _texts)
	{
		ImGui::Separator();

		ImGui::TextWrapped(text.c_str());
	}
}

void Mintye::PropertiesWindow::clear_target()
{
	_targetMode = TargetMode::None;
	_targetIsBuiltIn = false;
	_targetId = INVALID_UUID;

	_targetEntity = NULL_ENTITY;
	_targetPath = "";

	_texts.clear();
}

void Mintye::PropertiesWindow::set_target(Minty::Entity const entity)
{
	clear_target();

	if (entity != NULL_ENTITY)
	{
		_targetMode = TargetMode::Entity;
		_targetEntity = entity;
		EntityRegistry& registry = get_scene()->get_entity_registry();
		_targetId = registry.get_id(entity);
	}
}

void Mintye::PropertiesWindow::set_target(Minty::Path const& path)
{
	clear_target();

	AssetEngine& assets = AssetEngine::instance();
	if (assets.exists(path))
	{
		_targetMode = TargetMode::Asset;
		_targetIsBuiltIn = path.string().starts_with("BuiltIn");
		_targetPath = path;

		_targetId = assets.read_id(path);

		// add file itself to be drawn, if it is readable
		if (Asset::is_readable(path))
		{
			_texts.push_back(assets.read_text(path));
		}

		_texts.push_back(assets.read_text(Asset::get_meta_path(path)));
	}
}
