#include "ME_EditorWindow.h"

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

Project* mintye::EditorWindow::get_project() const
{
	return _project;
}

minty::Engine* mintye::EditorWindow::get_engine() const
{
	return _engine;
}

void mintye::EditorWindow::set_project(Project* const project)
{
	_project = project;
}

void mintye::EditorWindow::set_engine(minty::Engine* const engine)
{
	_engine = engine;
}
