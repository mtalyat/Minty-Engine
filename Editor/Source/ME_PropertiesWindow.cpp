#include "ME_PropertiesWindow.h"

#include "ME_EditorApplication.h"
#include "ME_ConsoleWindow.h"
#include "ME_HierarchyWindow.h"
#include "ME_Project.h"
#include "AssetUtility.h"
#include "Meta.h"

using namespace Minty;
using namespace Mintye;

Mintye::PropertiesWindow::PropertiesWindow(EditorApplication& application)
	: EditorWindow(application)
	, m_targetMode(TargetMode::None)
	, m_targetIsBuiltIn(false)
	, m_targetId(INVALID_UUID)
	, m_targetEntity(NULL_ENTITY)
	, m_targetPath()
	, m_texts()
{}

void Mintye::PropertiesWindow::draw()
{
	GUI::set_next_window_size(Float2(400, 400), GuiCondition::FirstUseEver);
	if (!GUI::begin("Properties"))
	{
		GUI::end();
		return;
	}

	// draw based on target mode
	switch (m_targetMode)
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
		GUI::text(std::format("Error: Unknown target mode: {}", static_cast<int>(m_targetMode)).c_str());
		break;
	}

	GUI::end();
}

void Mintye::PropertiesWindow::reset()
{
}

void Mintye::PropertiesWindow::refresh()
{
	Path path = m_targetPath;
	set_target(path);
}

void Mintye::PropertiesWindow::set_scene(Minty::Ref<Minty::Scene> const scene)
{
	if (get_scene() != scene)
	{
		// new scene
		m_targetMode = TargetMode::None;
		m_targetEntity = NULL_ENTITY;
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

	Size i = offset;

	String className = rootNode.get_name();

	while (!nodes.empty())
	{
		// get next node
		std::pair<Node*, int> pair = nodes.back();
		nodes.pop_back();

		// ignore < 0 indents
		if (pair.second < 0) continue;

		String indentString = String(static_cast<Size>(pair.second << 1), ' ');

		// print, if indent is >= 0
		if (pair.second == 0)
		{
			// display text on root
			String string = to_string(*pair.first);
			GUI::text(indentString.append(string.empty() ? "_" : string).c_str());
		}
		else
		{
			// input text

			Size size = std::min(static_cast<Size>(BUFFER_SIZE), pair.first->get_data().size() + 1);
			memcpy(buffer, pair.first->get_data_as_string().c_str(), size);
			buffer[size - 1] = '\0';

			String memberName = pair.first->get_name();

			String text = std::format("{}{}: ", indentString, memberName);
			float leftTextWidth = GUI::calculate_text_size(text.c_str()).x;
			GUI::text(text.c_str());
			GUI::same_line();
			// if an ID, get the name of the object with the ID
			UUID id(INVALID_UUID);
			String idName = "";
			if (Parse::try_uuid(buffer, id))
			{
				idName = get_application().get_name(id);
			}
			float rightTextWidth = GUI::calculate_text_size(idName.c_str()).x;
			String label = std::format("{}##{}", idName, i);
			Type metaType = MetaDatabase::get(className, memberName);
			if (metaType == Type::MultilineString)
			{
				// add 10 so it is within the group box
				if (GUI::input_text_multiline_expand_offset(label.c_str(), buffer, BUFFER_SIZE, leftTextWidth, rightTextWidth + 10.0f))
				{
					pair.first->set_data(buffer);
					modified = true;
				}
			}
			else
			{
				// add 10 so it is within the group box
				if (GUI::input_text_expand_offset(label.c_str(), buffer, BUFFER_SIZE, leftTextWidth, rightTextWidth + 10.0f))
				{
					pair.first->set_data(buffer);
					modified = true;
				}
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
	GUI::text("--");
}

void Mintye::PropertiesWindow::draw_entity()
{
	Ref<Scene> scene = get_scene();

	// no scene?
	if (!scene) return;

	EntityRegistry& registry = get_scene()->get_entity_registry();

	static const Size INPUT_SIZE = 256;
	static char inputBuffer[INPUT_SIZE];
	static char popupBuffer[INPUT_SIZE];

	//		Entity

	GUI::text("Entity:");

	// name
	String text = registry.get_name(m_targetEntity);
	Size size = std::min(INPUT_SIZE, text.size() + 1);
	memcpy(inputBuffer, text.c_str(), size);
	inputBuffer[size - 1] = '\0';

	if (GUI::input_text("Name", inputBuffer, INPUT_SIZE))
	{
		registry.set_name(m_targetEntity, inputBuffer);
	}

	// id
	String idText = to_string(m_targetId);
	GUI::push_style_color(GuiColorID::Text, Color(0.65f, 0.65f, 0.65f, 1.0f));
	if (GUI::button(idText.c_str()))
	{
		// copy ID on click
		Operation::set_clipboard_text(idText);
	}
	GUI::pop_style_color();

	// enabled
	bool enabled = registry.get_enabled(m_targetEntity);
	if (GUI::checkbox("Enabled", enabled))
	{
		registry.set_enabled(m_targetEntity, enabled);
	}

	GUI::same_line();

	// renderable (visible)
	bool visible = registry.get_renderable(m_targetEntity);
	if (GUI::checkbox("Visible", visible))
	{
		registry.set_renderable(m_targetEntity, visible);
	}

	// tag
	text = registry.get_tag(m_targetEntity);
	size = std::min(INPUT_SIZE, text.size() + 1);
	memcpy(inputBuffer, text.c_str(), size);
	inputBuffer[size - 1] = '\0';

	if (GUI::input_text("Tag", inputBuffer, INPUT_SIZE))
	{
		registry.set_tag(m_targetEntity, inputBuffer);
	}

	GUI::separator();

	// these will be ignored in the general components list below,
	// since they are already drawn above
	static std::unordered_set<String> ignoreComponentNames = { "Name", "Tag", "Enabled", "Renderable"};

	//		Components

	GUI::text("Components:");

	// TODO: use TextNodeWriter
	DynamicContainer container;
	TextMemoryWriter writer(&container);
	registry.serialize_entity(writer, m_targetEntity);
	TextMemoryReader reader(&container);
	Node entityNode = reader.get_current_node().get_child(0);

	static const Size OFFSET = static_cast<Size>(1 << 10); // 1024 component entries (should be more than enough)
	Size i = 0;

	static float const xMargin = 2.0f;
	static float const yMargin = 2.0f;
	float const width = GUI::get_content_region_size().x - 2.0f * xMargin;

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
			// treat each script as a component
			for (auto&& childNode : node.get_children())
			{
				// get the node name as an id
				UUID id = Parse::to_uuid(childNode.get_name());

				// get the Script with that ID
				Ref<ScriptClass> childNodeClass = ScriptEngine::find_class(id);

				// should have gotten a class from that ID
				MINTY_ASSERT_FORMAT(childNodeClass != nullptr, "Cannot find a class with the ID \"{}\".", to_string(id));

				// use class's full name
				String childNodeClassName = childNodeClass->get_class_name();
				childNode.set_name(childNodeClassName);

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
	if (GUI::button("Add Component", Float2(width, 0.0f)))
	{
		// clear buffer
		popupBuffer[0] = '\0';

		// show popup
		GUI::open_popup("Add Component");
	}

	if (GUI::begin_popup_modal("Add Component", nullptr, GuiWindowFlags::AlwaysAutoResize))
	{
		GUI::text("Enter the name of the component to add:");

		GUI::set_keyboard_focus_here();
		GUI::input_text("##addComponentText", popupBuffer, INPUT_SIZE);

		if (GUI::button("Done") || GUI::is_key_pressed(Key::Enter))
		{
			// get name
			String name = popupBuffer;

			// check for Script asset
			// ensure it is loaded
			Project* project = get_project();

			Path assetPath = project->find_asset(name, AssetType::Script);
			
			// register it with scene if needed
			if (!assetPath.empty())
			{
				if (!scene->is_registered(assetPath))
				{
					scene->register_asset(assetPath);
				}
			}

			// place onto the object
			if (!registry.emplace_by_name(name, m_targetEntity))
			{
				get_application().log_error(std::format("Failed to add component \"{}\" to Entity \"{}\".", name, registry.get_name(m_targetEntity)));
			}

			memset(popupBuffer, 0, sizeof(popupBuffer));
			GUI::close_current_popup();
		}
		GUI::same_line();
		if (GUI::button("Cancel") || GUI::is_key_pressed(Key::Escape))
		{
			memset(popupBuffer, 0, sizeof(popupBuffer));
			GUI::close_current_popup();
		}

		GUI::end_popup();
	}
}

void Mintye::PropertiesWindow::draw_component(Minty::Node& node, Size const i, Minty::Ref<Minty::Scene> const scene, Minty::EntityRegistry& registry)
{
	static float const xMargin = 2.0f;
	static float const yMargin = 2.0f;
	float const width = GUI::get_content_region_size().x - 2.0f * xMargin;

	static std::unordered_set<String> dirtyableComponentNames = { "Transform", "UITransform", "Canvas" };

	GUI::begin_group_box();

	// list components
	if (input_node(node, true, static_cast<uint32_t>(i)))
	{
		// component changed, update it in the registy
		Component* component = registry.get_by_name(node.get_name(), m_targetEntity);
		EntitySerializationData data
		{
			.scene = get_scene().get(),
			.entity = m_targetEntity,
		};
		TextNodeReader reader(node);
		reader.push_data(&data);
		component->deserialize(reader);

		// if certain component was updated, dirty the entity, update, continue
		if (dirtyableComponentNames.contains(node.get_name()))
		{
			registry.dirty(m_targetEntity);
			scene->finalize();
		}
	}

	// additonal actions for specific components
	if (node.get_name() == "Relationship")
	{
		// only if relationship has a parent
		RelationshipComponent* relationship = static_cast<RelationshipComponent*>(registry.get_by_name(node.get_name(), m_targetEntity));

		if (relationship && relationship->parent != NULL_ENTITY)
		{
			bool canMoveUp = relationship->prev != NULL_ENTITY;
			bool canMoveDown = relationship->next != NULL_ENTITY;

			if (!canMoveUp) GUI::begin_disabled();

			if (GUI::button(std::format("Move Up##{}", i).c_str()))
			{
				registry.move_to_previous(m_targetEntity);
				HierarchyWindow* hierarchy = get_application().find_editor_window<HierarchyWindow>("Hierarchy");
				if (hierarchy) hierarchy->refresh();
			}

			if (!canMoveUp) GUI::end_disabled();

			GUI::same_line();

			if (!canMoveDown) GUI::begin_disabled();

			if (GUI::button(std::format("Move Down##{}", i).c_str()))
			{
				registry.move_to_next(m_targetEntity);
				HierarchyWindow* hierarchy = get_application().find_editor_window<HierarchyWindow>("Hierarchy");
				if (hierarchy) hierarchy->refresh();
			}

			if (!canMoveDown) GUI::end_disabled();

			GUI::same_line();

			if (!canMoveUp) GUI::begin_disabled();

			if (GUI::button(std::format("Move to First##{}", i).c_str()))
			{
				registry.move_to_first(m_targetEntity);
				HierarchyWindow* hierarchy = get_application().find_editor_window<HierarchyWindow>("Hierarchy");
				if (hierarchy) hierarchy->refresh();
			}

			if (!canMoveUp) GUI::end_disabled();

			GUI::same_line();

			if (!canMoveDown) GUI::begin_disabled();

			if (GUI::button(std::format("Move to Last##{}", i).c_str()))
			{
				registry.move_to_last(m_targetEntity);
				HierarchyWindow* hierarchy = get_application().find_editor_window<HierarchyWindow>("Hierarchy");
				if (hierarchy) hierarchy->refresh();
			}

			if (!canMoveDown) GUI::end_disabled();
		}
	}

	// actions
	if (GUI::button(std::format("Remove##{}", i).c_str()))
	{
		registry.erase_by_name(node.get_name(), m_targetEntity);
	}

	GUI::end_group_box(Float2(width, 0.0f), Float2(xMargin, yMargin));
}

void Mintye::PropertiesWindow::draw_asset()
{
	// TODO: do differently for specific asset types

	// print name
	GUI::text(m_targetPath.stem().string().c_str());

	GUI::separator();

	// button ID to copy it
	if (GUI::button(to_string(m_targetId).c_str()))
	{
		Operation::set_clipboard_text(to_string(m_targetId).c_str());
	}

	if (!m_targetIsBuiltIn)
	{
		GUI::separator();

		// button to refresh the contents
		if (GUI::button("Refresh"))
		{
			get_application().refresh();
			return;
		}

		// gap
		GUI::same_line();
		GUI::set_cursor_position_x(GUI::get_cursor_position_x() + 20.0f);

		// button to open the file
		if (GUI::button("Open File"))
		{
			Operation::open(get_project()->get_base_path() / m_targetPath);
		}

		GUI::same_line();

		// button to open the meta file
		if (GUI::button("Open Meta"))
		{
			Operation::open(get_project()->get_base_path() / Asset::get_meta_path(m_targetPath));
		}

		GUI::same_line();

		// button to open the directory
		if (GUI::button("Open Folder"))
		{
			Operation::open_directory(get_project()->get_base_path() / m_targetPath);
		}

		// gap
		GUI::same_line();
		GUI::set_cursor_position_x(GUI::get_cursor_position_x() + 20.0f);

		if (GUI::button("Delete File"))
		{
			// delete file and its corresponding .meta file
			std::filesystem::remove(get_project()->get_base_path() / m_targetPath);
			std::filesystem::remove(get_project()->get_base_path() / Asset::get_meta_path(m_targetPath));

			clear_target();

			get_application().refresh();

			return;
		}
	}

	// show all texts
	for (auto const& text : m_texts)
	{
		GUI::separator();

		GUI::text_wrapped(text.c_str());
	}
}

void Mintye::PropertiesWindow::clear_target()
{
	m_targetMode = TargetMode::None;
	m_targetIsBuiltIn = false;
	m_targetId = INVALID_UUID;

	m_targetEntity = NULL_ENTITY;
	m_targetPath = "";

	m_texts.clear();
}

void Mintye::PropertiesWindow::set_target(Minty::Entity const entity)
{
	clear_target();

	if (entity != NULL_ENTITY)
	{
		m_targetMode = TargetMode::Entity;
		m_targetEntity = entity;
		EntityRegistry& registry = get_scene()->get_entity_registry();
		m_targetId = registry.get_id(entity);
	}
}

void Mintye::PropertiesWindow::set_target(Minty::Path const& path)
{
	clear_target();

	if (AssetManager::exists(path))
	{
		m_targetMode = TargetMode::Asset;
		m_targetIsBuiltIn = path.string().starts_with("BuiltIn");
		m_targetPath = path;

		m_targetId = AssetManager::read_id(path);

		// add file itself to be drawn, if it is readable
		AssetType type = Asset::get_type_from_path(path);
		if (AssetUtility::is_plain_text(type))
		{
			m_texts.push_back(AssetManager::read_file(path));
		}

		m_texts.push_back(AssetManager::read_file(Asset::get_meta_path(path)));
	}
}
