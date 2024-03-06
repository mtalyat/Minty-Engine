#include "ME_EditorWindow.h"

#include "ME_EditorApplication.h"

using namespace mintye;

mintye::EditorWindow::EditorWindow(EditorApplication& application)
	: _application(&application)
{}

mintye::EditorWindow::~EditorWindow()
{
}

EditorApplication& mintye::EditorWindow::get_application() const
{
	return *_application;
}

minty::Runtime& mintye::EditorWindow::get_runtime() const
{
	return _application->get_runtime();
}

Project* mintye::EditorWindow::get_project() const
{
	return _project;
}

minty::Scene* mintye::EditorWindow::get_scene() const
{
	return _scene;
}

void mintye::EditorWindow::set_project(Project* const project)
{
	_project = project;
}

void mintye::EditorWindow::set_scene(minty::Scene* const scene)
{
	_scene = scene;
}
