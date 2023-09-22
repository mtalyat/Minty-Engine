#include "pch.h"
#include "M_Scene.h"

using namespace minty;

minty::Scene::Scene()
{
}

Registry& minty::Scene::get_registry()
{
	return _registry;
}
