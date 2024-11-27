#include "ME_EditorWindow.h"

#include "ME_EditorApplication.h"

using namespace Minty;
using namespace Mintye;

Mintye::EditorWindow::EditorWindow(EditorApplication& application)
	: _application(&application)
	, _project(nullptr)
	, _scene(nullptr)
{}

Mintye::EditorWindow::~EditorWindow()
{
}

EditorApplication& Mintye::EditorWindow::get_application() const
{
	return *_application;
}

Project* Mintye::EditorWindow::get_project() const
{
	return _project;
}

Minty::Ref<Minty::Scene> Mintye::EditorWindow::get_scene() const
{
	return _scene;
}

void Mintye::EditorWindow::set_project(Project* const project)
{
	_project = project;
}

void Mintye::EditorWindow::set_scene(Ref<Scene> const scene)
{
	_scene = scene;
}
