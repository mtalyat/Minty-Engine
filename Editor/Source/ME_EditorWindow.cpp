#include "ME_EditorWindow.h"

#include "ME_EditorApplication.h"

using namespace Minty;
using namespace Mintye;

Mintye::EditorWindow::EditorWindow(EditorApplication& application)
	: mp_application(&application)
	, mp_project(nullptr)
	, m_scene(nullptr)
{}

Mintye::EditorWindow::~EditorWindow()
{
}

EditorApplication& Mintye::EditorWindow::get_application() const
{
	return *mp_application;
}

Project* Mintye::EditorWindow::get_project() const
{
	return mp_project;
}

Minty::Ref<Minty::Scene> Mintye::EditorWindow::get_scene() const
{
	return m_scene;
}

void Mintye::EditorWindow::set_project(Project* const project)
{
	mp_project = project;
}

void Mintye::EditorWindow::set_scene(Ref<Scene> const scene)
{
	m_scene = scene;
}
