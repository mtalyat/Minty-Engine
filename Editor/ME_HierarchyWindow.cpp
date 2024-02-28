#include "ME_HierarchyWindow.h"

#include "ME_Application.h"
#include "ME_PropertiesWindow.h"

#include <vector>
#include <unordered_set>

using namespace mintye;
using namespace minty;

mintye::HierarchyWindow::HierarchyWindow(Application& application)
	: EditorWindow(application)
	, _registeredSystems()
	, _selected(NULL_ENTITY)
	, _clicked(NULL_ENTITY)
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
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		set_clicked(NULL_ENTITY);
		MINTY_LOG("Cleared clicked entity");
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
	{
		set_selected(NULL_ENTITY);
	}

	// draw entities
	EntityRegistry* entityRegistry = &scene->get_entity_registry();
	if (ImGui::BeginChild("HierarchyEntities", ImVec2(0.0f, 0.0f), true))
	{
		ImGui::Text(std::format("Entities ({})", entityRegistry->size()).c_str());
		ImGui::Separator();

		// keep track of the parents, keep indenting based on the number of parents
		std::vector<Entity> familyStack;
		std::unordered_set<Entity> familySet;

		size_t i = 0;

		for (auto [entity, relationship] : entityRegistry->view<RelationshipComponent const>().each())
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
			String name = std::format("{}##entity{}", String((familyStack.size() - 1) << 1, ' ').append(entityRegistry->get_name_safe(entity)), i);
			if (ImGui::Selectable(name.c_str(), entity == _selected))
			{
				set_selected(entity);
			}

			// on right click, set clicked
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			{
				set_clicked(entity);
				MINTY_LOG_FORMAT("Clicked entity: \"{}\"", entityRegistry->get_name_safe(entity));

				//ImGui::OpenPopup("ButtonContext");
			}

			i++;
		}

		for (auto [entity] : entityRegistry->view<Entity>().each())
		{
			if (!entityRegistry->valid(entity) || entityRegistry->all_of<RelationshipComponent>(entity)) continue;

			// no parents, print at bottom

			// on left click, select
			String name = std::format("{}##entity{}", entityRegistry->get_name_safe(entity), i);
			if (ImGui::Selectable(name.c_str(), entity == _selected))
			{
				set_selected(entity);
			}

			// on right click, set clicked
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			{
				set_clicked(entity);
				MINTY_LOG_FORMAT("Clicked entity: \"{}\"", entityRegistry->get_name_safe(entity));

				//ImGui::OpenPopup("ButtonContext");
			}

			i++;
		}
	}
	ImGui::EndChild();

	// right-click pop up menu
	// Right-click on the button to open the context menu
	if (ImGui::BeginPopupContextItem("ButtonContext"))
	{
		// TODO: cut, copy, paste
		if (ImGui::MenuItem("Cut", nullptr, false, _clicked != NULL_ENTITY))
		{

		}
		if (ImGui::MenuItem("Copy", nullptr, false, _clicked != NULL_ENTITY))
		{

		}
		if (ImGui::MenuItem("Paste", nullptr, false, _clicked != NULL_ENTITY))
		{
			
		}
		if (ImGui::MenuItem("Paste as child", nullptr, false, _clicked != NULL_ENTITY))
		{

		}

		ImGui::Separator();

		if (ImGui::MenuItem("Rename", nullptr, false, _clicked != NULL_ENTITY))
		{

		}
		if (ImGui::MenuItem("Duplicate", nullptr, false, _clicked != NULL_ENTITY))
		{

		}
		if (ImGui::MenuItem("Delete", nullptr, false, _clicked != NULL_ENTITY))
		{
			if (_clicked == _selected)
			{
				set_selected(NULL_ENTITY);
			}

			entityRegistry->destroy_immediate(_clicked);
			set_clicked(NULL_ENTITY);
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Create empty"))
		{
			// create and select
			set_selected(entityRegistry->create());
			scene->sort();
		}

		ImGui::EndPopup();
	}

	ImGui::End();
}

void mintye::HierarchyWindow::reset()
{
	_selected = NULL_ENTITY;
	_clicked = NULL_ENTITY;
}

void mintye::HierarchyWindow::set_project(Project* const project)
{
	EditorWindow::set_project(project);

	_registeredSystems = SystemRegistry::get_registered_systems();
}

void mintye::HierarchyWindow::set_scene(minty::Scene* const scene)
{
	if (get_scene() != scene)
	{
		// new scene
		_selected = NULL_ENTITY;
	}

	EditorWindow::set_scene(scene);
}

void mintye::HierarchyWindow::set_clicked(minty::Entity const entity)
{
	_clicked = entity;
}

void mintye::HierarchyWindow::set_selected(minty::Entity const entity)
{
	// when selected, send Entity to properties window
	if (PropertiesWindow* properties = get_application().find_editor_window<PropertiesWindow>("Properties"))
	{
		properties->set_target(entity);
	}
	_selected = entity;
}
