#include "ME_HierarchyWindow.h"

#include "ME_EditorApplication.h"
#include "ME_PropertiesWindow.h"
#include "ME_SceneWindow.h"

#include <vector>
#include <unordered_set>

using namespace Mintye;
using namespace Minty;

Mintye::HierarchyWindow::HierarchyWindow(EditorApplication& application)
	: EditorWindow(application)
	, _registeredSystems()
	, _selected(NULL_ENTITY)
	, _clicked(NULL_ENTITY)
{}

void Mintye::HierarchyWindow::draw()
{
	Ref<Scene> scene = get_scene();

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
	if (ImGui::BeginChild("HierarchySystems", ImVec2(0.0f, std::min(200.0f, splitHeight)), true))
	{
		ImGui::Text(std::format("Systems ({})", systemRegistry.size()).c_str());
		ImGui::Separator();

		// do a checkbox for each system
		// if it has the system, check, if not, no check
		// clicking the check will add/remove the system

		size_t i = 0;

		for (auto const& systemName : _registeredSystems)
		{
			bool checked = systemRegistry.find<System const>(systemName);

			if (ImGui::Checkbox(std::format("{}##system{}", systemName, i).c_str(), &checked))
			{
				if (checked)
				{
					// add system
					systemRegistry.emplace_by_name(systemName);
				}
				else
				{
					// remove system
					systemRegistry.erase_by_name(systemName);
				}
			}

			i++;
		}
	}
	ImGui::EndChild();

	// if clicked in window, reset the clicked Entity
	if (ImGui::IsWindowHovered())
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			set_clicked(NULL_ENTITY);
		}

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			set_selected(NULL_ENTITY);
		}
	}

	// draw entities
	EntityRegistry& entityRegistry = scene->get_entity_registry();
	if (ImGui::BeginChild("HierarchyEntities", ImVec2(0.0f, 0.0f), true))
	{
		ImGui::Text(std::format("Entities ({})", entityRegistry.size()).c_str());
		ImGui::Separator();

		// keep track of the parents, keep indenting based on the number of parents
		std::vector<Entity> familyStack;
		std::unordered_set<Entity> familySet;

		size_t i = 0;

		for (auto [entity, relationship] : entityRegistry.view<RelationshipComponent const>().each())
		{
			// family

			// if not the same parent, determine what to do...
			if (familySet.contains(relationship.parent))
			{
				// the parent is part of this line

				// go back until we get to the parent
				while (familyStack.back() != relationship.parent)
				{
					familySet.erase(familyStack.back());
					familyStack.pop_back();
				}
			}
			else if (relationship.parent == NULL_ENTITY)
			{
				// no parent, but possibly children
				familyStack.clear();
				familySet.clear();
			}
			else
			{
				// else, out of order
				MINTY_WARN("Entity out of order.");

				familyStack.clear();
				familySet.clear();
			}

			// add self to stack
			familyStack.push_back(entity);
			familySet.emplace(entity);

			// on left click, select
			// print with indent
			String name = std::format("{}##entity{}", String((familyStack.size() - 1) << 1, ' ').append(entityRegistry.get_name_safe(entity)), i);
			if (ImGui::Selectable(name.c_str(), entity == _selected))
			{
				set_selected(entity);
			}

			// on right click, set clicked
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			{
				set_clicked(entity);
				MINTY_LOG_FORMAT("Clicked entity: \"{}\"", entityRegistry.get_name_safe(entity));

				//ImGui::OpenPopup("ButtonContext");
			}

			i++;
		}

		for (auto [entity] : entityRegistry.view<Entity>().each())
		{
			if (!entityRegistry.valid(entity) || entityRegistry.all_of<RelationshipComponent>(entity)) continue;

			// no parents, print at bottom

			// on left click, select
			String name = std::format("{}##entity{}", entityRegistry.get_name_safe(entity), i);
			if (ImGui::Selectable(name.c_str(), entity == _selected))
			{
				set_selected(entity);
			}

			// on right click, set clicked
			// on double click, focus
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			{
				if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				{
					set_clicked(entity);
				}
			}

			i++;
		}
	}
	ImGui::EndChild();

	// right-click pop up menu
	// Right-click on the button to open the context menu
	if (ImGui::BeginPopupContextItem("ButtonContext"))
	{
		draw_popup();

		ImGui::EndPopup();
	}

	run_shortcuts();

	ImGui::End();
}

void Mintye::HierarchyWindow::reset()
{
	_selected = NULL_ENTITY;
	_clicked = NULL_ENTITY;
}

void Mintye::HierarchyWindow::refresh()
{
	if(get_scene()) get_scene()->finalize();
}

void Mintye::HierarchyWindow::set_project(Project* const project)
{
	EditorWindow::set_project(project);

	_registeredSystems = SystemRegistry::get_registered_systems();
}

void Mintye::HierarchyWindow::set_scene(Minty::Ref<Minty::Scene> const scene)
{
	if (get_scene() != scene)
	{
		// new scene
		_selected = NULL_ENTITY;
	}

	EditorWindow::set_scene(scene);
}

Minty::Entity Mintye::HierarchyWindow::create_entity()
{
	EntityRegistry& registry = get_scene()->get_entity_registry();

	// create empty entity
	Entity entity = registry.create();

	// enable and visible by default
	registry.enable(entity);
	registry.set_renderable(entity, true);

	return entity;
}

void Mintye::HierarchyWindow::copy_entity(Minty::Entity const entity)
{
	get_application().copy_asset(get_scene()->get_entity_registry().get_id(entity));
}

Minty::Entity Mintye::HierarchyWindow::paste_entity()
{
	if (!get_application().is_asset_copied("Entity")) return NULL_ENTITY;

	std::vector<String> lines = Text::split(ImGui::GetClipboardText(), '\n');
	lines.erase(lines.begin()); // remove the meta text line

	EntityRegistry& registry = get_scene()->get_entity_registry();

	// deserialize copied as entity
	Node node = Node::parse(lines).get_children().front();
	UUID id(0);
	if (Parse::try_uuid(node.get_data(), id) && registry.find_by_id(id) != NULL_ENTITY)
	{
		// already contains ID, so generate a new one
		id = UUID();
		node.set_data(to_string(id));
	}
	return registry.deserialize_entity(node);
}

void Mintye::HierarchyWindow::destroy_entity(Minty::Entity const entity)
{
	if (entity == _clicked)
	{
		set_clicked(NULL_ENTITY);
	}

	if (entity == _selected)
	{
		set_selected(NULL_ENTITY);
	}

	EntityRegistry& registry = get_scene()->get_entity_registry();
	registry.destroy_immediate(entity);
}

Minty::Entity Mintye::HierarchyWindow::clone_entity(Minty::Entity const entity)
{
	EntityRegistry& registry = get_scene()->get_entity_registry();
	return registry.clone(entity);
}

void Mintye::HierarchyWindow::focus_entity(Minty::Entity const entity)
{
	// ignore null
	if (entity == NULL_ENTITY) return;

	// select if not selected
	set_selected(entity);

	SceneWindow* sceneWindow = get_application().find_editor_window<SceneWindow>("Scene");
	
	// ignore if no scene view
	if (!sceneWindow) return;

	sceneWindow->focus(entity);
}

void Mintye::HierarchyWindow::set_clicked(Minty::Entity const entity)
{
	_clicked = entity;
}

void Mintye::HierarchyWindow::set_selected(Minty::Entity const entity)
{
	// when selected, send Entity to properties window
	if (PropertiesWindow* properties = get_application().find_editor_window<PropertiesWindow>("Properties"))
	{
		properties->set_target(entity);
	}
	_selected = entity;
}

void Mintye::HierarchyWindow::draw_popup()
{
	EntityRegistry& registry = get_scene()->get_entity_registry();

	if (ImGui::MenuItem("Cut", nullptr, false, _clicked != NULL_ENTITY))
	{
		// copy entity to clipboard
		copy_entity(_clicked);

		// delete entity
		destroy_entity(_clicked);

		return;
	}
	if (ImGui::MenuItem("Copy", nullptr, false, _clicked != NULL_ENTITY))
	{
		copy_entity(_clicked);

		return;
	}
	String clipboardText = ImGui::GetClipboardText() ? ImGui::GetClipboardText() : "";
	if (ImGui::MenuItem("Paste", nullptr, false, clipboardText.starts_with("Minty Entity")))
	{
		Entity entity = paste_entity();

		// set to same parent as clicked
		Entity parent = registry.get_parent(_clicked);
		registry.set_parent(entity, parent);

		// select
		set_selected(entity);

		return;
	}
	if (ImGui::MenuItem("Paste as child", nullptr, false, clipboardText.starts_with("Minty Entity") && _clicked != NULL_ENTITY))
	{
		Entity entity = paste_entity();

		// set parent as clicked
		registry.set_parent(entity, _clicked);

		// select
		set_selected(entity);

		return;
	}

	ImGui::Separator();

	if (ImGui::MenuItem("Focus", nullptr, false, _clicked != NULL_ENTITY))
	{
		focus_entity(_clicked);

		return;
	}
	if (ImGui::MenuItem("Rename", nullptr, false, false/*_clicked != NULL_ENTITY*/))
	{
		return;
	}
	if (ImGui::MenuItem("Duplicate", nullptr, false, _clicked != NULL_ENTITY))
	{
		set_selected(clone_entity(_clicked));

		return;
	}
	if (ImGui::MenuItem("Delete", nullptr, false, _clicked != NULL_ENTITY))
	{
		destroy_entity(_clicked);

		return;
	}

	ImGui::Separator();

	if (ImGui::MenuItem("Create empty"))
	{
		// create and select
		Entity newEntity = create_entity();
		set_selected(newEntity);
		get_scene()->sort();

		// set parent as clicked
		registry.set_parent(newEntity, _clicked);

		return;
	}
}

void Mintye::HierarchyWindow::run_shortcuts()
{
	EntityRegistry& registry = get_scene()->get_entity_registry();

	ImGuiIO& io = ImGui::GetIO();
	
	// must be focused in the window
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
	{
		// must have ctrl pressed
		if (io.KeyCtrl)
		{
			// must have something selected
			if (_selected != NULL_ENTITY)
			{
				if (ImGui::IsKeyPressed(ImGuiKey_X)) // cut
				{
					copy_entity(_selected);
					destroy_entity(_selected);
				}
				if (ImGui::IsKeyPressed(ImGuiKey_C)) // copy
				{
					copy_entity(_selected);
				}
				if (ImGui::IsKeyPressed(ImGuiKey_D)) // duplicate
				{
					Entity entity = clone_entity(_selected);
					set_selected(entity);
				}
			}

			if (ImGui::IsKeyPressed(ImGuiKey_V)) // paste
			{
				Entity entity = paste_entity();
				registry.set_parent(entity, registry.get_parent(_selected));
				set_selected(entity);
			}
		}

		// no ctrl

		if (_selected != NULL_ENTITY)
		{
			if (ImGui::IsKeyPressed(ImGuiKey_Delete)) // delete
			{
				destroy_entity(_selected);
			}
			if (ImGui::IsKeyPressed(ImGuiKey_F)) // delete
			{
				focus_entity(_selected);
			}
		}
	}
}
