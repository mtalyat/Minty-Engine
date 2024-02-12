#include "ME_EditorWindow.h"

#include "ME_Application.h"

using namespace mintye;

mintye::EditorWindow::EditorWindow(Application& application)
	: _application(&application)
{}

mintye::EditorWindow::~EditorWindow()
{
}

Application& mintye::EditorWindow::get_application() const
{
	return *_application;
}

minty::Engine& mintye::EditorWindow::get_engine() const
{
	return _application->get_engine();
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
