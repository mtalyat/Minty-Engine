#include "GameWindow.h"

using namespace Minty;

Mintye::GameWindow::GameWindow(EditorApplication& application)
	: EditorWindow(application)
{}

void Mintye::GameWindow::draw()
{
	GUI::set_next_window_size(Float2(400, 400), GuiCondition::FirstUseEver);
	if (!GUI::begin("Game", nullptr))
	{
		GUI::end();
		return;
	}



	GUI::end();
}

void Mintye::GameWindow::reset()
{
}

void Mintye::GameWindow::refresh()
{
}
