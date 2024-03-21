#include "pch.h"
#include "M_UISystem.h"

#include "M_Runtime.h"
#include "M_UITransformComponent.h"
#include "M_Scene.h"

using namespace minty;

minty::UISystem::UISystem(Runtime& engine, Scene& scene)
	: System("UI", engine, scene)
{
	//_onMouseMove = [this](MouseMoveEventArgs const& args)
	//	{
	//		
	//	};

	//_onMouseClick = [this](MouseClickEventArgs const& args)
	//	{

	//	};
}
