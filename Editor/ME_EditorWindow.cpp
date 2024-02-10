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
