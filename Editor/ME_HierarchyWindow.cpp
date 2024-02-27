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

		for (auto const& systemName : _registeredSystems)
		{
			bool checked = systemRegistry.find<System const>(systemName);

			if (ImGui::Checkbox(systemName.c_str(), &checked))
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
		}
	}
	ImGui::EndChild();

	// draw entities
	EntityRegistry* entityRegistry = &scene->get_entity_registry();
	if (ImGui::BeginChild("HierarchyEntities", ImVec2(0.0f, 0.0f), true))
	{
		ImGui::Text(std::format("Entities ({})", entityRegistry->size()).c_str());
		ImGui::Separator();

		// keep track of the parents, keep indenting based on the number of parents
		std::vector<Entity> familyStack;
		std::unordered_set<Entity> familySet;

		for (auto [entity] : entityRegistry->storage<Entity>().each())
		{
			RelationshipComponent const* relationship = entityRegistry->try_get<RelationshipComponent>(entity);

			if (!relationship)
			{
				// no more parents
				if (!familyStack.empty())
				{
					familyStack.clear();
					familySet.clear();
				}
			}
			else
			{
				// family

				// if not the same parent, determine what to do...
				if (familySet.contains(relationship->parent))
				{
					// the parent is part of this line

					// go back until we get to the parent
					while (familyStack.back() != relationship->parent)
					{
						familySet.erase(familyStack.back());
						familyStack.pop_back();
					}

					// add self to stack
					familyStack.push_back(entity);
					familySet.emplace(entity);
				}
				else if (relationship->parent == NULL_ENTITY)
				{
					// no parent, but possibly children
					familyStack.clear();
					familySet.clear();

					familyStack.push_back(entity);
					familySet.emplace(entity);
				}
				else
				{
					// else, out of order
					MINTY_WARN("Entity out of order.");
				}
			}

			// print with indent
			if (ImGui::Selectable(String(familyStack.size() << 1, ' ').append(entityRegistry->get_name(entity)).c_str(), entity == _selected))
			{
				// when selected, send Entity to properties window
				if (PropertiesWindow* properties = get_application().find_editor_window<PropertiesWindow>("Properties"))
				{
					_selected = entity;
					properties->set_target(entity);
				}
			}
		}
	}
	ImGui::EndChild();

	ImGui::End();
}

void mintye::HierarchyWindow::reset()
{

}

void mintye::HierarchyWindow::set_project(Project* const project)
{
	EditorWindow::set_project(project);

	_registeredSystems = SystemRegistry::get_registered_systems();
}
