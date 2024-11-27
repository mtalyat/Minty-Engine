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

	GUI::set_next_window_size(Float2(400, 800), GuiCondition::FirstUseEver);
	if (!GUI::begin("Hierarchy") || !scene)
	{
		GUI::end();
		return;
	}

	// if clicked in window, reset the clicked Entity
	if (GUI::is_window_hovered())
	{
		if (GUI::is_mouse_clicked(MouseButton::Right))
		{
			set_clicked(NULL_ENTITY);

			GUI::open_popup("ButtonContext");
		}

		if (GUI::is_mouse_clicked(MouseButton::Left))
		{
			set_selected(NULL_ENTITY);
		}
	}

	// draw entities
	EntityRegistry& entityRegistry = scene->get_entity_registry();
	GUI::text(std::format("Entities ({})", entityRegistry.size()).c_str());
	GUI::separator();

	// keep track of the parents, keep indenting based on the number of parents
	std::vector<Entity> familyStack;
	std::unordered_set<Entity> familySet;

	Size i = 0;

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
		if (GUI::selectable(name.c_str(), entity == _selected))
		{
			set_selected(entity);
		}

		// on right click, set clicked
		if (GUI::is_item_clicked(MouseButton::Right) && GUI::is_item_hovered(GuiHoveredFlags::AllowWhenBlockedByActiveItem))
		{
			set_clicked(entity);
			//MINTY_LOG_FORMAT("Clicked entity: \"{}\"", entityRegistry.get_name_safe(entity));

			GUI::open_popup("ButtonContext");
		}

		i++;
	}

	for (auto [entity] : entityRegistry.view<Entity>().each())
	{
		if (!entityRegistry.valid(entity) || entityRegistry.all_of<RelationshipComponent>(entity)) continue;

		// no parents, print at bottom

		// on left click, select
		String name = std::format("{}##entity{}", entityRegistry.get_name_safe(entity), i);
		if (GUI::selectable(name.c_str(), entity == _selected))
		{
			set_selected(entity);
		}

		// on right click, set clicked
		// on double click, focus
		if (GUI::is_item_hovered(GuiHoveredFlags::AllowWhenBlockedByActiveItem))
		{
			if (GUI::is_item_clicked(MouseButton::Right))
			{
				set_clicked(entity);
			}
		}

		i++;
	}

	// right-click pop up menu
	// Right-click on the button to open the context menu
	if (GUI::begin_popup_context_item("ButtonContext"))
	{
		draw_popup();

		GUI::end_popup();
	}

	run_shortcuts();

	GUI::end();
}

void Mintye::HierarchyWindow::reset()
{
	_selected = NULL_ENTITY;
	_clicked = NULL_ENTITY;
}

void Mintye::HierarchyWindow::refresh()
{
	if(get_scene().get()) get_scene()->finalize();
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

	// add transform by default
	registry.emplace<TransformComponent>(entity);

	return entity;
}

void Mintye::HierarchyWindow::copy_entity(Minty::Entity const entity)
{
	get_application().copy_asset(get_scene()->get_entity_registry().get_id(entity));
}

Minty::Entity Mintye::HierarchyWindow::paste_entity()
{
	if (!get_application().is_asset_copied("Entity")) return NULL_ENTITY;

	EntityRegistry& registry = get_scene()->get_entity_registry();

	String text = Operation::get_clipboard_text();
	text = replace(text, "\r", "");
	
	// remove the meta text line
	Size index = text.find('\n', 0);
	if (index != String::npos)
	{
		text = text.substr(0, index);
	}

	// create container from clipboard text
	StaticContainer container(text.size() + 1);
	container.set(text.data(), text.size());
	Char eol = '\0';
	container.set_at(&eol, sizeof(Char), text.size());

	// read it
	TextMemoryReader reader(&container);
	return registry.deserialize_entity(reader);
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

	if (GUI::menu_item("Cut", "", false, _clicked != NULL_ENTITY))
	{
		// copy entity to clipboard
		copy_entity(_clicked);

		// delete entity
		destroy_entity(_clicked);

		return;
	}
	if (GUI::menu_item("Copy", "", false, _clicked != NULL_ENTITY))
	{
		copy_entity(_clicked);

		return;
	}
	String clipboardText = Operation::get_clipboard_text();
	if (GUI::menu_item("Paste", "", false, clipboardText.starts_with("Minty Entity")))
	{
		Entity entity = paste_entity();

		// set to same parent as clicked
		Entity parent = registry.get_parent(_clicked);
		registry.set_parent(entity, parent);

		// select
		set_selected(entity);

		return;
	}
	if (GUI::menu_item("Paste as child", "", false, clipboardText.starts_with("Minty Entity") && _clicked != NULL_ENTITY))
	{
		Entity entity = paste_entity();

		// set parent as clicked
		registry.set_parent(entity, _clicked);

		// select
		set_selected(entity);

		return;
	}

	GUI::separator();

	if (GUI::menu_item("Focus", "", false, _clicked != NULL_ENTITY))
	{
		focus_entity(_clicked);

		return;
	}
	if (GUI::menu_item("Rename", "", false, false/*_clicked != NULL_ENTITY*/))
	{
		return;
	}
	if (GUI::menu_item("Duplicate", "", false, _clicked != NULL_ENTITY))
	{
		set_selected(clone_entity(_clicked));

		return;
	}
	if (GUI::menu_item("Delete", "", false, _clicked != NULL_ENTITY))
	{
		destroy_entity(_clicked);

		return;
	}

	GUI::separator();

	if (GUI::menu_item("Create empty"))
	{
		// create and select
		Entity newEntity = create_entity();
		set_selected(newEntity);
		get_scene()->sort();

		// set parent as clicked, if there was one
		if (_clicked != NULL_ENTITY)
		{
			registry.set_parent(newEntity, _clicked);
		}

		return;
	}
}
void Mintye::HierarchyWindow::run_shortcuts()
{
	EntityRegistry& registry = get_scene()->get_entity_registry();

	// must be focused in the window
	if (GUI::is_window_focused(GuiFocusedFlags::ChildWindows))
	{
		// must have ctrl pressed
		if (GUI::is_key_modifier_pressed(KeyModifiers::Control))
		{
			// must have something selected
			if (_selected != NULL_ENTITY)
			{
				if (GUI::is_key_pressed(Key::X)) // cut
				{
					copy_entity(_selected);
					destroy_entity(_selected);
				}
				if (GUI::is_key_pressed(Key::C)) // copy
				{
					copy_entity(_selected);
				}
				if (GUI::is_key_pressed(Key::D)) // duplicate
				{
					Entity entity = clone_entity(_selected);
					set_selected(entity);
				}
			}

			if (GUI::is_key_pressed(Key::V)) // paste
			{
				Entity entity = paste_entity();
				registry.set_parent(entity, registry.get_parent(_selected));
				set_selected(entity);
			}
		}

		// no ctrl

		if (_selected != NULL_ENTITY)
		{
			if (GUI::is_key_pressed(Key::Delete)) // delete
			{
				destroy_entity(_selected);
			}
			if (GUI::is_key_pressed(Key::F)) // delete
			{
				focus_entity(_selected);
			}
		}
	}
}
