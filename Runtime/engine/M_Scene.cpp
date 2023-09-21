#include "pch.h"
#include "M_Scene.h"

using namespace minty;

minty::Scene::Scene()
{
}

Registry& minty::Scene::getRegistry()
{
	return _registry;
}
