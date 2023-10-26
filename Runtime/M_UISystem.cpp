#include "pch.h"
#include "M_UISystem.h"

#include "M_UITransformComponent.h"

using namespace minty;

minty::UISystem::UISystem(minty::Engine* const engine, minty::EntityRegistry* const registry)
	: System(engine, registry)
{
	_onMouseMove = [this](MouseMoveEventArgs const& args)
		{
			
		};

	_onMouseClick = [this](MouseClickEventArgs const& args)
		{

		};
}

void minty::UISystem::load()
{
	InputMap* map = _engine->get_global_input_map();

	map->emplace_mouse_move(_onMouseMove);
	map->emplace_mouse(MouseButton::Left, _onMouseClick);
}

void minty::UISystem::update()
{
}

void minty::UISystem::unload()
{
}
