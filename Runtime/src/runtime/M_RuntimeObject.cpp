#include "pch.h"
#include "runtime/M_RuntimeObject.h"

#include "runtime/M_Runtime.h"
#include "scenes/M_SceneManager.h"
#include "scenes/M_Scene.h"

using namespace minty;

minty::RuntimeObject::RuntimeObject()
	: _runtime(nullptr)
{}

minty::RuntimeObject::RuntimeObject(Runtime& engine)
	: _runtime(&engine)
{}

minty::RuntimeObject::~RuntimeObject()
{
}

Runtime& minty::RuntimeObject::get_runtime() const
{
	MINTY_ASSERT(_runtime != nullptr);

	return *_runtime;
}

void minty::RuntimeObject::set_runtime(Runtime& engine)
{
	_runtime = &engine;
}
