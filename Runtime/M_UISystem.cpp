#include "pch.h"
#include "M_UISystem.h"

#include "M_Runtime.h"
#include "M_UITransformComponent.h"
#include "M_Scene.h"

using namespace minty;

minty::UISystem::UISystem(Runtime& engine, ID const sceneId)
	: System(engine, sceneId)
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
	InputMap& map = get_scene().get_engine().get_global_input_map();

	map.emplace_mouse_move(_onMouseMove);
	map.emplace_mouse(MouseButton::Left, _onMouseClick);
}

void minty::UISystem::update()
{
}

void minty::UISystem::unload()
{
}
